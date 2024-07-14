#include <iostream>
#include <fstream> 
#include <thread>
#include <chrono>
#include <winsock2.h>
#include <string>
#include <tuple>
#include <iomanip>

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp> 
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"
#include <filesystem>

#include "nxLib.h"
#include "camera_functions.h"
#include "OpenCV.h"
#include "file_functions.h"
#include "Struct.h"
#include "Global_vars.h"
#include "Robot_server.h"
#include "configure_workspace.h"
#include "Calib_method.h"
#include "Utils.h"
#include "Optimization.h"

#pragma comment(lib, "ws2_32.lib")

cv::Mat poseMatrix;
std::vector<cv::Mat> RobMatrix_vec;
std::vector<cv::Mat> CamMatrix_vec;

std::string Matrix_cam_file = "OutFiles/Matrix_Cam_pos.txt";
std::string Matrix_rob_file = "OutFiles/Matrix_Rob_pos.txt";
std::ofstream OutMatrixCamFile;
std::ofstream OutMatrixRobFile;

std::vector<std::string> lines;

int main()
{
    int project_part = ProjectPart();
    if (project_part == 0)
    {
        std::vector<Limit> limiti(3);
        poruka = { 0, 0, 0, 0, 0, 0, 1 };
        Offset offset_trial = { 0, 0, 0, 0, 0, 0, 0 };
        int i = 0;
        bool CaptureSuccess = true;
        usao_u_catch = false;
        CommMsg home_pos;
        std::thread StartServer(runServer);
        std::string tempDir = CreateTempDirectory();
        std::cout << "Temporary dir created" << tempDir << std::endl;
        std::string cameraSerial = StartCamera();
        if (project_part == 0) {
            user_input = UserInput();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        inital_position = { comm_msg.x, comm_msg.y, comm_msg.z, comm_msg.a, comm_msg.b, comm_msg.c };
        MoveHome(inital_position);

        bool CaptureSucess = true;
        std::vector<cv::Point2f> imagePoints;
        inputFile("input.txt",lines);
        int Pattern1; int Pattern2;  int Char1; int Char2;
        std::stringstream stream(lines[1]); stream >> Pattern1 >> Pattern2 >> square_size >> Char1 >> Char2 >> squareLength >> markerLength >> 
            step_x >> step_y >> step_z >> estimated_it >> it >> rotation_range;
        cv::Size PatternSize(Pattern1, Pattern2); // 9 6

        // Charuco pattern borad
        cv::Size CharucoSize(Char1, Char2); // 5 ,7

        WriteParamFile(cameraSerial);
        std::string ParamsFilePath = "calibrated_params.json";
        cameraParams = readCameraParameters(ParamsFilePath);
        cameraMatrix = std::get<0>(cameraParams);
        distortionCoeffs = (cv::Mat_<double>(1, 5) << 0, 0, 0, 0, 0);

        OutMatrixCamFile.open(Matrix_cam_file);
        OutMatrixRobFile.open(Matrix_rob_file);


        if (user_input == 1) // Ako je ENSENSO
        {
            InitizalizePatternCapture();
        }
        for (int g = 0; g <= 2; g++)
        {
            cv::Mat image;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            home_pos = { comm_msg.x, comm_msg.y, comm_msg.z + offset_trial.z_off, comm_msg.a, comm_msg.b, comm_msg.c, 1 };
            MoveAbsLIN(home_pos);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (g == 0) {
                offset_trial.z_off = 0; // ne treba se podizati za prvu poziciju
            }
            else {
                offset_trial.z_off = step_z;
            }
            home_pos = { comm_msg.x, comm_msg.y, comm_msg.z + offset_trial.z_off, comm_msg.a, comm_msg.b, comm_msg.c, 1 };
            MoveAbsLIN(home_pos);
            limiti[g].z = comm_msg.z + offset_trial.z_off;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            offset_trial.z_off = 0;

            switch (user_input)
            {
            case 1:
                X_Movement_Ensenso(home_pos, g, offset_trial, limiti, CaptureSuccess, cameraSerial, tempDir, i);
                Y_Movement_Ensenso(home_pos, g, offset_trial, limiti, CaptureSuccess, cameraSerial, tempDir, i);
                break;
            case 2:
                X_Movement_OpenCV(home_pos, g, offset_trial, limiti, CaptureSuccess, cameraSerial, tempDir, i, PatternSize, imagePoints, image);
                Y_Movement_OpenCV(home_pos, g, offset_trial, limiti, CaptureSuccess, cameraSerial, tempDir, i, PatternSize, imagePoints, image);
                break;
            case 3:
                X_Movement_CharucoCV(home_pos, g, offset_trial, limiti, CaptureSuccess, cameraSerial, tempDir, i, image, CharucoSize);
                Y_Movement_CharucoCV(home_pos, g, offset_trial, limiti, CaptureSuccess, cameraSerial, tempDir, i, image, CharucoSize);
                break;
            default:
                break;
            }
            std::cout << "x_neg: " << limiti[g].x_neg << std::endl;
            std::cout << "x_poz: " << limiti[g].x_poz << std::endl;
            std::cout << "y_neg: " << limiti[g].y_neg << std::endl;
            std::cout << "y_poz: " << limiti[g].y_poz << std::endl;
            std::cout << "z: " << limiti[g].z << std::endl;
        }

        // generiranje nasumicne pozicje 
        if (user_input == 1)
        {
            InitizalizePatternCapture();
        }
        bool capture_success = false;
        int s = 0;
        int pozicija = 0;
        int error_count = 0;

        while (s <= estimated_it)
        {
            if (s <= 7)
            {
                pozicija = 0;
            }
            else if (s <= 13) {
                pozicija = 1;
            }
            else {
                pozicija = 2;
            }
            double x_ = generateRandomNumber(limiti[pozicija].x_neg, limiti[pozicija].x_poz);
            double y_ = generateRandomNumber(limiti[pozicija].y_neg, limiti[pozicija].y_poz);
            double middle_point_x = (limiti[pozicija].x_neg + limiti[pozicija].x_poz) / 2;
            double middle_point_y = (limiti[pozicija].y_neg + limiti[pozicija].y_poz) / 2;
            std::cout << "midx: " << middle_point_x << std::endl;
            std::cout << "midy: " << middle_point_y << std::endl;
            double rot_ = generateRandomNumber(-rotation_range, rotation_range);

            if (error_count >= 5)
            {
                error_count = 0;
                MoveHome(inital_position);
                std::this_thread::sleep_for(std::chrono::milliseconds(3000));
            }
            MoveAbsNoRotLIN(x_, y_, limiti[pozicija].z);
            Offset mala_rot = { 0,0,0,rot_,rot_,rot_};
            ToolRotWorldPos(mala_rot);
            cv::Mat image;
            switch (user_input) {
            case 1:
                capture_success = CaptureImages(cameraSerial, true);
                break;
            case 2:
                capture_success = FindPatternFunctionCV(cameraSerial, tempDir, i, PatternSize, imagePoints, image);
                break;
            case 3:
                capture_success = FindPatternFunctionCharucoCV(cameraSerial, tempDir, i, CharucoSize, image);
                break;
            default:
                std::cerr << "Invalid user input" << std::endl;
                capture_success = false;
                break;
            }
            if (capture_success)
            {
                if (user_input == 2) {
                    cv::Mat R_target2cam;
                    cv::Mat t_target2cam;
                    SolvePnP(PatternSize,imagePoints, cameraMatrix, distortionCoeffs, image, R_target2cam, t_target2cam);
                    cv::Mat target2cam = createTransformationMatrix(R_target2cam, t_target2cam);
                    OutMatrixCamFile << target2cam << std::endl;
                    i++;
                    std::cout << "Image captured sucessfully, slika: " << s << std::endl;

                    cv::Vec3d euler((comm_msg.a) * (CV_PI / 180.0), (comm_msg.b) * (CV_PI / 180.0), (comm_msg.c) * (CV_PI / 180.0));
                    cv::Mat R_gripper2base = EulerToMatrix(euler);
                    cv::Mat t_gripper2base = (cv::Mat_<double>(3, 1) << comm_msg.x / 1000, comm_msg.y / 1000, comm_msg.z / 1000);
                    cv::Mat gripper2base = createTransformationMatrix(R_gripper2base, t_gripper2base);
                    OutMatrixRobFile << gripper2base << std::endl;
                    s++;
                }
                else if (user_input == 1) {
                    std::cout << "Image captured sucessfully" << std::endl;
                    SaveImages(s, cameraSerial);
                    cv::Vec3d euler(comm_msg.c * (CV_PI / 180.0), comm_msg.b * (CV_PI / 180.0), comm_msg.a * (CV_PI / 180.0));
                    cv::Vec3d positions(comm_msg.x, comm_msg.y, comm_msg.z);
                    cv::Vec4d angles = EulerToAngleAxis(euler);
                    cv::Mat quat_row(1, 7, CV_64F);
                    quat_row.at<double>(0, 0) = positions[0];
                    quat_row.at<double>(0, 1) = positions[1];
                    quat_row.at<double>(0, 2) = positions[2];
                    quat_row.at<double>(0, 3) = angles[0]; //theta
                    quat_row.at<double>(0, 4) = angles[1];
                    quat_row.at<double>(0, 5) = angles[2];
                    quat_row.at<double>(0, 6) = angles[3];
                    quat_row.copyTo(poseMatrix.row(s));
                    s++;
                }
                else if (user_input == 3) {

                    cv::Mat R_target2cam;
                    cv::Mat t_target2cam;
                    cv::Mat target2cam;
                    EstimatePoseCharucoBoard(image, CharucoSize, R_target2cam, t_target2cam);
                    target2cam = createTransformationMatrix(R_target2cam, t_target2cam);
                    OutMatrixCamFile << target2cam << std::endl;
                    i++;
                    std::cout << "Image captured sucessfully, slika: " << s << std::endl;

                    cv::Vec3d euler((comm_msg.a) * (CV_PI / 180.0), (comm_msg.b) * (CV_PI / 180.0), (comm_msg.c) * (CV_PI / 180.0));
                    cv::Mat R_gripper2base = EulerToMatrix(euler);
                    cv::Mat t_gripper2base = (cv::Mat_<double>(3, 1) << comm_msg.x / 1000, comm_msg.y / 1000, comm_msg.z / 1000);
                    cv::Mat gripper2base = createTransformationMatrix(R_gripper2base, t_gripper2base);
                    OutMatrixRobFile << gripper2base << std::endl;
                    s++;
                }
            }
            else
            {
                std::cout << "Failed to capture image" << std::endl;
                error_count++;
            }
        }
        cv::Mat EstimatedCam2Gripper = CalibrateHandEyeCVBoard(Matrix_rob_file, Matrix_cam_file, RobMatrix_vec, CamMatrix_vec);
        cv::Mat identityMatrix = cv::Mat::eye(4, 4, CV_64F);
        if ((cv::countNonZero(EstimatedCam2Gripper != identityMatrix) == 0))
        {
            std::cout << "Not enough usable data" << std::endl;
            return 1;
        }
        clearFile(OutMatrixRobFile, Matrix_rob_file);
        clearFile(OutMatrixCamFile, Matrix_cam_file);
        std::cout << "esti: " << EstimatedCam2Gripper << std::endl;
        MoveHome(inital_position);
        s = 0;
        cv::Mat image;
        bool found = false;
        double a_inital = comm_msg.a, b_inital = comm_msg.b, c_inital = comm_msg.c;
        while (s <= it)
        {
            for (int g = 0; g < limiti.size(); g++)
            {
                double x_middle = (limiti[g].x_neg + limiti[g].x_poz) / 2;
                double y_middle = (limiti[g].y_neg + limiti[g].y_poz) / 2;

                std::vector<std::vector<double>> VALUES;

                std::vector<double> x_values = { limiti[g].x_neg, limiti[g].x_poz, y_middle };
                std::vector<double> y_values = { limiti[g].y_neg, limiti[g].y_poz, x_middle };
                VALUES.push_back(x_values);
                VALUES.push_back(y_values);
                Offset offset = {x_middle, y_middle,limiti[g].z,a_inital,b_inital,c_inital, 1};
                double rot_y = 0; double rot_x = 0;
                for (int j = 0; j < VALUES.size(); j++)
                {
                    for (int k = VALUES[j][0]; k <= VALUES[j][1]; k += abs(VALUES[j][0] - VALUES[j][1]) / 2)
                    {
                        MoveAbsPTP(offset);
                        usao_u_catch = false;
                        if (j == 0)
                        {
                            rot_y = generateRandomNumber(-rotation_range, rotation_range);
                            rot_x = generateRandomNumber(-rotation_range, rotation_range);
                            MoveAbsNoRotLIN(k, VALUES[j][2], limiti[g].z);
                            std::cout << "x: " << k << std::endl;
                            std::cout << "y: " << VALUES[j][2] << std::endl;
                            if (!usao_u_catch)
                            {
                                if (user_input == 3)
                                {
                                    found = RotateAroundPoint(cameraSerial, tempDir, CharucoSize, rot_x, rot_y, EstimatedCam2Gripper);
                                }
                                else if(user_input == 2)
                                {
                                    found = RotateAroundPointCV(cameraSerial, tempDir, PatternSize, imagePoints, image, rot_x, rot_y, EstimatedCam2Gripper);
                                }
                            }
                        }
                        else
                        {
                            rot_y = generateRandomNumber(-15, 15);
                            rot_x = generateRandomNumber(-15, 15);
                            MoveAbsNoRotLIN(VALUES[j][2], k, limiti[g].z);
                            std::cout << "x: " << k << std::endl;
                            std::cout << "y: " << VALUES[j][2] << std::endl;
                            if (!usao_u_catch)
                            {
                                if (user_input == 3)
                                {
                                    found = RotateAroundPoint(cameraSerial, tempDir, CharucoSize, rot_x, rot_y, EstimatedCam2Gripper);
                                }
                                else if (user_input == 2)
                                {
                                    found = RotateAroundPointCV(cameraSerial, tempDir, PatternSize, imagePoints, image, rot_x, rot_y, EstimatedCam2Gripper);
                                }
                            }
                        }
                        if (!usao_u_catch)

                        {
                            cv::Mat image;
                            switch (user_input) {
                            case 1:
                                capture_success = CaptureImages(cameraSerial, true);
                                break;
                            case 2:
                                capture_success = FindPatternFunctionCV(cameraSerial, tempDir, i, PatternSize, imagePoints, image);
                                break;
                            case 3:
                                capture_success = FindPatternFunctionCharucoCV(cameraSerial, tempDir, i, CharucoSize, image);
                                break;
                            default:
                                std::cerr << "Invalid user input" << std::endl;
                                capture_success = false;
                                break;
                            }
                            if (capture_success && found)
                            {
                                if (user_input == 2) {
                                    cv::Mat R_target2cam;
                                    cv::Mat t_target2cam;
                                    SolvePnP(PatternSize, imagePoints, cameraMatrix, distortionCoeffs, image, R_target2cam, t_target2cam);
                                    cv::Mat target2cam = createTransformationMatrix(R_target2cam, t_target2cam);
                                    OutMatrixCamFile << target2cam << std::endl;
                                    i++;
                                    std::cout << "Image captured sucessfully, slika: " << s << std::endl;

                                    cv::Vec3d euler((comm_msg.a) * (CV_PI / 180.0), (comm_msg.b) * (CV_PI / 180.0), (comm_msg.c) * (CV_PI / 180.0));
                                    cv::Mat R_gripper2base = EulerToMatrix(euler);
                                    cv::Mat t_gripper2base = (cv::Mat_<double>(3, 1) << comm_msg.x / 1000, comm_msg.y / 1000, comm_msg.z / 1000);
                                    cv::Mat gripper2base = createTransformationMatrix(R_gripper2base, t_gripper2base);
                                    OutMatrixRobFile << gripper2base << std::endl;
                                    s++;
                                }
                                else if (user_input == 1) {
                                    std::cout << "Image captured sucessfully" << std::endl;
                                    SaveImages(s, cameraSerial);
                                    cv::Vec3d euler(comm_msg.c * (CV_PI / 180.0), comm_msg.b * (CV_PI / 180.0), comm_msg.a * (CV_PI / 180.0));
                                    cv::Vec3d positions(comm_msg.x, comm_msg.y, comm_msg.z);
                                    cv::Vec4d angles = EulerToAngleAxis(euler);
                                    cv::Mat quat_row(1, 7, CV_64F);
                                    quat_row.at<double>(0, 0) = positions[0];
                                    quat_row.at<double>(0, 1) = positions[1];
                                    quat_row.at<double>(0, 2) = positions[2];
                                    quat_row.at<double>(0, 3) = angles[0]; //theta
                                    quat_row.at<double>(0, 4) = angles[1];
                                    quat_row.at<double>(0, 5) = angles[2];
                                    quat_row.at<double>(0, 6) = angles[3];
                                    quat_row.copyTo(poseMatrix.row(s));
                                    s++;
                                }
                                else if (user_input == 3) {

                                    cv::Mat R_target2cam;
                                    cv::Mat t_target2cam;
                                    cv::Mat target2cam;
                                    EstimatePoseCharucoBoard(image, CharucoSize, R_target2cam, t_target2cam);
                                    target2cam = createTransformationMatrix(R_target2cam, t_target2cam);
                                    OutMatrixCamFile << target2cam << std::endl;
                                    i++;

                                    std::cout << "Image captured sucessfully, slika: " << s << std::endl;

                                    cv::Vec3d euler((comm_msg.a) * (CV_PI / 180.0), (comm_msg.b) * (CV_PI / 180.0), (comm_msg.c) * (CV_PI / 180.0));
                                    cv::Mat R_gripper2base = EulerToMatrix(euler);
                                    cv::Mat t_gripper2base = (cv::Mat_<double>(3, 1) << comm_msg.x / 1000, comm_msg.y / 1000, comm_msg.z / 1000);
                                    cv::Mat gripper2base = createTransformationMatrix(R_gripper2base, t_gripper2base);
                                    OutMatrixRobFile << gripper2base << std::endl;
                                    s++;
                                }
                            }
                            else
                            {
                                std::cout << "Failed to capture image" << std::endl;
                                error_count++;
                            }
                        }
                    }
                }
            }
        }
        cv::Mat cam2gripper = CalibrateHandEyeCVBoard(Matrix_rob_file, Matrix_cam_file, RobMatrix_vec, CamMatrix_vec);
        ValidateTranslation(cameraSerial, tempDir, CharucoSize,cam2gripper,PatternSize,imagePoints,image);
        ValidateRotation(cameraSerial, tempDir, CharucoSize, cam2gripper,PatternSize,imagePoints,image);
        CloseCamera();
        serverRunning = false;
        if (StartServer.joinable()) {
            StartServer.join();
        }
        return 0;
    }
    if (project_part == 1)
    {
        CalibrateHandEyeCVBoard(Matrix_rob_file, Matrix_cam_file, RobMatrix_vec, CamMatrix_vec);
    }
    return 0;

}


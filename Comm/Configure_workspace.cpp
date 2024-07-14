#include "Configure_workspace.h"

void X_Movement(CommMsg home_pos, int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSuccess,
    std::string cameraSerial, std::string tempDir, int i, cv::Size PatternSize, std::vector<cv::Point2f>& imagePoints, int UserInput, cv::Mat image,
    cv::Size charucoSize)
{
    for (int j = -1; j <= 1; j += 2)
    {
        if (j == -1) {
            limiti[g].x_neg = comm_msg.x;
        }
        else {
            limiti[g].x_poz = comm_msg.x;
        }                                      // u slucaju da se ne moze ni jednom maknuti
        while (!usao_u_catch && CaptureSuccess) // izvrsava se dok ne dosegne max reach ili ne vidi plocu vise
        {
            poruka = { 0, 0, 0, 0, 0, 0, 1 };
            offset_trial.x_off = step_x * j;
            MoveLIN(offset_trial);
            if (j == -1) {
                limiti[g].x_neg = comm_msg.x;
            }
            else {
                limiti[g].x_poz = comm_msg.x;
            }
            switch (UserInput)
            {
            case 1: //ENSENSO
                CaptureSuccess = CaptureImages(cameraSerial, true);
                break;
            case 2: //OpenCV
                CaptureSuccess = FindPatternFunctionCV(cameraSerial, tempDir, i, PatternSize, imagePoints, image);
                break;
            case 3: //ChArUco
                CaptureSuccess = FindPatternFunctionCharucoCV(cameraSerial, tempDir, i, charucoSize, image);
                break;
            default:
                break;
            }
            if (CaptureSuccess) {
                std::cout << "Pattern visible" << std::endl;
            }
            else {
                std::cout << "Pattern not visible" << std::endl;
            }
        }
        // potrebno je sacuvati koordinatu u world-u

        usao_u_catch = false;
        CaptureSuccess = true;
        offset_trial.x_off = 0;
        MoveAbsLIN(home_pos);
    }
}

void X_Movement_Ensenso(CommMsg home_pos, int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSuccess,
    std::string cameraSerial, std::string tempDir, int i)
{
    static std::vector<cv::Point2f> emptyImagePoints;
    X_Movement(home_pos, g, offset_trial, limiti, CaptureSuccess, cameraSerial, tempDir, i, {}, emptyImagePoints, 1, {}, {});
}

void X_Movement_OpenCV(CommMsg home_pos, int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSuccess,
    std::string cameraSerial, std::string tempDir, int i, cv::Size PatternSize, std::vector<cv::Point2f>& imagePoints, cv::Mat image)
{
    X_Movement(home_pos, g, offset_trial, limiti, CaptureSuccess, cameraSerial, tempDir, i, PatternSize, imagePoints, 2, image, {});
}

void X_Movement_CharucoCV(CommMsg home_pos,int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSucess,
    std::string cameraSerial, std::string tempDir, int i,cv::Mat image, cv::Size charucoSize)
{
    static std::vector<cv::Point2f> emptyImagePoints;
    X_Movement(home_pos, g, offset_trial, limiti, CaptureSucess, cameraSerial, tempDir, i, {}, emptyImagePoints, 3, image, charucoSize);
}


void Y_Movement(CommMsg home_pos, int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSuccess,
    std::string cameraSerial, std::string tempDir, int i, cv::Size PatternSize, std::vector<cv::Point2f>& imagePoints, int UserInput, cv::Mat image,
    cv::Size charucoSize)
{
    for (int j = -1; j <= 1; j += 2)
    {
        if (j == -1) {
            limiti[g].y_neg = comm_msg.y;
        }
        else {
            limiti[g].y_poz = comm_msg.y;
        }                                      // u slucaju da se ne moze ni jednom maknuti
        while (!usao_u_catch && CaptureSuccess) // izvrsava se dok ne dosegne max reach ili ne vidi plocu vise
        {
            poruka = { 0, 0, 0, 0, 0, 0, 1 };
            offset_trial.y_off = step_y * j;
            MoveLIN(offset_trial);
            if (j == -1) {
                limiti[g].y_neg = comm_msg.y;
            }
            else {
                limiti[g].y_poz = comm_msg.y;
            }
            switch (UserInput)
            {
            case 1: //ENSENSO
                CaptureSuccess = CaptureImages(cameraSerial, true);
                break;
            case 2: //OpenCV
                CaptureSuccess = FindPatternFunctionCV(cameraSerial, tempDir, i, PatternSize, imagePoints, image);
                break;
            case 3: //ChArUco
                CaptureSuccess = FindPatternFunctionCharucoCV(cameraSerial, tempDir, i, charucoSize, image);
                break;
            default:
                break;
            }
            if (CaptureSuccess) {
                std::cout << "Pattern visible" << std::endl;
            }
            else {
                std::cout << "Pattern not visible" << std::endl;
            }
        }
        // potrebno je sacuvati koordinatu u world-u

        usao_u_catch = false;
        CaptureSuccess = true;
        offset_trial.y_off = 0;
        MoveAbsLIN(home_pos);
    }
}

void Y_Movement_Ensenso(CommMsg home_pos, int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSuccess,
    std::string cameraSerial, std::string tempDir, int i)
{
    static std::vector<cv::Point2f> emptyImagePoints;
    Y_Movement(home_pos, g, offset_trial, limiti, CaptureSuccess, cameraSerial, tempDir, i, {}, emptyImagePoints, 1, {}, {});
}

void Y_Movement_OpenCV(CommMsg home_pos, int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSuccess,
    std::string cameraSerial, std::string tempDir, int i, cv::Size PatternSize, std::vector<cv::Point2f>& imagePoints, cv::Mat image)
{
    Y_Movement(home_pos, g, offset_trial, limiti, CaptureSuccess, cameraSerial, tempDir, i, PatternSize, imagePoints, 2, image, {});
}

void Y_Movement_CharucoCV(CommMsg home_pos, int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSucess,
    std::string cameraSerial, std::string tempDir, int i, cv::Mat image, cv::Size charucoSize)
{
    static std::vector<cv::Point2f> emptyImagePoints;
    Y_Movement(home_pos, g, offset_trial, limiti, CaptureSucess, cameraSerial, tempDir, i, {}, emptyImagePoints, 3, image, charucoSize);
}
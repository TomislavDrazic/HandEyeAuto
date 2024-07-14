#include "Optimization.h"


cv::Mat CalibrateHandEyeCVBoard(std::string rob, std::string cam,std::vector<cv::Mat> RobMatrix_vec, std::vector <cv::Mat> CamMatrix_vec)
{
	cv::Mat R_cam2gripper, t_cam2gripper, cam2gripper, base2board;
	std::vector<cv::Mat> R_gripper2base_vec, t_gripper2base_vec, R_target2cam_vec, t_target2cam_vec;
	readMatrixFromFile(rob, RobMatrix_vec);
	readMatrixFromFile(cam, CamMatrix_vec);
	splitMatrices(CamMatrix_vec, R_target2cam_vec, t_target2cam_vec);
	splitMatrices(RobMatrix_vec, R_gripper2base_vec, t_gripper2base_vec);
	cv::calibrateHandEye(R_gripper2base_vec, t_gripper2base_vec, R_target2cam_vec, t_target2cam_vec, R_cam2gripper, t_cam2gripper, cv::CALIB_HAND_EYE_TSAI);
	cam2gripper = createTransformationMatrix(R_cam2gripper, t_cam2gripper);
	std::cout << "Cam2gripper: " << cam2gripper <<std::endl;
	cv::Vec3d euler = rotationMatrixToEulerAngles(R_cam2gripper);
	std::cout << "euler: " << euler * (180/CV_PI)<< std::endl;

	return cam2gripper;

}



int readMatrixFromFile(std::string filePath, std::vector<cv::Mat>& Matrix_vec)
{
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Unable to open file " << filePath << std::endl;
		return -1;
	}
	std::string line;
	int matrix_line = 0;
	cv::Mat mat = cv::Mat::zeros(4, 4, CV_64F);

	while (std::getline(file, line)) {

		if (!line.empty() && line.front() == '[') {
			line.erase(0, 1);
		}

		if (!line.empty() && line.back() == ']') {
			line.pop_back();
		}

		if (!line.empty() && line.back() == ';') {
			line.pop_back();
		}

		std::stringstream ss(line);
		std::string value;
		std::vector<double> values;
		while (std::getline(ss, value, ',')) {
			values.push_back(std::stod(value));
		}
		if (values.size() != 4) {
			std::cerr << "The first row does not contain exactly 4 values." << std::endl;
			return -1;
		}

		for (size_t i = 0; i < values.size(); ++i) {
			mat.at<double>(matrix_line, i) = values[i];
		}
		matrix_line++;
		if (matrix_line == 4)
		{
			Matrix_vec.push_back(mat.clone());
			matrix_line = 0;
		}
	}

	file.close();
	return 0;
}

int splitMatrices(std::vector<cv::Mat>Matrix_vec, std::vector<cv::Mat>& R_Matrix_vec, std::vector<cv::Mat>& t_Matrix_vec)
{
	cv::Mat R_temp, t_temp;
	cv::Mat R_mat, t_mat;
	cv::Rect R_roi(0, 0, 3, 3);
	cv::Rect t_roi(3, 0, 1, 3);
	for (const auto& mat : Matrix_vec) {
		R_temp = mat(R_roi).clone();
		t_temp = mat(t_roi).clone();
		R_Matrix_vec.push_back(R_temp);
		t_Matrix_vec.push_back(t_temp);
	}
	return 0;
}

double euclideanDistance(const cv::Mat& estimated_cam, const cv::Mat& real_cam) {

	cv::Mat point1 = estimated_cam(cv::Rect(3, 1, 1, 3)).clone();
	cv::Mat point2 = real_cam(cv::Rect(3, 1, 1, 3)).clone();
	if (point1.size() != point2.size() || point1.type() != point2.type()) {
		std::cerr << "Error: The points must be of the same size and type." << std::endl;
		return -1.0;
	}

	cv::Mat diff;
	cv::absdiff(point1, point2, diff);
	diff = diff.mul(diff);
	double sum = cv::sum(diff)[0];
	return std::sqrt(sum);
}

void ValidateTranslation(std::string cameraSerial,std::string tempDir, cv::Size charucoSize, cv::Mat& cam2gripper,cv::Size patternSize, std::vector<cv::Point2f> imagePoints,cv::Mat image)
{
	double greskaX = 100, greskaY = 100, greskaZ = 100;
	double visina_start = 0, visina_end = 0;
	cv::Mat TranslationMatrix = cv::Mat::eye(4, 4, CV_64F); 
	cv::Mat RotationX; 
	cv::Mat RotationY;
	cv::Mat error = cv::Mat::eye(4, 4, CV_64F);
	MoveHome(inital_position);
	cv::Vec3d eulerHome((comm_msg.a) * (CV_PI / 180.0), (comm_msg.b) * (CV_PI / 180.0), (comm_msg.c) * (CV_PI / 180.0));
	cv::Mat R_gripper2baseHome = EulerToMatrix(eulerHome);
	cv::Mat t_gripper2baseHome = (cv::Mat_<double>(3, 1) << comm_msg.x / 1000, comm_msg.y / 1000, comm_msg.z / 1000);
	cv::Mat gripper2baseHome = createTransformationMatrix(R_gripper2baseHome, t_gripper2baseHome);
	cv::Mat markerImage;
	cv::Mat t_target2cam, R_target2cam;
	int cx, cy;
	cv::Mat startPos, tempPos1, tempPos2;;
	std::vector<std::vector<cv::Point2f>> wantedCorners;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	if (user_input == 3)
	{
		bool boardFound = FindPatternFunctionCharucoCV(cameraSerial, tempDir, 0, charucoSize, markerImage);
		if (boardFound) {
			detectArucoMarker(markerImage, markerLength, 8, wantedCorners, R_target2cam, t_target2cam, cx, cy);
			startPos = getStereoPoint(cameraSerial, cx, cy);
		}
	}
	else if (user_input == 2)
	{
		bool boardFound = FindPatternFunctionCV(cameraSerial, tempDir, 0, patternSize, imagePoints, image);
		if (boardFound) {
			OpenCVCorner(patternSize, imagePoints, image, cx, cy);
			startPos = getStereoPoint(cameraSerial, cx, cy);
		}
	}

	std::cout << "startPos: " << startPos << std::endl;
	TranslationMatrix.at<double>(0, 3) = startPos.at<double>(0, 0);
	TranslationMatrix.at<double>(1, 3) = startPos.at<double>(1, 0);
	TranslationMatrix.at<double>(2, 3) = startPos.at<double>(2, 0);
	double rady = 30 * CV_PI / 180.0;
	double radx = 10 * CV_PI / 180.0;
	double multiplierX = 1, multiplierY = 1, multiplierZ = 1;
	double stara_greska_po_z;

	double greska_po_z = 100;
	double iteration = 0;
	bool usao_u_catch_1 = true; bool usao_u_catch_2 = true;
	
	while (abs(greska_po_z) >= 0.0005 && iteration <= 20)
	{
		int y_deg = generateRandomNumber(10, 20);
		std::cout << "y_deg: " << y_deg << std::endl;
		usao_u_catch = false;
		rady = y_deg * CV_PI / 180.0;
		radx = 0 * CV_PI / 180.0;
		MoveTool_H(startPos, cam2gripper,radx,rady,1,gripper2baseHome);
		if (!usao_u_catch)
		{
			if (user_input == 3)
			{
				bool boardFound = FindPatternFunctionCharucoCV(cameraSerial, tempDir, 0, charucoSize, markerImage);
				if (boardFound) {
					detectArucoMarker(markerImage, markerLength, 8, wantedCorners, R_target2cam, t_target2cam, cx, cy);
					tempPos1 = getStereoPoint(cameraSerial, cx, cy);
				}
			}
			else if (user_input == 2)
			{
				bool boardFound = FindPatternFunctionCV(cameraSerial, tempDir, 0, patternSize, imagePoints, image);
				if (boardFound) {
					OpenCVCorner(patternSize, imagePoints, image, cx, cy);
					tempPos1 = getStereoPoint(cameraSerial, cx, cy);
				}
			}
				
			greskaX = (startPos.at<double>(0, 0)) - tempPos1.at<double>(0, 0);
			greskaY = (startPos.at<double>(1, 0)) - tempPos1.at<double>(1, 0);
			greskaZ = (startPos.at<double>(2, 0)) - tempPos1.at<double>(2, 0);
			std::cout << "greskaX: " << greskaX * 1000 << " mm" << std::endl;
			std::cout << "greskaY: " << greskaY  * 1000<< " mm" << std::endl;
			std::cout << "greskaZ: " << greskaZ  * 1000<< " mm" << std::endl;
			usao_u_catch_1 = false;
		}
		else
		{
			std::cout << "Robot ne moze doci u tu poziciju X" << std::endl;
			usao_u_catch_1 = true;
		}

		rady = -y_deg * CV_PI / 180.0;
		radx = 0 * CV_PI / 180.0;
		MoveTool_H(startPos, cam2gripper, radx, rady, 1, gripper2baseHome);
		if (!usao_u_catch)
		{
			if (user_input == 3)
			{
				bool boardFound = FindPatternFunctionCharucoCV(cameraSerial, tempDir, 0, charucoSize, markerImage);
				if (boardFound) {
					detectArucoMarker(markerImage, markerLength, 8, wantedCorners, R_target2cam, t_target2cam, cx, cy);
					tempPos2 = getStereoPoint(cameraSerial, cx, cy);
				}
			}
			else if (user_input == 2)
			{
				bool boardFound = FindPatternFunctionCV(cameraSerial, tempDir, 0, patternSize, imagePoints, image);
				if (boardFound) {
					OpenCVCorner(patternSize, imagePoints, image, cx, cy);
					tempPos2 = getStereoPoint(cameraSerial, cx, cy);
				}
			}

			greskaX = (startPos.at<double>(0, 0)) - tempPos2.at<double>(0, 0);
			greskaY = (startPos.at<double>(1, 0)) - tempPos2.at<double>(1, 0);
			greskaZ = (startPos.at<double>(2, 0)) - tempPos2.at<double>(2, 0);
			usao_u_catch_2 = false;
		}
		else
		{
			std::cout << "Robot ne moze doci u tu poziciju X" << std::endl;
			usao_u_catch_2 = true;
		}
		stara_greska_po_z = greska_po_z;
		greska_po_z = tempPos1.at<double>(2,0) - tempPos2.at<double>(2,0);
		if (predznakChange(stara_greska_po_z, greska_po_z))
		{
			multiplierX = multiplierX * 2;
		}
		if (!usao_u_catch_1 && !usao_u_catch_2)
		{
			std::cout << "greska izmedu dvije rotacije: " << greska_po_z * 1000 << " mm" << std::endl;
			cam2gripper.at<double>(0, 3) += greska_po_z / multiplierX;
			std::cout << "new_x: " << cam2gripper.at<double>(0, 3) << std::endl;
			iteration++;
		}
	}

	iteration = 0;
	greska_po_z = 100;
	usao_u_catch_1 = true; usao_u_catch_2 = true;

	while (abs(greska_po_z) >= 0.0005 && iteration <= 20)
	{
		int x_deg = generateRandomNumber(10, 20);
		std::cout << "x_deg: " << x_deg << std::endl;
		usao_u_catch = false;
		rady = 0 * CV_PI / 180.0;
		radx = x_deg * CV_PI / 180.0;
		MoveTool_H(startPos, cam2gripper, radx, rady, 1, gripper2baseHome);
		if (!usao_u_catch)
		{
			if (user_input == 3)
			{
				bool boardFound = FindPatternFunctionCharucoCV(cameraSerial, tempDir, 0, charucoSize, markerImage);
				if (boardFound) {
					detectArucoMarker(markerImage, markerLength, 8, wantedCorners, R_target2cam, t_target2cam, cx, cy);
					tempPos1 = getStereoPoint(cameraSerial, cx, cy);
				}
			}
			else if (user_input == 2)
			{
				bool boardFound = FindPatternFunctionCV(cameraSerial, tempDir, 0, patternSize, imagePoints, image);
				if (boardFound) {
					OpenCVCorner(patternSize, imagePoints, image, cx, cy);
					tempPos1 = getStereoPoint(cameraSerial, cx, cy);
				}
			}

			greskaX = (startPos.at<double>(0, 0)) - tempPos1.at<double>(0, 0);
			greskaY = (startPos.at<double>(1, 0)) - tempPos1.at<double>(1, 0);
			greskaZ = (startPos.at<double>(2, 0)) - tempPos1.at<double>(2, 0);
			std::cout << "greskaX: " << greskaX * 1000 << " mm" << std::endl;
			std::cout << "greskaY: " << greskaY * 1000 << " mm" << std::endl;
			std::cout << "greskaZ: " << greskaZ * 1000 << " mm" << std::endl;
			usao_u_catch_1 = false;
		}
		else
		{
			std::cout << "Robot ne moze doci u tu poziciju Y" << std::endl;
			usao_u_catch_1 = true;
		}

		rady = 0 * CV_PI / 180.0;
		radx = -x_deg * CV_PI / 180.0;
		MoveTool_H(startPos, cam2gripper, radx, rady, 1, gripper2baseHome);
		if (!usao_u_catch)
		{
			if (user_input == 3)
			{
				bool boardFound = FindPatternFunctionCharucoCV(cameraSerial, tempDir, 0, charucoSize, markerImage);
				if (boardFound) {
					detectArucoMarker(markerImage, markerLength, 8, wantedCorners, R_target2cam, t_target2cam, cx, cy);
					tempPos2 = getStereoPoint(cameraSerial, cx, cy);
				}
			}
			else if (user_input == 2)
			{
				bool boardFound = FindPatternFunctionCV(cameraSerial, tempDir, 0, patternSize, imagePoints, image);
				if (boardFound) {
					OpenCVCorner(patternSize, imagePoints, image, cx, cy);
					tempPos2 = getStereoPoint(cameraSerial, cx, cy);
				}
			}

			greskaX = (startPos.at<double>(0, 0)) - tempPos2.at<double>(0, 0);
			greskaY = (startPos.at<double>(1, 0)) - tempPos2.at<double>(1, 0);
			greskaZ = (startPos.at<double>(2, 0)) - tempPos2.at<double>(2, 0);
			usao_u_catch_2 = false;
		}
		else
		{
			std::cout << "Robot ne moze doci u tu poziciju Y" << std::endl;
			usao_u_catch_2 = true;
		}
		stara_greska_po_z = greska_po_z;
		greska_po_z = tempPos1.at<double>(2, 0) - tempPos2.at<double>(2, 0);
		if (predznakChange(stara_greska_po_z, greska_po_z))
		{
			multiplierX = multiplierX * 2;
		}
		if (!usao_u_catch_1 && !usao_u_catch_2)
		{
			std::cout << "greska izmedu dvije rotacije: " << greska_po_z * 1000 << " mm" << std::endl;
			cam2gripper.at<double>(1, 3) += greska_po_z / multiplierX;
			std::cout << "new_y: " << cam2gripper.at<double>(1, 3) << std::endl;
			iteration++;
		}
	}

	iteration = 0;
	while (abs(greskaZ) >= 0.0005 && iteration <= 20)
	{
		MoveTool_H(startPos, cam2gripper, radx, rady, 1, gripper2baseHome);
		if (!usao_u_catch)
		{
			if (user_input == 3)
			{
				bool boardFound = FindPatternFunctionCharucoCV(cameraSerial, tempDir, 0, charucoSize, markerImage);
				if (boardFound) {
					detectArucoMarker(markerImage, markerLength, 8, wantedCorners, R_target2cam, t_target2cam, cx, cy);
					tempPos1 = getStereoPoint(cameraSerial, cx, cy);
				}
			}
			else if (user_input == 2)
			{
				bool boardFound = FindPatternFunctionCV(cameraSerial, tempDir, 0, patternSize, imagePoints, image);
				if (boardFound) {
					OpenCVCorner(patternSize, imagePoints, image, cx, cy);
					tempPos1 = getStereoPoint(cameraSerial, cx, cy);
				}
			}
			greskaX = (startPos.at<double>(0, 0)) - tempPos1.at<double>(0, 0);
			greskaY = (startPos.at<double>(1, 0)) - tempPos1.at<double>(1, 0);
			greskaZ = (startPos.at<double>(2, 0)) - tempPos1.at<double>(2, 0);
			std::cout << "greskaX: " << greskaX * 1000 <<" mm" << std::endl;
			std::cout << "greskaY: " << greskaY * 1000<<" mm" << std::endl;
			std::cout << "greskaZ: " << greskaZ  * 1000<<" mm" << std::endl;
			cam2gripper.at<double>(2, 3) += greskaZ;
			std::cout << "cam2gripper_x: " << cam2gripper.at<double>(0, 3) << std::endl;
			std::cout << "cam2gripper_x: " << cam2gripper.at<double>(1, 3) << std::endl;
			std::cout << "cam2gripper_x: " << cam2gripper.at<double>(2, 3) << std::endl;
			iteration++;
		}
	}
	std::cout << "New cam2gripper: " << cam2gripper << std::endl;

}


void ValidateRotation(std::string cameraSerial, std::string tempDir, cv::Size charucoSize, cv::Mat& cam2gripper, cv::Size patternSize, std::vector<cv::Point2f> imagePoints, cv::Mat image)
{
	double greska_x = 100, greska_y = 100;
	double greska_rad_x = 100, greska_rad_y = 100;
	double visina_start = 0, visina_end = 0;

	MoveHome(inital_position);
	cv::Vec3d eulerHome((comm_msg.a) * (CV_PI / 180.0), (comm_msg.b) * (CV_PI / 180.0), (comm_msg.c) * (CV_PI / 180.0));
	cv::Mat R_gripper2baseHome = EulerToMatrix(eulerHome);
	cv::Mat t_gripper2baseHome = (cv::Mat_<double>(3, 1) << comm_msg.x / 1000, comm_msg.y / 1000, comm_msg.z / 1000);
	cv::Mat gripper2baseHome = createTransformationMatrix(R_gripper2baseHome, t_gripper2baseHome);
	cv::Mat markerImage;
	cv::Mat t_target2cam, R_target2cam;
	cv::Mat startPos, tempPos;
	int cx, cy;
	double multiplier_x = 1, multiplier_y = 1;
	std::vector<std::vector<cv::Point2f>> wantedCorners;
	bool boardFound = false;
	if (user_input == 3)
	{
		boardFound = FindPatternFunctionCharucoCV(cameraSerial, tempDir, 0, charucoSize, markerImage);
		if (boardFound) {
			detectArucoMarker(markerImage, markerLength, 8, wantedCorners, R_target2cam, t_target2cam, cx, cy);
			startPos = getStereoPoint(cameraSerial, cx, cy);
		}
	}
	else if (user_input == 2)
	{
		boardFound = FindPatternFunctionCV(cameraSerial, tempDir, 0, patternSize, imagePoints, image);
		if (boardFound) {
			OpenCVCorner(patternSize, imagePoints, image, cx, cy);
			startPos = getStereoPoint(cameraSerial, cx, cy);
		}
	}
	std::cout << "startPos: " << startPos << std::endl;
	double offset = 0.1;
	boardFound = true;
	while (boardFound && !usao_u_catch)
	{
		offset = offset + 0.01;
		MoveToolRotValid_H(cam2gripper, offset, gripper2baseHome);
		if (user_input == 3)
			boardFound = FindPatternFunctionCharucoCV(cameraSerial, tempDir, 0, charucoSize, markerImage);
		else if (user_input == 2) {
			boardFound = FindPatternFunctionCV(cameraSerial, tempDir, 0, patternSize, imagePoints, image);
		}
	}
	offset = offset - 0.01;

	int fx = 0, fy = 0;
	while (fx <= 6 || fy <= 6)
	{
		usao_u_catch = false;
		MoveToolRotValid_H(cam2gripper, offset, gripper2baseHome);
		if (!usao_u_catch)
		{
			if (user_input == 3)
			{
				boardFound = FindPatternFunctionCharucoCV(cameraSerial, tempDir, 0, charucoSize, markerImage);
				if (boardFound) {
					detectArucoMarker(markerImage, markerLength, 8, wantedCorners, R_target2cam, t_target2cam, cx, cy);
					tempPos = getStereoPoint(cameraSerial, cx, cy);
				}
			}
			else if (user_input == 2)
			{
				boardFound = FindPatternFunctionCV(cameraSerial, tempDir, 0, patternSize, imagePoints, image);
				if (boardFound) {
					OpenCVCorner(patternSize, imagePoints, image, cx, cy);
					tempPos = getStereoPoint(cameraSerial, cx, cy);
				}
			}
			else
			{
				std::cout << "board not found " << std::endl;
				MoveHome(inital_position);
			}
			std::cout << "tempPos: " << tempPos << std::endl;
			double stara_greska_x = greska_x;
			double stara_greska_y = greska_y;
			greska_x = startPos.at<double>(0, 0) - tempPos.at<double>(0, 0);
			greska_y = startPos.at<double>(1, 0) - tempPos.at<double>(1, 0);
			if (predznakChange(stara_greska_x, greska_x))
			{
				multiplier_x = multiplier_x * 2;
				fx++;
			}
			if (predznakChange(stara_greska_y, greska_y))
			{
				multiplier_y = multiplier_y * 2;
				fy++;
			}
			greska_rad_x = std::asin(-4 * greska_x / (multiplier_x * tempPos.at<double>(2, 0)));
			greska_rad_y = std::asin(4 * greska_y / (multiplier_y * tempPos.at<double>(2, 0)));
			std::cout << "greska_x: " << greska_x <<" mm" << std::endl;
			std::cout << "greska_y: " << greska_y <<" mm" << std::endl;
			double cy = std::cos(greska_rad_x); double sy = std::sin(greska_rad_x);
			double cx = std::cos(greska_rad_y); double sx = std::sin(greska_rad_y);
			cv::Mat rotationY = (cv::Mat_<double>(4, 4) <<
				cy, 0, sy, 0,
				0, 1, 0, 0,
				-sy, 0, cy, 0,
				0, 0, 0, 1);
			cv::Mat rotationX = (cv::Mat_<double>(4, 4) <<
				1, 0, 0, 0,
				0, cx, -sx, 0,
				0, sx, cx, 0,
				0, 0, 0, 1);
			cam2gripper = cam2gripper * rotationY * rotationX;
			cv::Mat subMatrix = cam2gripper(cv::Range(0, 3), cv::Range(0, 3));
			cv::Vec3d euler = rotationMatrixToEulerAngles(subMatrix);
			std::cout << "euler:" << euler * (180 / CV_PI) <<" deg" << std::endl;
		}
		else
		{
			MoveHome(inital_position);
		}
	}
	std::cout << "New cam2gripper: " << cam2gripper << std::endl;
}
bool RotateAroundPoint(std::string cameraSerial, std::string tempDir, cv::Size charucoSize, double radx, double rady,
	cv::Mat EstimatedCam2gripper)
{
	bool found = false;
	cv::Mat error = cv::Mat::eye(4, 4, CV_64F);
	cv::Vec3d eulerHome((comm_msg.a) * (CV_PI / 180.0), (comm_msg.b) * (CV_PI / 180.0), (comm_msg.c) * (CV_PI / 180.0));
	cv::Mat R_gripper2baseHome = EulerToMatrix(eulerHome);
	cv::Mat t_gripper2baseHome = (cv::Mat_<double>(3, 1) << comm_msg.x / 1000, comm_msg.y / 1000, comm_msg.z / 1000);
	cv::Mat gripper2baseHome = createTransformationMatrix(R_gripper2baseHome, t_gripper2baseHome);
	cv::Mat markerImage;
	cv::Mat t_target2cam, R_target2cam;
	int cx, cy;
	cv::Mat startPos, tempPos1, tempPos2;;
	std::vector<std::vector<cv::Point2f>> wantedCorners;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	bool boardFound = FindPatternFunctionCharucoCV(cameraSerial, tempDir, 0, charucoSize, markerImage);
	if (boardFound) {
		detectArucoMarkerNoImg(markerImage, markerLength, 8, wantedCorners, R_target2cam, t_target2cam, cx, cy);
		startPos = getStereoPoint(cameraSerial, cx, cy);
		found = true;
	}
	else
	{
		std::cout << "board not found" << std::endl;
		found = false;
	}
	radx = radx * (CV_PI / 180);
	rady = rady * (CV_PI / 180);
	if (found)
	{
		MoveTool_H(startPos, EstimatedCam2gripper, radx, rady, 1, gripper2baseHome);
	}
	return found;
}


bool RotateAroundPointCV(std::string cameraSerial, std::string tempDir, cv::Size patternSize,std::vector<cv::Point2f> imagePoints, cv::Mat image, double radx, double rady, 
	cv::Mat EstimatedCam2gripper)
{
	bool found = false;
	cv::Mat error = cv::Mat::eye(4, 4, CV_64F);
	cv::Vec3d eulerHome((comm_msg.a) * (CV_PI / 180.0), (comm_msg.b) * (CV_PI / 180.0), (comm_msg.c) * (CV_PI / 180.0));
	cv::Mat R_gripper2baseHome = EulerToMatrix(eulerHome);
	cv::Mat t_gripper2baseHome = (cv::Mat_<double>(3, 1) << comm_msg.x / 1000, comm_msg.y / 1000, comm_msg.z / 1000);
	cv::Mat gripper2baseHome = createTransformationMatrix(R_gripper2baseHome, t_gripper2baseHome);
	cv::Mat markerImage;
	cv::Mat t_target2cam, R_target2cam;
	int cx, cy;
	cv::Mat startPos, tempPos1, tempPos2;;
	std::vector<std::vector<cv::Point2f>> wantedCorners;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	bool boardFound = FindPatternFunctionCV(cameraSerial, tempDir, 0, patternSize, imagePoints, image);
	if (boardFound) {
		OpenCVCorner(patternSize, imagePoints, image,cx,cy);
		startPos = getStereoPoint(cameraSerial, cx, cy);
		found = true;
	}
	else
	{
		std::cout << "board not found" << std::endl;
		found = false;
	}
	radx = radx * (CV_PI / 180);
	rady = rady * (CV_PI / 180);
	if (found)
	{
		MoveTool_H(startPos, EstimatedCam2gripper, radx, rady, 1, gripper2baseHome);
	}
	return found;
}


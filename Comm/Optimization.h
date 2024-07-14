#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <vector>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include "Utils.h"
#include "Camera_functions.h"
#include "Calib_method.h"
#include "Robot_server.h"
#include "OpenCV.h"

int readMatrixFromFile(std::string filePath, std::vector<cv::Mat>& Matrix_vec);

int splitMatrices(std::vector<cv::Mat>Matrix_vec, std::vector<cv::Mat>& R_Matrix_vec, std::vector<cv::Mat>& t_Matrix_vec);

double euclideanDistance(const cv::Mat& estimated_cam, const cv::Mat& real_cam);

cv::Mat CalibrateHandEyeCVBoard(std::string rob, std::string cam, std::vector<cv::Mat> RobMatrix_vec, std::vector <cv::Mat> CamMatrix_vec);


void ValidateTranslation(std::string cameraSerial, std::string tempDir, cv::Size charucoSize, cv::Mat& cam2gripper, cv::Size patternSize, std::vector<cv::Point2f> imagePoints, cv::Mat image);

void ValidateRotation(std::string cameraSerial, std::string tempDir, cv::Size charucoSize, cv::Mat& cam2gripper, cv::Size patternSize, std::vector<cv::Point2f> imagePoints, cv::Mat image);

bool RotateAroundPoint(std::string cameraSerial, std::string tempDir, cv::Size charucoSize, double radx, double rady,
	cv::Mat EstimatedCam2gripper);

bool RotateAroundPointCV(std::string cameraSerial, std::string tempDir, cv::Size patternSize, std::vector<cv::Point2f> imagePoints, cv::Mat image, double radx, double rady,
	cv::Mat EstimatedCam2gripper);

#endif 


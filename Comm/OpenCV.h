#ifndef OPENCV_H
#define OPENCV_H

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp> 
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream> 
#include <iomanip>
#include <tuple>
#include "C:/Users/crta/Desktop/Tomislav/rapidjson-master/include/rapidjson/document.h"
#include "C:/Users/crta/Desktop/Tomislav/rapidjson-master/include/rapidjson/istreamwrapper.h"
#include "C:/Users/crta/Desktop/Tomislav/rapidjson-master/include/rapidjson/error/en.h"
#include "camera_functions.h"
#include <opencv2/aruco/charuco.hpp>
#include "Utils.h"

bool FindChessboardCV(cv::Size pattern_size, cv::Mat image, std::vector<cv::Point2f>& imagePoints);

bool FindChessboardCharucoCV(cv::Mat gray, cv::Size charucoSize);

cv::Mat ReadImage(std::string image_path);

void SolvePnP(cv::Size patternSize, std::vector<cv::Point2f> imagePoints, cv::Mat cameraMatrix, cv::Mat distortionCoeffs, cv::Mat& image, cv::Mat& eulerAngles_mat, cv::Mat& tvec);

void OpenCVCorner(cv::Size patternSize, std::vector<cv::Point2f> imagePoints, cv::Mat& image, int& cx, int& cy);

void EstimatePoseCharucoBoard(cv::Mat gray, cv::Size charucoSize, cv::Mat& R_target2cam, cv::Mat& T_target2cam);

void detectArucoMarker(cv::Mat gray, float makerLength, int wantedMarker, std::vector<std::vector<cv::Point2f>>& wantedCorners,
    cv::Mat& R_target2cam, cv::Mat& T_target2cam, int& cx, int& cy);

void detectArucoMarkerNoImg(cv::Mat gray, float makerLength, int wantedMarker, std::vector<std::vector<cv::Point2f>>& wantedCorners,
    cv::Mat& R_target2cam, cv::Mat& T_target2cam, int& cx, int& cy);

cv::Mat Crosses(cv::Mat& image, std::vector<std::vector<cv::Point2f>>& wantedCorners);

std::tuple<cv::Mat, cv::Mat, double, cv::Vec3d, cv::Vec3d> readCameraParameters(const std::string& jsonFilePath);

bool FindPatternFunctionCV(std::string camera, std::string TempDir, int i, cv::Size PatternSize, std::vector<cv::Point2f>& imagePoints, cv::Mat& gray);

bool FindPatternFunctionCharucoCV(std::string camera, std::string tempDir, int i, cv::Size CharucoSize, cv::Mat& gray);

#endif 

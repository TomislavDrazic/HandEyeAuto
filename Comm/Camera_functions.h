#ifndef CAMERA_FUNCTIONS_H
#define CAMERA_FUNCTIONS_H

#include "nxLib.h"
#include "C:/Program Files/Ensenso/development/examples/C++/common/example.h"
#include <iomanip>
#include <iostream>
#include <fstream> 
#include <string>
#include <cstdio>

#include <thread> // For std::this_thread::sleep_for
#include <chrono> // For std::chrono::milliseconds

#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp> 
#include <opencv2/core/quaternion.hpp> 
#include "Struct.h"
#include "Utils.h"
#include "OpenCV.h"
#include "Global_vars.h"

std::string StartCamera();

bool CaptureImages(std::string cam_name, bool decodedData);

void CaptureTempImages(std::string cam_name);

void SaveImages(int i, std::string cam_name);

std::string SaveTempImages(int i, std::string cam_name, std::string newDir);

void EnsensoCalibrate(std::string cam_name, cv::Mat poseMatrix);

void WriteParamFile(std::string cam_name);

void CloseCamera();

void InitizalizePatternCapture();

XYZ getTraingulatedPoint(std::string cam_name, int pixelX, int pixelY);

cv::Mat CameraCoordinates(XYZ point);

cv::Mat getStereoPoint(std::string camera_name, int pixelX, int pixelY);

struct Pose {
    cv::Point3d position; // x, y, z coordinates
    cv::Vec4d quaternion; // w, x, y, z components of the quaternion
};

#endif 
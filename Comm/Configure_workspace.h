#include "Struct.h"
#include "Robot_server.h"
#include <thread>
#include <vector>
#include "OpenCV.h"
#include "camera_functions.h"

#ifndef CONFIGURE_WORKSPACE_H
#define CONFIGRUE_WORKSPACE_H

void X_Movement(CommMsg home_pos, int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSuccess,
    std::string cameraSerial, std::string tempDir, int i, cv::Size PatternSize, std::vector<cv::Point2f>& imagePoints, int UserInput, cv::Mat image,
    cv::Size charucoSize);

void X_Movement_Ensenso(CommMsg home_pos, int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSuccess,
    std::string cameraSerial, std::string tempDir, int i);

void X_Movement_OpenCV(CommMsg home_pos, int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSuccess,
    std::string cameraSerial, std::string tempDir, int i, cv::Size PatternSize, std::vector<cv::Point2f>& imagePoints, cv::Mat image);


void X_Movement_CharucoCV(CommMsg home_pos, int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSucess,
    std::string cameraSerial, std::string tempDir, int i, cv::Mat image, cv::Size charucoSize);

void Y_Movement(CommMsg home_pos, int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSuccess,
    std::string cameraSerial, std::string tempDir, int i, cv::Size PatternSize, std::vector<cv::Point2f>& imagePoints, int UserInput, cv::Mat image,
    cv::Size charucoSize);

void Y_Movement_Ensenso(CommMsg home_pos, int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSuccess,
    std::string cameraSerial, std::string tempDir, int i);

void Y_Movement_OpenCV(CommMsg home_pos, int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSuccess,
    std::string cameraSerial, std::string tempDir, int i, cv::Size PatternSize, std::vector<cv::Point2f>& imagePoints, cv::Mat image);

void Y_Movement_CharucoCV(CommMsg home_pos, int g, Offset offset_trial, std::vector<Limit>& limiti, bool& CaptureSucess,
    std::string cameraSerial, std::string tempDir, int i, cv::Mat image, cv::Size charucoSize);

#endif#pragma once

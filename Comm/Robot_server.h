#ifndef ROBOT_SERVER_H
#define ROBOT_SERVER_H

#include <iostream>
#include <fstream> 
#include <thread>
#include <chrono>
#include <winsock2.h>
#include <string>

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp> 
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"
#include <filesystem>

#include "Struct.h"
#include "Global_vars.h"
#include "Utils.h"

void runServer();

std::string createJsonString(CommMsg poruka);

CommMsg GetCoords(SOCKET clientSocket);

void RobotSleep();

void MovePTP(Offset offset);

void MoveLIN(Offset offset);

void MoveHome(CommMsg inital_pos);

void MoveAbsLIN(CommMsg comm_msg);

void MoveAbsPTP(Offset offset);

void MoveAbsNoRotLIN(double x, double y, double z);

void MoveTool_H(cv::Mat startPos, cv::Mat cam2gripper, double radx, double rady, int axis, cv::Mat gripper2base);

void MoveToolRotValid_H(cv::Mat cam2gripper, double odstupanje, cv::Mat gripper2base);

void ToolRotWorldPos(Offset offset);

#endif

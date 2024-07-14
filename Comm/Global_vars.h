
#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include "Struct.h" 
#include <atomic>
#include "string"
#include "opencv2/opencv.hpp"

extern CommMsg poruka;
extern CommMsg comm_msg;
extern CommMsg inital_position;
extern bool robot_at_pos;
extern bool usao_u_catch;
extern std::atomic<bool> serverRunning;
extern std::tuple<cv::Mat, cv::Mat, double, cv::Vec3d, cv::Vec3d > cameraParams;
extern float squareLength;
extern float markerLength;
extern double square_size;
extern cv::Mat cameraMatrix;
extern cv::Mat distortionCoeffs;
extern int step_x;
extern int step_y;
extern int step_z;
extern int estimated_it;
extern int it;
extern int rotation_range;
extern int user_input;

#endif 

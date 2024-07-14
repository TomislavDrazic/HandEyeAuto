#include "global_vars.h"

CommMsg poruka = { 0, 0, 0, 0, 0, 0, 1 };
CommMsg comm_msg = { 0, 0, 0, 0, 0, 0, 1 };
CommMsg inital_position;
bool robot_at_pos;
bool usao_u_catch;
std::atomic<bool> serverRunning(false);
std::tuple<cv::Mat, cv::Mat, double, cv::Vec3d, cv::Vec3d > cameraParams;
float squareLength;
float markerLength;
double square_size;
cv::Mat cameraMatrix;
cv::Mat distortionCoeffs;
int step_x;
int step_y;
int step_z;
int estimated_it;
int it;
int rotation_range;
int user_input;
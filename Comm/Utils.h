#ifndef UTILS_H
#define UTILS_H

#include <Eigen/Dense>
#include <stdexcept>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <random>
#include <fstream>
#include <iostream>

Eigen::Matrix3d skew(const Eigen::Vector3d& v);

Eigen::Matrix3d logMatrix(const Eigen::Matrix3d& A);

Eigen::MatrixXd kroneckerProduct(const Eigen::MatrixXd& A, const Eigen::MatrixXd& B);

cv::Mat EulerToMatrix(cv::Vec3d eulerXYZ);

cv::Vec3d MatrixToEuler(cv::Mat rotationMatrix);

cv::Mat createTransformationMatrix(const cv::Mat& rotationMatrix, const cv::Mat& translationVector);

std::vector<Eigen::MatrixXd> convertToEigen(const std::vector<cv::Mat>& inputVec);

cv::Mat angleAxisToMatrix(double theta, cv::Vec3d u);

void createMatrixH_(std::vector<Eigen::MatrixXd>EigenRob, std::vector<Eigen::MatrixXd>EigenCam,
    std::vector<Eigen::MatrixXd>& Vec_Hgij, std::vector<Eigen::MatrixXd>& Vec_Hcij);

int generateRandomNumber(int lower, int upper);

int UserInput();

int ProjectPart();

cv::Vec3d rotationMatrixToEulerAngles(const cv::Mat& R);

cv::Vec4d EulerToAngleAxis(cv::Vec3d eulerXYZ);

bool predznakChange(double stara_vrj, double nova_vrj);

void clearFile(std::ofstream& file, std::string file_path);

#endif

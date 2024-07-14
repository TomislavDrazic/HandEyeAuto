#ifndef TSAI_CALIBRATION_H
#define TSAI_CALIBRATION_H

#include <vector>
#include <Eigen/Dense>
#include <sstream>
#include <iostream>
#include <cmath>
#include "Utils.h"

Eigen::Matrix4d tsai(const std::vector<Eigen::MatrixXd>& A, const std::vector<Eigen::MatrixXd>& B);

Eigen::Matrix4d andreff(const std::vector<Eigen::MatrixXd>& AA, const std::vector<Eigen::MatrixXd>& BB);


#endif

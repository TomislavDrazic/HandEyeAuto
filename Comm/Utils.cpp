#include "Utils.h"

Eigen::Matrix3d skew(const Eigen::Vector3d& v) {
    Eigen::Matrix3d S;
    S << 0, -v.z(), v.y(),
        v.z(), 0, -v.x(),
        -v.y(), v.x(), 0;
    return S;
}

Eigen::Matrix3d logMatrix(const Eigen::Matrix3d& A) {
    if (!A.isApprox(A.transpose().inverse(), 1e-8)) {
        throw std::invalid_argument("Input matrix is not a valid rotation matrix");
    }

    double theta = acos((A.trace() - 1) / 2.0);
    if (std::abs(theta) < 1e-8) {
        return Eigen::Matrix3d::Zero();
    }

    double sinTheta = sin(theta);
    if (std::abs(sinTheta) < 1e-8) {
        throw std::runtime_error("Numerical instability due to small sin(theta)");
    }

    Eigen::Matrix3d logA = theta / (2 * sinTheta) * (A - A.transpose());
    return logA;
}

Eigen::MatrixXd kroneckerProduct(const Eigen::MatrixXd& A, const Eigen::MatrixXd& B) {
    int rowsA = A.rows();
    int colsA = A.cols();
    int rowsB = B.rows();
    int colsB = B.cols();
    Eigen::MatrixXd result(rowsA * rowsB, colsA * colsB);

    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsA; ++j) {
            result.block(i * rowsB, j * colsB, rowsB, colsB) = A(i, j) * B;
        }
    }
    return result;
}

cv::Mat EulerToMatrix(cv::Vec3d eulerZYX) // ZYX notacija (valjda)
{
    double thetaZ = eulerZYX[0];
    double thetaY = eulerZYX[1];
    double thetaX = eulerZYX[2];


    cv::Mat R_x = (cv::Mat_<double>(3, 3) <<
        1, 0, 0,
        0, cos(thetaX), -sin(thetaX),
        0, sin(thetaX), cos(thetaX));

    cv::Mat R_y = (cv::Mat_<double>(3, 3) <<
        cos(thetaY), 0, sin(thetaY),
        0, 1, 0,
        -sin(thetaY), 0, cos(thetaY));

    cv::Mat R_z = (cv::Mat_<double>(3, 3) <<
        cos(thetaZ), -sin(thetaZ), 0,
        sin(thetaZ), cos(thetaZ), 0,
        0, 0, 1);
    cv::Mat rotationMatrix = R_z * R_y * R_x;
    return rotationMatrix;
}

cv::Vec3d MatrixToEuler(cv::Mat rotationMatrix)
{
    double r11 = rotationMatrix.at<double>(0, 0);
    double r12 = rotationMatrix.at<double>(0, 1);
    double r13 = rotationMatrix.at<double>(0, 2);
    double r21 = rotationMatrix.at<double>(1, 0);
    double r22 = rotationMatrix.at<double>(1, 1);
    double r23 = rotationMatrix.at<double>(1, 2);
    double r31 = rotationMatrix.at<double>(2, 0);
    double r32 = rotationMatrix.at<double>(2, 1);
    double r33 = rotationMatrix.at<double>(2, 2);

    double thetaX = atan2(r32, r33);
    double thetaY = atan2(-r31, sqrt(r32 * r32 + r33 * r33));
    double thetaZ = atan2(r21, r11);
    cv::Vec3d Euler(thetaX, thetaY, thetaZ);

    return Euler;
}


cv::Mat createTransformationMatrix(const cv::Mat& rotationMatrix, const cv::Mat& translationVector) {

    cv::Mat transformationMatrix = cv::Mat::eye(4, 4, rotationMatrix.type());

    rotationMatrix.copyTo(transformationMatrix(cv::Rect(0, 0, 3, 3)));

    translationVector.copyTo(transformationMatrix(cv::Rect(3, 0, 1, 3)));

    return transformationMatrix;
}

std::vector<Eigen::MatrixXd> convertToEigen(const std::vector<cv::Mat>& inputVec) {
    std::vector<Eigen::MatrixXd> outputVec;
    for (const auto& mat : inputVec) {
        Eigen::MatrixXd eigenMat(mat.rows, mat.cols);

        for (int i = 0; i < mat.rows; ++i) {
            for (int j = 0; j < mat.cols; ++j) {
                eigenMat(i, j) = mat.at<double>(i, j); 
            }
        }
        outputVec.push_back(eigenMat);
    }
    return outputVec;
}

cv::Mat angleAxisToMatrix(double theta, cv::Vec3d u) {
    u = u / cv::norm(u);

    cv::Vec3d angle_axis = u * theta;

    cv::Mat rotation_matrix;
    cv::Rodrigues(angle_axis, rotation_matrix);

    return rotation_matrix;
}


void createMatrixH_(std::vector<Eigen::MatrixXd>EigenRob, std::vector<Eigen::MatrixXd>EigenCam,
    std::vector<Eigen::MatrixXd>& Vec_Hgij, std::vector<Eigen::MatrixXd>& Vec_Hcij)
{
    int idx = 0;
    for (size_t i = 0; i < EigenRob.size(); i++)
    {
        for (size_t j = i + 1; j < EigenRob.size(); j++, idx++)
        {
            Eigen::CompleteOrthogonalDecomposition <Eigen::MatrixXd> cod(EigenRob[j]);
            Eigen::MatrixXd Hgij_inv = cod.pseudoInverse();
            Eigen::MatrixXd Hgij = Hgij_inv * EigenRob[i];
            Vec_Hgij.push_back(Hgij);

            Eigen::CompleteOrthogonalDecomposition <Eigen::MatrixXd> cod1(EigenCam[i]);
            Eigen::MatrixXd Hcij_inv = cod1.pseudoInverse();
            Eigen::MatrixXd Hcij = EigenCam[j] * Hcij_inv;
            Vec_Hcij.push_back(Hcij);

        }
    }
}

int generateRandomNumber(int lower, int upper) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(lower, upper);
    return dis(gen);
}

int UserInput()
{
    int number;
    std::cout << "Calibration board, ENSENSO(1), OpenCV(2) ChAruco(3)): \n";
    std::cin >> number;
    return number;
}

int ProjectPart()
{
    int number;
    std::cout << "Full program (0), Calibratin only (1): \n";
    std::cin >> number;
    return number;
}

cv::Vec3d rotationMatrixToEulerAngles(const cv::Mat& R) {
    double sy = sqrt(R.at<double>(0, 0) * R.at<double>(0, 0) + R.at<double>(1, 0) * R.at<double>(1, 0));

    bool singular = sy < 1e-6; // If sy is close to zero, singularity occurs

    double x, y, z;
    if (!singular) {
        x = atan2(R.at<double>(2, 1), R.at<double>(2, 2));
        y = atan2(-R.at<double>(2, 0), sy);
        z = atan2(R.at<double>(1, 0), R.at<double>(0, 0));
    }
    else {
        x = atan2(-R.at<double>(1, 2), R.at<double>(1, 1));
        y = atan2(-R.at<double>(2, 0), sy);
        z = 0;
    }
    return cv::Vec3d(z, y, x);
}


cv::Vec4d EulerToAngleAxis(cv::Vec3d eulerXYZ)
{
    cv::Mat T = EulerToMatrix(eulerXYZ);
    cv::Mat R = T(cv::Rect(0, 0, 3, 3));
    double theta = acos((cv::trace(R)[0] - 1) / 2);
    double s = 2 * sin(theta);
    cv::Vec3d axis;
    axis[0] = (R.at<double>(2, 1) - R.at<double>(1, 2)) / s;
    axis[1] = (R.at<double>(0, 2) - R.at<double>(2, 0)) / s;
    axis[2] = (R.at<double>(1, 0) - R.at<double>(0, 1)) / s;

    return cv::Vec4d(theta, axis[0], axis[1], axis[2]);
}

bool predznakChange(double stara_vrj, double nova_vrj)
{
    if (stara_vrj >= 0)
    {
        if (nova_vrj > 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        if (nova_vrj <= 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

void clearFile(std::ofstream& file, std::string file_path) {
    file.close();
    file.open(file_path, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
    }
}

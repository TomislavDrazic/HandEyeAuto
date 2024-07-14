#include "Calib_method.h"


Eigen::Matrix4d tsai(const std::vector<Eigen::MatrixXd>& A, const std::vector<Eigen::MatrixXd>& B)
{
    size_t n = A.size(); //robot valjda
    size_t m = A[0].rows();

    Eigen::MatrixXd S = Eigen::MatrixXd::Zero(3 * n, 3);
    Eigen::VectorXd v = Eigen::VectorXd::Zero(3 * n);

    for (int i = 0; i < n; ++i) {
        Eigen::Matrix3d A1 = logMatrix((A[i].block(0, 0, 3, 3)));
        Eigen::Matrix3d B1 = logMatrix((B[i].block(0, 0, 3, 3)));
        Eigen::Vector3d a(A1(2, 1), A1(0, 2), A1(1, 0));
        Eigen::Vector3d b(B1(2, 1), B1(0, 2), B1(1, 0));
        if (i == 0) {
            std::cout << "a: " << a << std::endl;
        }
        a.normalize();
        b.normalize();
        S.block(3 * i, 0, 3, 3) = skew(a + b);
        v.segment(3 * i, 3) = a - b;
    }

    Eigen::VectorXd x = S.colPivHouseholderQr().solve(v);
    double theta = 2 * atan(x.norm());
    x.normalize();
    Eigen::Matrix3d R = (Eigen::Matrix3d::Identity() * cos(theta) + sin(theta) * skew(x) + (1 - cos(theta)) * x * x.transpose()).eval();

    Eigen::MatrixXd C = Eigen::MatrixXd::Zero(3 * n, 3);
    Eigen::VectorXd d = Eigen::VectorXd::Zero(3 * n);

    for (int i = 0; i < n; ++i) {
        Eigen::Matrix3d I = Eigen::Matrix3d::Identity();
        C.block(3 * i, 0, 3, 3) = I - (A[i].block(0, 0, 3, 3));
        d.segment(3 * i, 3) = A[i].block(0, 3, 3, 1) - R * B[i].block(0, 3, 3, 1);
    }

    Eigen::VectorXd t = C.colPivHouseholderQr().solve(d);

    Eigen::Matrix4d X = Eigen::Matrix4d::Identity();
    X.block(0, 0, 3, 3) = R;
    X.block(0, 3, 3, 1) = t;

    return X;
}

Eigen::Matrix4d andreff(const std::vector<Eigen::MatrixXd>& AA, const std::vector<Eigen::MatrixXd>& BB) {
    size_t n = AA.size();

    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(12 * n, 12);
    Eigen::VectorXd b = Eigen::VectorXd::Zero(12 * n);

    for (int i = 0; i < n; ++i) {
        Eigen::Matrix3d Ra = AA[i].block<3, 3>(0, 0);
        Eigen::Matrix3d Rb = BB[i].block<3, 3>(0, 0);
        Eigen::Vector3d ta = AA[i].block<3, 1>(0, 3);
        Eigen::Vector3d tb = BB[i].block<3, 1>(0, 3);

        A.block<9, 9>(12 * i, 0) = Eigen::MatrixXd::Identity(9, 9) - kroneckerProduct(Rb, Ra);
        A.block<3, 12>(12 * i + 9, 0) << kroneckerProduct(tb.transpose(), Eigen::Matrix3d::Identity()), Eigen::Matrix3d::Identity() - Ra;
        b.segment<3>(12 * i + 9) = ta;
    }

    Eigen::VectorXd x = A.colPivHouseholderQr().solve(b);

    Eigen::Matrix3d X = Eigen::Map<Eigen::Matrix3d>(x.segment<9>(0).data());
    X = (std::pow(X.determinant(), -1.0 / 3.0)) * X;

    Eigen::JacobiSVD<Eigen::Matrix3d> svd(X, Eigen::ComputeFullU | Eigen::ComputeFullV);
    X = svd.matrixU() * svd.matrixV().transpose();
    if (X.determinant() < 0) {
        Eigen::Matrix3d diag = Eigen::Matrix3d::Identity();
        diag(2, 2) = -1;
        X = svd.matrixU() * diag * svd.matrixV().transpose();
    }

    Eigen::Matrix4d result;
    result.setIdentity();
    result.block<3, 3>(0, 0) = X.transpose();
    result.block<3, 1>(0, 3) = x.segment<3>(9);

    return result;
}

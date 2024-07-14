#include "OpenCV.h"

bool FindChessboardCV(cv::Size pattern_size, cv::Mat image, std::vector<cv::Point2f>& imagePoints)
{
    std::vector<cv::Point2f> corners;
    bool patternFound = cv::findChessboardCorners(image, pattern_size, corners,
        cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);
    if (patternFound)
    {
        cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.001);

        cv::Size winSize(11, 11);

        cv::Size zeroZone(-1, -1);

        // Refine corner positions
        cv::cornerSubPix(image, corners, winSize, zeroZone, criteria);
    }
    imagePoints = corners;
    return patternFound;
}



bool FindChessboardCharucoCV(cv::Mat gray, cv::Size charucoSize)
{
    bool valid = false;
    bool showRejected = true;
    bool refindStrategy = true;

    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::aruco::DetectorParameters params;

    cv::Mat imageCopy;
    cv::Mat image = gray;
    if (image.empty()) {
        std::cerr << "Could not open or find the image: " << gray << std::endl;
        return false;
    }
    image.copyTo(imageCopy);

    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners;
    cv::aruco::ArucoDetector detector(dictionary, params);
    detector.detectMarkers(image, markerCorners, markerIds);
    int marker_num = (static_cast<double>(charucoSize.width * charucoSize.height)/2) - 0.5;

    if (markerIds.size() == marker_num)
    {
        valid = true;
    }

    return valid;
}

void EstimatePoseCharucoBoard(cv::Mat gray, cv::Size charucoSize, cv::Mat& R_target2cam, cv::Mat& T_target2cam)
{
    bool valid = false;
    bool showRejected = true;
    bool refindStrategy = true;

    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::Ptr<cv::aruco::CharucoBoard> board = cv::makePtr<cv::aruco::CharucoBoard>(charucoSize, squareLength, markerLength, dictionary);
    cv::aruco::DetectorParameters params;

    cv::Mat imageCopy;
    cv::Mat image = gray;
    if (image.empty()) {
        std::cerr << "Could not open or find the image: " << gray << std::endl;
    }
    image.copyTo(imageCopy);

    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners;
    cv::aruco::ArucoDetector detector(dictionary, params);
    detector.detectMarkers(image, markerCorners, markerIds);

    if (markerIds.size() > 0)
    {
        std::cout << markerIds.size() << std::endl;
        cv::aruco::drawDetectedMarkers(imageCopy, markerCorners, markerIds);
        std::vector<cv::Point2f> charucoCorners;
        std::vector<int> charucoIds;
        cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, image, board, charucoCorners, charucoIds, cameraMatrix, distortionCoeffs);

        if (charucoIds.size() > 0)
        {
            cv::Scalar color = cv::Scalar(255, 0, 0);
            cv::Mat rvec;
            cv::aruco::drawDetectedCornersCharuco(imageCopy, charucoCorners, charucoIds, color);
            valid = cv::aruco::estimatePoseCharucoBoard(charucoCorners, charucoIds, board, cameraMatrix, distortionCoeffs, rvec, T_target2cam);
            cv::Rodrigues(rvec, R_target2cam);
            if (valid){
                cv::drawFrameAxes(imageCopy, cameraMatrix, distortionCoeffs, rvec, T_target2cam, 0.1f);
            }
        }
        cv::imshow("out", imageCopy);
        cv::waitKey(4000);
    }
}


void detectArucoMarkerNoImg(cv::Mat gray, float makerLength, int wantedMarker,std::vector<std::vector<cv::Point2f>>& wantedCorners,
    cv::Mat& R_target2cam, cv::Mat& T_target2cam, int& cx, int& cy)
{
    cv::aruco::DetectorParameters params;
    cv::aruco::Dictionary dicitionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::aruco::ArucoDetector detector(dicitionary, params);
    cv::Mat objPoints(4, 1, CV_32FC3);
    objPoints.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-makerLength / 2.f, makerLength / 2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(makerLength / 2.f, makerLength / 2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(makerLength / 2.f, -makerLength / 2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-makerLength / 2.f, -makerLength / 2.f, 0);

    cv::Mat imageCopy;
    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> corners, rejected;
    int id_place = 0;
    detector.detectMarkers(gray, corners, ids, rejected);
    for (int j = 0; j < ids.size(); j++)
    {
        if (ids[j] == wantedMarker)
        {
            id_place = j;
        }
    }
    wantedCorners.push_back(corners.at(id_place));
    cv::Mat rvec;

    if (!ids.empty())
    {
        cv::solvePnP(objPoints, corners.at(id_place), cameraMatrix, distortionCoeffs, rvec, T_target2cam);
    }
    cv::Rodrigues(rvec, R_target2cam);
    gray.copyTo(imageCopy);
    if (!ids.empty()) {
        cv::drawFrameAxes(imageCopy, cameraMatrix, distortionCoeffs, rvec, T_target2cam, makerLength * 1.5f, 2);
        cv::Point2f centroid(0, 0);
        for (const auto& corner : corners.at(id_place)) {
            centroid.x += corner.x;
            centroid.y += corner.y;
        }
        centroid.x /= 4.0;
        centroid.y /= 4.0;

        cx = static_cast<int>(centroid.x);
        cy = static_cast<int>(centroid.y);
        if (cx >= 0 && cx < gray.cols && cy >= 0 && cy < gray.rows) {
            uchar pixelValue = gray.at<uchar>(cy, cx);
            std::cout << "Center pixels:  (" << cx << ", " << cy << ")" << std::endl;
        }
    }

}

void detectArucoMarker(cv::Mat gray, float makerLength, int wantedMarker, std::vector<std::vector<cv::Point2f>>& wantedCorners,
    cv::Mat& R_target2cam, cv::Mat& T_target2cam, int& cx, int& cy)
{
    cv::aruco::DetectorParameters params;
    cv::aruco::Dictionary dicitionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::aruco::ArucoDetector detector(dicitionary, params);
    cv::Mat objPoints(4, 1, CV_32FC3);
    objPoints.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-makerLength / 2.f, makerLength / 2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(makerLength / 2.f, makerLength / 2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(makerLength / 2.f, -makerLength / 2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-makerLength / 2.f, -makerLength / 2.f, 0);

    cv::Mat imageCopy;
    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> corners, rejected;
    int id_place = 0;
    detector.detectMarkers(gray, corners, ids, rejected);
    for (int j = 0; j < ids.size(); j++)
    {
        if (ids[j] == wantedMarker)
        {
            id_place = j;
        }
    }
    wantedCorners.push_back(corners.at(id_place));
    cv::Mat rvec;

    if (!ids.empty())
    {
        cv::solvePnP(objPoints, corners.at(id_place), cameraMatrix, distortionCoeffs, rvec, T_target2cam);
    }
    cv::Rodrigues(rvec, R_target2cam);
    gray.copyTo(imageCopy);
    if (!ids.empty()) {
        cv::drawFrameAxes(imageCopy, cameraMatrix, distortionCoeffs, rvec, T_target2cam, makerLength * 1.5f, 2);
        cv::Point2f centroid(0, 0);
        for (const auto& corner : corners.at(id_place)) {
            centroid.x += corner.x;
            centroid.y += corner.y;
        }
        centroid.x /= 4.0;
        centroid.y /= 4.0;
        cx = static_cast<int>(centroid.x);
        cy = static_cast<int>(centroid.y);
        if (cx >= 0 && cx < gray.cols && cy >= 0 && cy < gray.rows) {
            uchar pixelValue = gray.at<uchar>(cy, cx);
            std::cout << "Center pixels:  (" << cx << ", " << cy << ")" << std::endl;
        }
    }
    cv::Mat colorImage = Crosses(imageCopy, wantedCorners);
    cv::imshow("out", colorImage);
    cv::waitKey(4000);

    if (wantedCorners.size() > 1) {
        wantedCorners.pop_back();
    }

}

cv::Mat Crosses(cv::Mat& image, std::vector<std::vector<cv::Point2f>>& wantedCorners)
{
    cv::Mat colorImage;
    cv::cvtColor(image, colorImage, cv::COLOR_GRAY2BGR);
    for (int i = 0; i < wantedCorners.size(); i++)
    {
        if (i == 0)
        {
            cv::line(colorImage, wantedCorners[i][0], wantedCorners[i][2], cv::Scalar(0, 0, 255), 2); // Diagonal 1
            cv::line(colorImage, wantedCorners[i][1], wantedCorners[i][3], cv::Scalar(0, 0, 255), 2); // Diagonal 2
        }
        else
        {
            cv::line(colorImage, wantedCorners[i][0], wantedCorners[i][2], cv::Scalar(0, 0, 0), 2); // Diagonal 1
            cv::line(colorImage, wantedCorners[i][1], wantedCorners[i][3], cv::Scalar(0, 0, 0), 2); // Diagonal 2
        }
    }
    return colorImage;
}


cv::Mat ReadImage(std::string image_path)
{
    try {
        cv::Mat image = cv::imread(image_path);
        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);
        return gray;
    }
    catch (...) {
        std::cout << "Error while loading image" << std::endl;
    }
}

void SolvePnP(cv::Size patternSize, std::vector<cv::Point2f> imagePoints, cv::Mat cameraMatrix, cv::Mat distortionCoeffs, cv::Mat& image, cv::Mat& rotationMatrix, cv::Mat& tvec)
{
    cv::Mat rvec;
    int checkerBoard[2] = { patternSize.width + 1, patternSize.height + 1 };
    std::vector<cv::Point3f> objp;
    for (int i = 1; i < checkerBoard[1]; i++) {
        for (int j = 1; j < checkerBoard[0]; j++) {
            objp.push_back(cv::Point3f(j * square_size, i * square_size, 0));
        }
    }
    cv::solvePnP(objp, imagePoints, cameraMatrix, distortionCoeffs, rvec, tvec);
    cv::Rodrigues(rvec, rotationMatrix);
    //Display
    cv::drawChessboardCorners(image, patternSize, imagePoints, true);
    cv::imshow("chessboard detection", image);
    cv::waitKey(4000);

}

void OpenCVCorner(cv::Size patternSize, std::vector<cv::Point2f> imagePoints, cv::Mat& image, int& cx, int& cy)
{
    cv::drawChessboardCorners(image, patternSize, imagePoints, true);
    cv::imshow("chessboard detection", image);
    cv::waitKey(4000);

    int middleIndex = (patternSize.height / 2) * patternSize.width + (patternSize.width / 2);
    if (imagePoints.size() > middleIndex) {
        cv::Point2f middleCorner = imagePoints[middleIndex];
        std::cout << "Middle Corner: (" << middleCorner.x << ", " << middleCorner.y << ")" << std::endl;
        cx = middleCorner.x; cy = middleCorner.y;
    }
    else {
        std::cerr << "Not enough corners detected to retrieve middle corner coordinates" << std::endl;
    }
}

bool FindPatternFunctionCV(std::string camera, std::string TempDir, int i, cv::Size PatternSize, std::vector<cv::Point2f>& imagePoints, cv::Mat& gray)
{
    bool PatternFound = false;
    CaptureTempImages(camera);
    std::string image_path = SaveTempImages(i, camera, TempDir);
    gray = ReadImage(image_path);
    PatternFound = FindChessboardCV(PatternSize, gray, imagePoints);
    return PatternFound;
}

bool FindPatternFunctionCharucoCV(std::string camera, std::string tempDir, int i, cv::Size CharucoSize, cv::Mat& gray)
{
    bool PatternFound = false;
    CaptureTempImages(camera);
    std::string image_path = SaveTempImages(i, camera, tempDir);
    gray = ReadImage(image_path);
    PatternFound = FindChessboardCharucoCV(gray, CharucoSize);
    return PatternFound;
}




std::tuple<cv::Mat, cv::Mat, double, cv::Vec3d, cv::Vec3d> readCameraParameters(const std::string& jsonFilePath)
{
    cv::Mat cameraMatrix;
    cv::Mat distortionCoeffs;
    double angle;
    cv::Vec3d translations;
    cv::Vec3d rotations;

    std::ifstream ifs(jsonFilePath);
    if (!ifs.is_open())
    {
        std::cerr << "Error: Failed to open file for reading" << std::endl;
        return std::make_tuple(cameraMatrix, distortionCoeffs, angle, rotations, translations);
    }

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document document;
    document.ParseStream(isw);

    if (document.HasParseError())
    {
        std::cerr << "Error: Failed to parse JSON file. "
            << "Error code: " << rapidjson::GetParseError_En(document.GetParseError()) << std::endl;
        return std::make_tuple(cameraMatrix, distortionCoeffs, angle, rotations, translations);
    }

    if (document.HasMember("Calibration") && document["Calibration"].IsObject() &&
        document["Calibration"].HasMember("Stereo") && document["Calibration"]["Stereo"].IsObject() &&
        document["Calibration"]["Stereo"].HasMember("Left") && document["Calibration"]["Stereo"]["Left"].IsObject())
    {
        if (!document["Calibration"]["Stereo"]["Left"]["Camera"].IsNull())
        {
            const rapidjson::Value& cameraValue = document["Calibration"]["Stereo"]["Left"]["Camera"];

            if (cameraValue.IsArray() && cameraValue.Size() == 3)
            {
                double cameraMatrixData[3][3];
                for (rapidjson::SizeType i = 0; i < cameraValue.Size(); ++i)
                {
                    const rapidjson::Value& row = cameraValue[i];
                    if (row.IsArray() && row.Size() == 3)
                    {
                        for (rapidjson::SizeType j = 0; j < row.Size(); ++j)
                        {
                            if (row[j].IsNumber())
                            {
                                cameraMatrixData[i][j] = row[j].GetDouble();
                            }
                            else
                            {
                                std::cerr << "Error: Invalid value type for camera matrix" << std::endl;
                                return std::make_tuple(cameraMatrix, distortionCoeffs, angle, rotations, translations);
                            }
                        }
                    }
                    else
                    {
                        std::cerr << "Error: Invalid camera matrix format" << std::endl;
                        return std::make_tuple(cameraMatrix, distortionCoeffs, angle, rotations, translations);
                    }
                }

                cameraMatrix = cv::Mat(3, 3, CV_64F, cameraMatrixData).clone().t();
            }
            else
            {
                std::cerr << "Error: Invalid camera matrix size" << std::endl;
                return std::make_tuple(cameraMatrix, distortionCoeffs, angle, rotations, translations);
            }
        }
        else
        {
            std::cerr << "Error: Camera matrix not found in JSON file" << std::endl;
            return std::make_tuple(cameraMatrix, distortionCoeffs, angle, rotations, translations);
        }

        if (!document["Calibration"]["Stereo"]["Left"]["Distortion"].IsNull())
        {
            const rapidjson::Value& distortionValue = document["Calibration"]["Stereo"]["Left"]["Distortion"];

            if (distortionValue.IsObject())
            {
                double k1, k2, k3, rx, ry, t1, t2;

                if (distortionValue.HasMember("K1") && distortionValue["K1"].IsNumber()) {
                    k1 = distortionValue["K1"].GetDouble();
                }
                else
                {
                    std::cerr << "Error: Missing or invalid K1 distortion coefficient" << std::endl;
                    return std::make_tuple(cameraMatrix, distortionCoeffs, angle, rotations, translations);
                }
                if (distortionValue.HasMember("K2") && distortionValue["K2"].IsNumber())
                {
                    k2 = distortionValue["K2"].GetDouble();
                }
                else
                {
                    std::cerr << "Error: Missing or invalid K2 distortion coefficient" << std::endl;
                    return std::make_tuple(cameraMatrix, distortionCoeffs, angle, rotations, translations);
                }
                if (distortionValue.HasMember("K3") && distortionValue["K3"].IsNumber()) {
                    k3 = distortionValue["K3"].GetDouble();
                }
                else
                {
                    std::cerr << "Error: Missing or invalid K3 distortion coefficient" << std::endl;
                    return std::make_tuple(cameraMatrix, distortionCoeffs, angle, rotations, translations);
                }

                if (distortionValue.HasMember("Rx") && distortionValue["Rx"].IsNumber())
                {
                    rx = distortionValue["Rx"].GetDouble();
                }
                else
                {
                    std::cerr << "Error: Missing or invalid Rx distortion coefficient" << std::endl;
                    return std::make_tuple(cameraMatrix, distortionCoeffs, angle, rotations, translations);
                }

                if (distortionValue.HasMember("Ry") && distortionValue["Ry"].IsNumber())
                {
                    ry = distortionValue["Ry"].GetDouble();
                }
                else
                {
                    std::cerr << "Error: Missing or invalid Ry distortion coefficient" << std::endl;
                    return std::make_tuple(cameraMatrix, distortionCoeffs, angle, rotations, translations);
                }

                if (distortionValue.HasMember("T1") && distortionValue["T1"].IsNumber())
                {
                    t1 = distortionValue["T1"].GetDouble();
                }
                else
                {
                    std::cerr << "Error: Missing or invalid T1 distortion coefficient" << std::endl;
                    return std::make_tuple(cameraMatrix, distortionCoeffs, angle, rotations, translations);
                }

                if (distortionValue.HasMember("T2") && distortionValue["T2"].IsNumber())
                {
                    t2 = distortionValue["T2"].GetDouble();
                }
                else
                {
                    std::cerr << "Error: Missing or invalid T2 distortion coefficient" << std::endl;
                    return std::make_tuple(cameraMatrix, distortionCoeffs, angle, rotations, translations);
                }

                distortionCoeffs = (cv::Mat_<double>(1, 5) << k1, k2, t1, t2, k3);
            }
            else
            {
                std::cerr << "Error: Invalid distortion object type" << std::endl;
                return std::make_tuple(cameraMatrix, distortionCoeffs, angle, rotations, translations);
            }
        }
    }
    if (document.HasMember("Link") && document["Link"].IsObject() &&
        document["Link"].HasMember("Rotation") && document["Link"]["Rotation"].IsObject())
    {
        if (!document["Link"]["Rotation"]["Angle"].IsNull())
        {
            const rapidjson::Value& AngleValue = document["Link"]["Rotation"]["Angle"];
            angle = AngleValue.GetDouble();
        }
        if (!document["Link"]["Rotation"]["Axis"].IsNull())
        {
            const rapidjson::Value& AxisValue = document["Link"]["Rotation"]["Axis"];
            rotations[0] = AxisValue[0].GetDouble();
            rotations[1] = AxisValue[1].GetDouble();
            rotations[2] = AxisValue[2].GetDouble();
        }
        if (!document["Link"]["Translation"].IsNull())
        {
            const rapidjson::Value& TranslationValue = document["Link"]["Translation"];
            translations[0] = TranslationValue[0].GetDouble();
            translations[1] = TranslationValue[1].GetDouble();
            translations[2] = TranslationValue[2].GetDouble();
        }
    }
    ifs.close();
    return std::make_tuple(cameraMatrix, distortionCoeffs, angle, rotations, translations);
}
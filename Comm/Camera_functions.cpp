#include "Camera_functions.h"


std::string StartCamera()
{
    try
    {
        nxLibInitialize(true);
        NxLibItem camera = getAvailableCamera({ valStereo, valStructuredLight, valMonocular });

        // Open the Ensenso
        std::string serial = camera[itmSerialNumber].asString();
        std::cout << "Open camera " << serial << std::endl;
        NxLibCommand open(cmdOpen);
        open.parameters()[itmCameras] = serial;
        open.execute();
        return serial;
    }
    catch (...)
    {
        std::cout << "Error while opening camera" << std::endl;
        return 0;
    }
}

void InitizalizePatternCapture()
{
    NxLibItem()[itmParameters][itmPattern][itmGridSpacing] = 50;
    NxLibCommand(cmdDiscardPatterns).execute();
}

void CloseCamera()
{

    printf("Closing camera\n");
    NxLibCommand(cmdClose).execute();
    printf("Closing NxLib\n");
    nxLibFinalize();
}



bool CaptureImages(std::string cam_name, bool decodedData)
{
    // parametri kamere
    NxLibItem()[itmCameras][{cam_name}][itmParameters][itmCapture][itmProjector] = false;
    NxLibItem()[itmCameras][{cam_name}][itmParameters][itmCapture][itmFrontLight] = true;

    // Definiranje caputre parametara
    NxLibCommand capture(cmdCapture);
    capture.parameters()[itmTimeout] = 5000;

    capture.execute();
    NxLibCommand(cmdRectifyImages).execute();
    bool foundPattern;

    try
    {
        NxLibCommand collectPattern(cmdCollectPattern);
        collectPattern.parameters()[itmCameras] = cam_name;
        if (!decodedData) {
            collectPattern.parameters()[itmDecodeData] = true;
            collectPattern.parameters()[itmUpdateGlobalPatternData] = true;
        }
        collectPattern.execute();
        foundPattern = collectPattern.result()[itmStereo].exists();
        return foundPattern;
    }
    catch (NxLibException&)
    {
        std::cout << "Pattern not found," << std::endl;
        return false;
    }
}



void CaptureTempImages(std::string cam_name)
{
    // parametri kamere
    NxLibItem()[itmCameras][{cam_name}][itmParameters][itmCapture][itmProjector] = false;
    NxLibItem()[itmCameras][{cam_name}][itmParameters][itmCapture][itmFrontLight] = true;

    // Definiranje caputre parametara
    NxLibCommand capture(cmdCapture);
    capture.parameters()[itmTimeout] = 5000;

    capture.execute();
    NxLibCommand(cmdRectifyImages).execute();
}


void SaveImages(int i, std::string cam_name)
{
    NxLibItem root;
    NxLibItem camera = root[itmCameras][cam_name];
    NxLibCommand saveImage(cmdSaveImage);
    // raw left
    saveImage.parameters()[itmNode] = camera[itmImages][itmRaw][itmLeft].path;
    saveImage.parameters()[itmFilename] = "C:/Users/crta/Desktop/Tomislav/Raw_left/raw_left_" + std::to_string(i) + ".png";
    saveImage.execute();
    // raw right
    saveImage.parameters()[itmNode] = camera[itmImages][itmRaw][itmRight].path;
    saveImage.parameters()[itmFilename] = "C:/Users/crta/Desktop/Tomislav/Raw_right/raw_right_" + std::to_string(i) + ".png";
    saveImage.execute();

    // rectified left
    saveImage.parameters()[itmNode] = camera[itmImages][itmRectified][itmLeft].path;
    saveImage.parameters()[itmFilename] = "C:/Users/crta/Desktop/Tomislav/Rectified_left/rectified_left_" + std::to_string(i) + ".png";
    saveImage.execute();

    // rectified right
    saveImage.parameters()[itmNode] = camera[itmImages][itmRectified][itmRight].path;
    saveImage.parameters()[itmFilename] = "C:/Users/crta/Desktop/Tomislav/Rectified_right/rectified_right_" + std::to_string(i) + ".png";
    saveImage.execute();

}

std::string SaveTempImages(int i, std::string cam_name, std::string newDir)
{
    NxLibItem root;
    NxLibItem camera = root[itmCameras][cam_name];
    NxLibCommand saveImage(cmdSaveImage);

    // rectified left
    saveImage.parameters()[itmNode] = camera[itmImages][itmRectified][itmLeft].path;
    std::string filePath = newDir + "rectified_left_" + std::to_string(i) + ".png";
    saveImage.parameters()[itmFilename] = filePath;
    saveImage.execute();
    return filePath;

}


void EnsensoCalibrate(std::string cam_name, cv::Mat poseMatrix)
{
    NxLibCommand calibrateHandEye(cmdCalibrateHandEye);
    calibrateHandEye.parameters()[itmSetup] = valMoving;

    for (int i = 0; i < poseMatrix.rows; ++i)
    {
        calibrateHandEye.parameters()[itmTransformations][i][itmTranslation][0] = poseMatrix.at <double>(i, 0);
        calibrateHandEye.parameters()[itmTransformations][i][itmTranslation][1] = poseMatrix.at <double>(i, 1);
        calibrateHandEye.parameters()[itmTransformations][i][itmTranslation][2] = poseMatrix.at <double>(i, 2);
        calibrateHandEye.parameters()[itmTransformations][i][itmRotation][itmAngle] = poseMatrix.at <double>(i, 3);
        calibrateHandEye.parameters()[itmTransformations][i][itmRotation][itmAxis][0] = poseMatrix.at <double>(i, 4);
        calibrateHandEye.parameters()[itmTransformations][i][itmRotation][itmAxis][1] = poseMatrix.at <double>(i, 5);
        calibrateHandEye.parameters()[itmTransformations][i][itmRotation][itmAxis][2] = poseMatrix.at <double>(i, 6);
    }

    try
    {
        std::cout << "Calibrating camera..." << std::endl;
        calibrateHandEye.execute();
        std::cout << "Camera calibrated!" << std::endl;
    }
    catch (NxLibException& ex)
    {
        std::cout << "Exception: " << ex.getErrorCode() << std::endl;
        std::cout << "Exception: " << ex.getErrorText() << std::endl;
        auto item = calibrateHandEye.result()[itmErrorText];
        std::cout << "Result: " << item.asString() << std::endl;
    }

    NxLibCommand storeCalibration(cmdStoreCalibration);
    storeCalibration.parameters()[itmCameras] = cam_name;
    storeCalibration.parameters()[itmLink] = true;
    storeCalibration.execute();

}

void WriteParamFile(std::string cam_name)
{

    NxLibItem root;
    NxLibItem camera = root[itmCameras][cam_name];
    std::ofstream file("calibrated_params.json");
    if (file.is_open())
    {
        file << camera.asJson(true);
    }
    else
    {
        std::cout << "Camera parameters file coud not be opened for writing" << std::endl;
    }
}


XYZ getTraingulatedPoint(std::string cam_name, int pixelX, int pixelY)
{
    NxLibCommand capture(cmdCapture);
    capture.execute();
    NxLibCommand computeDisparity(cmdComputeDisparityMap);
    computeDisparity.execute();
    NxLibCommand computePoints(cmdComputePointMap);
    computePoints.execute();
    int width, height, channels, bytesPerElement;
    bool isFloat;
    double timestamp;

    // query information about the binary data
    NxLibItem()[itmCameras][{cam_name}][itmImages][itmPointMap].getBinaryDataInfo(
        &width, &height, &channels, &bytesPerElement, &isFloat, &timestamp);
    assert(
        width > 0 && height > 0 &&
        channels == 3);
    assert(isFloat && bytesPerElement == sizeof(float));
    std::vector<XYZ> points;
    NxLibItem()[itmCameras][{cam_name}][itmImages][itmPointMap].getBinaryData(points, &timestamp);
    XYZ pixelValue = points[pixelY * width + pixelX];

    return pixelValue;
}

cv::Mat CameraCoordinates(XYZ point) {
    double theta = std::get<2>(cameraParams);
    cv::Vec3d u = std::get <3>(cameraParams);
    cv::Vec3d transVec = std::get <4>(cameraParams);
    cv::Mat R = angleAxisToMatrix(theta, u);
    cv::Mat R_4 = cv::Mat::eye(4, 4, CV_64F);
    R.copyTo(R_4(cv::Rect(0, 0, 3, 3)));
    R_4.at<double>(0, 3) = transVec[0];
    R_4.at<double>(1, 3) = transVec[1];
    R_4.at<double>(2, 3) = transVec[2];
    cv::Mat pointHomogeneous = (cv::Mat_<double>(4, 1) << point.x, point.y, point.z, 1.0);
    cv::Mat points_camera = R_4 * pointHomogeneous;
    cv::Mat result = (cv::Mat_<double>(3, 1) << points_camera.at<double>(0, 0) / 1000,
        points_camera.at<double>(1, 0) / 1000,
        points_camera.at<double>(2, 0) / 1000);
    return result;
}

cv::Mat getStereoPoint(std::string camera_name, int pixelX, int pixelY)
{
    XYZ point = getTraingulatedPoint(camera_name, pixelX, pixelY);
    cv::Mat point_coord = CameraCoordinates(point);
    return point_coord;
}




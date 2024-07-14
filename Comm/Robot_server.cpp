#include "Robot_server.h"

void runServer() {
    WSADATA wsData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return;
    }

    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8001); // Port number

    result = bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    result = listen(sockfd, 1);
    if (result == SOCKET_ERROR) {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    std::cout << "Server started. Waiting for a connection..." << std::endl;

    sockaddr_in cli_addr;
    int cli_len = sizeof(cli_addr);
    SOCKET clientSocket = accept(sockfd, (sockaddr*)&cli_addr, &cli_len);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    std::cout << "Client connected" << std::endl;
    serverRunning = true;
    while (serverRunning) {
        if (comm_msg.ackn == 102 || comm_msg.ackn == 103)
        {
            poruka.ackn = 90000 + comm_msg.ackn;
        }
        if (comm_msg.ackn == 808)
        {
            usao_u_catch = true;
            std::cout << "Robot can not reach given position" << std::endl;
            poruka.ackn = 808;
        }
        if (comm_msg.ackn == 90102 || comm_msg.ackn == 90103)
        {
            poruka.ackn = 1;
        }
        std::string init = createJsonString(poruka).append("\n");
        result = send(clientSocket, init.c_str(), init.length(), 0);
        CommMsg comm_msg = GetCoords(clientSocket);
    }

    // Close the client and server sockets
    closesocket(clientSocket);
    closesocket(sockfd);
    WSACleanup();
}


std::string createJsonString(CommMsg poruka) {
    rapidjson::Document doc;
    doc.SetObject();

    doc.AddMember("ipad", poruka.ackn, doc.GetAllocator());
    doc.AddMember("X_kam", poruka.x, doc.GetAllocator());
    doc.AddMember("Y_kam", poruka.y, doc.GetAllocator());
    doc.AddMember("Z_kam", poruka.z, doc.GetAllocator());
    doc.AddMember("A_kam", poruka.a, doc.GetAllocator());
    doc.AddMember("B_kam", poruka.b, doc.GetAllocator());
    doc.AddMember("C_kam", poruka.c, doc.GetAllocator());

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

CommMsg GetCoords(SOCKET clientSocket) {
    rapidjson::Document doc; // za node
    const int bufferSize = 256;
    char buffer[bufferSize];
    std::string receivedMessage;

    int bytesRead = recv(clientSocket, buffer, bufferSize, 0);
    if (bytesRead > 0) {
        receivedMessage.append(buffer, bytesRead);
    }
    else if (bytesRead == 0) {
        std::cout << "Client disconnected" << std::endl;
    }
    else {
        std::cerr << "Receive failed with error: " << WSAGetLastError() << std::endl;
    }
    doc.Parse(receivedMessage.c_str());
    comm_msg.ackn = doc["ackn"].GetInt();
    comm_msg.x = doc["x"].GetDouble();
    comm_msg.y = doc["y"].GetDouble();
    comm_msg.z = doc["z"].GetDouble();
    comm_msg.a = doc["a"].GetDouble();
    comm_msg.b = doc["b"].GetDouble();
    comm_msg.c = doc["c"].GetDouble();
    return comm_msg;
}


void RobotSleep() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
}


void MovePTP(Offset offset) //102
{
    poruka = poruka = { comm_msg.x + offset.x_off,comm_msg.y + offset.y_off,comm_msg.z + offset.z_off ,
    (offset.a_rot) * (3.141592 / 180),(offset.b_rot) * (3.141592 / 180),(offset.c_rot) * (3.141592 / 180),102 };
    while (comm_msg.ackn != 1)
    {
        RobotSleep();
    }
    std::cout << "Robot finished movement" << std::endl;
    robot_at_pos = true;
}

void MoveLIN(Offset offset) //103
{
    poruka = { comm_msg.x + offset.x_off,comm_msg.y + offset.y_off,comm_msg.z + offset.z_off ,
    (comm_msg.a + offset.a_rot) * (3.141592 / 180),(comm_msg.b + offset.b_rot) * (3.141592 / 180),(comm_msg.c + offset.c_rot) * (3.141592 / 180),103 };
    std::this_thread::sleep_for(std::chrono::seconds(2));

    while (comm_msg.ackn != 1)
    {
        RobotSleep();
    }
    std::cout << "Robot finished movement" << std::endl;
    robot_at_pos = true;
}

void MoveHome(CommMsg inital_pos) //102
{
    poruka = { inital_pos.x, inital_pos.y, inital_pos.z, inital_pos.a * (3.141592 / 180), inital_pos.b * (3.141592 / 180), inital_pos.c * (3.141592 / 180), 102 };
    std::this_thread::sleep_for(std::chrono::seconds(2));
    while (comm_msg.ackn != 1)
    {
        RobotSleep();
    }
    std::cout << "Robot finished movement" << std::endl;
}

void MoveAbsLIN(CommMsg comm_msg) //103
{
    poruka = { comm_msg.x, comm_msg.y, comm_msg.z, comm_msg.a * (3.141592 / 180), comm_msg.b * (3.141592 / 180), comm_msg.c * (3.141592 / 180), 103 };
    std::this_thread::sleep_for(std::chrono::seconds(2));
    while (comm_msg.ackn != 1)
    {
        RobotSleep();
    }
    std::cout << "Robot finished movement" << std::endl;
}

void MoveAbsPTP(Offset offset) //103
{
    poruka = {offset.x_off, offset.y_off, offset.z_off, offset.a_rot * (3.141592 / 180), offset.b_rot * (3.141592 / 180), offset.c_rot * (3.141592 / 180), 103 };
    std::this_thread::sleep_for(std::chrono::seconds(1));
    while (comm_msg.ackn != 1)
    {
        RobotSleep();
    }
    std::cout << "Robot finished movement" << std::endl;
}

void MoveAbsNoRotLIN(double x, double y, double z) //103
{
    poruka = { x, y, z, comm_msg.a * (3.141592 / 180), comm_msg.b * (3.141592 / 180), comm_msg.c * (3.141592 / 180), 103 };
    std::this_thread::sleep_for(std::chrono::seconds(1));
    while (comm_msg.ackn != 1)
    {
        RobotSleep();
    }
    std::cout << "Robot finished movement" << std::endl;
}


void MoveTool_H (cv::Mat startPos, cv::Mat cam2gripper, double radx, double rady, int axis,cv::Mat gripper2base)
{
    cv::Mat new_gripper2base, R;
    cv::Mat desiredGripperPos = cv::Mat::eye(4, 4, CV_64F);
    desiredGripperPos.at<double>(0, 3) = startPos.at<double>(0, 0);
    desiredGripperPos.at<double>(1, 3) = startPos.at<double>(1, 0);
    desiredGripperPos.at<double>(2, 3) = startPos.at<double>(2, 0);

    cv::Mat YRotation = (cv::Mat_<double>(4, 4) <<
        cos(rady), 0, sin(rady), 0,
        0, 1, 0, 0,
        -sin(rady), 0, cos(rady), 0,
        0, 0, 0, 1);

    cv::Mat XRotation = (cv::Mat_<double>(4, 4) <<
        1, 0, 0, 0,
        0, cos(radx), -sin(radx), 0,
        0, sin(radx), cos(radx), 0,
        0, 0, 0, 1);
    cv::Mat XYRotation = XRotation * YRotation;
    if (axis == 1) {
        new_gripper2base = gripper2base * cam2gripper * desiredGripperPos * XYRotation * desiredGripperPos.inv() * cam2gripper.inv();
    }
    else {
        new_gripper2base = gripper2base * cam2gripper * desiredGripperPos * XRotation * desiredGripperPos.inv() * cam2gripper.inv();
    }
    double x_pos = new_gripper2base.at<double>(0, 3);
    double y_pos = new_gripper2base.at<double>(1, 3);
    double z_pos = new_gripper2base.at<double>(2, 3);

    R = new_gripper2base(cv::Rect(0, 0, 3, 3)).clone();
    cv::Vec3d new_euler = rotationMatrixToEulerAngles(R);

    poruka = { x_pos * 1000,y_pos * 1000,z_pos * 1000,new_euler[0], new_euler[1], new_euler[2],102 };
    std::this_thread::sleep_for(std::chrono::seconds(2));
    while (comm_msg.ackn != 1)
    {
        RobotSleep();
    }
    std::cout << "Robot finished movement" << std::endl;
}


void MoveToolRotValid_H(cv::Mat cam2gripper, double odstupanje,cv::Mat gripper2base)
{
    cv::Mat new_gripper2base, R;
    cv::Mat desiredGripperPos = cv::Mat::eye(4, 4, CV_64F);
    desiredGripperPos.at<double>(0, 3) = 0;
    desiredGripperPos.at<double>(1, 3) = 0;
    desiredGripperPos.at<double>(2, 3) = odstupanje;

    new_gripper2base = gripper2base * cam2gripper * desiredGripperPos * cam2gripper.inv();
    double x_pos = new_gripper2base.at<double>(0, 3);
    double y_pos = new_gripper2base.at<double>(1, 3);
    double z_pos = new_gripper2base.at<double>(2, 3);

    R = new_gripper2base(cv::Rect(0, 0, 3, 3)).clone();
    cv::Vec3d new_euler = rotationMatrixToEulerAngles(R);

    poruka = { x_pos * 1000,y_pos * 1000,z_pos * 1000,new_euler[0], new_euler[1], new_euler[2],103 };
    std::this_thread::sleep_for(std::chrono::seconds(2));
    while (comm_msg.ackn != 1)
    {
        RobotSleep();
    }
    std::cout << "Robot finished movement" << std::endl;
}

void ToolRotWorldPos(Offset offset)
{
    cv::Vec3d New_Euler;
    cv::Mat T = cv::Mat::eye(4, 4, CV_64F);
    cv::Mat T_trans = cv::Mat::eye(4, 4, CV_64F);
    cv::Mat OffMatrix;
    cv::Mat Off_T_trans;
    cv::Mat ResultMatrix;
    cv::Mat TempResultMatrix;

    cv::Vec3d positions(comm_msg.x, comm_msg.y, comm_msg.z);

    double thetax = offset.c_rot * (CV_PI / 180.0);
    double thetay = offset.b_rot * (CV_PI / 180.0);
    double thetaz = offset.a_rot * (CV_PI / 180.0);

    cv::Mat rotationMatrixX = (cv::Mat_<double>(4, 4) <<
        1, 0, 0, 0,
        0, cos(thetax), -sin(thetax), 0,
        0, sin(thetax), cos(thetax), 0,
        0, 0, 0, 1
        );

    cv::Mat rotationMatrixY = (cv::Mat_<double>(4, 4) <<
        cos(thetay), 0, sin(thetay), 0,
        0, 1, 0, 0,
        -sin(thetay), 0, cos(thetay), 0,
        0, 0, 0, 1
        );

    cv::Mat rotationMatrixZ = (cv::Mat_<double>(4, 4) <<
        cos(thetaz), -sin(thetaz), 0, 0,
        sin(thetaz), cos(thetaz), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
        );


    cv::Vec3d euler(comm_msg.a * (CV_PI / 180.0), comm_msg.b * (CV_PI / 180.0), comm_msg.c * (CV_PI / 180.0));

    cv::Mat R_gripper2base = EulerToMatrix(euler);
    cv::Mat t_gripper2base = (cv::Mat_<double>(3, 1) << comm_msg.x, comm_msg.y, comm_msg.z);
    cv::Mat gripper2base = createTransformationMatrix(R_gripper2base, t_gripper2base);


    OffMatrix = rotationMatrixZ * rotationMatrixY * rotationMatrixX;
    cv::Mat identityMatrix = cv::Mat::eye(4, 4, CV_64F);
    cv::Mat newMatrix = gripper2base * OffMatrix;
    New_Euler = rotationMatrixToEulerAngles(newMatrix(cv::Rect(0, 0, 3, 3)));
    double x = newMatrix.at<double>(0, 3);
    double y = newMatrix.at<double>(1, 3);
    double z = newMatrix.at<double>(2, 3);

    poruka = { x, y, z, New_Euler[0], New_Euler[1], New_Euler[2], 103 };

    std::this_thread::sleep_for(std::chrono::seconds(2));
    while (comm_msg.ackn != 1)
    {
        RobotSleep();
    }

    std::cout << "Robot finished movement" << std::endl;
    robot_at_pos = true;
}



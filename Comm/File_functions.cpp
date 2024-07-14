#include "File_functions.h"

std::string CreateTempDirectory() {

    char currentPath[MAX_PATH];
    if (!GetCurrentDirectoryA(MAX_PATH, currentPath)) {
        std::cerr << "Error getting current directory" << std::endl;
        return "";
    }

    srand((unsigned int)time(NULL));
    std::string tempDirName = std::string(currentPath) + "\\" + "ImageFile" + "_" + std::to_string(rand());

    if (!CreateDirectoryA(tempDirName.c_str(), NULL)) {
        std::cerr << "Error creating temporary directory" << std::endl;
        return "";
    }

    return tempDirName + "\\";
}

int inputFile(std::string inputFIlePath,std::vector<std::string>& lines)
{
    std::ifstream inputFile(inputFIlePath);
    if(!inputFile)
    {
        std::cerr << "Error opening input file" << std::endl;
        return 1;
    }
    std::string line;
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();
    return 0;
}

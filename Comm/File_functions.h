#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H

#include <iostream>
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::string CreateTempDirectory();

int inputFile(std::string inputFIlePath, std::vector<std::string>& lines);


#endif

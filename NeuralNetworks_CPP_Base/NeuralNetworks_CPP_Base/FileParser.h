#pragma once

#include "string"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

struct SingleNumberData {
	std::vector<std::string> rowData;
	std::vector<int> rowDataRawFlattened;
	unsigned int number;
};

struct NumberFileData {
	std::vector<SingleNumberData> numberDatas;
};

class FileParser
{
public:
	std::string readFileToString(const std::string& filePath);
	NumberFileData processFilePathToFileData(const std::string& filePath);
};




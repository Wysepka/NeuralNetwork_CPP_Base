#pragma once

#include "string"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

struct SingleNumberData {
	std::vector<std::string> rowData;
	std::vector<int> rowDataRawFlattened;
	unsigned int number;
};

struct NumberFileData {
public:
	std::vector<SingleNumberData> numberDatas;

	void Shuffle();
};

class FileParser
{
public:
	std::string readFileToString(const std::string& filePath);
	NumberFileData processFilePathToFileData(const std::string& filePath);
};




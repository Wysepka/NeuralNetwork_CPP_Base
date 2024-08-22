#pragma once
#include "FileParser.h"

class DataLogWriter
{
public:
	void OutputNumberFileData(NumberFileData numberFileData);
	void OutputNumberPrediction(SingleNumberData numberData, int number);
};


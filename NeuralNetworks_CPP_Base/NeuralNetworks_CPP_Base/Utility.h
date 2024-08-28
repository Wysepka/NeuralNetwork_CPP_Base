#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iterator>
#include <numeric>
#include "FileParser.h"

class Utility
{
public:
	static std::vector<SingleNumberData> GetRandomSingleNumbers(const std::vector<SingleNumberData>& inputVector, size_t count = 10);
};


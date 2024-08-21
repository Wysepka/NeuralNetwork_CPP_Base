#pragma once
#include <cmath>

class Functions
{
public:
	static double sigmoid(double f) { return 1.0 / (1.0 + exp(-f)); }
	static double sigmoid_prim(double f) { return f * (1.0 - f); }
};


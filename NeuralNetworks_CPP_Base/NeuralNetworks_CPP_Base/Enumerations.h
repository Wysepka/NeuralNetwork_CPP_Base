#pragma once
#include <string>

class Enumeration 
{
public:
	enum NeuronType {Invalid, Input, Hidden, Output};

	static std::string NeuronTypeToString(NeuronType neuronType) {
		switch (neuronType)
		{
		case Enumeration::Invalid:
			return "Invalid";
			break;
		case Enumeration::Input:
			return "Input";
			break;
		case Enumeration::Hidden:
			return "Hidden";
			break;
		case Enumeration::Output:
			return "Output";
			break;
		default:
			break;
		}
	}
};
#pragma once
#include "Enumerations.h"

struct NeuronInfo
{
	unsigned int NeuronID;
	Enumeration::NeuronType NeuronType;

	NeuronInfo(unsigned int neuronID, Enumeration::NeuronType neuronType)
	{
		this->NeuronID = neuronID;
		this->NeuronType = neuronType;
	}
};


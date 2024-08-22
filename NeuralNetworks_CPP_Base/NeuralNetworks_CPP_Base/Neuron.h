#pragma once
#include "Synapse.h"
#include "vector"
#include "memory"
#include "utility"
#include "stdexcept"
#include "Synapse.h"
#include "Functions.h"
#include "Enumerations.h"

using SynapseArray = std::unique_ptr<std::vector<std::shared_ptr<Synapse>>>;

class Neuron
{
private:
	unsigned int neuronID;
	unsigned int layerID;
	float bias;
	float gradientValue;
	float value;
	float valueSum;
	int feedCount;
	SynapseArray forwardSynapses;
	SynapseArray backwardSynapses;
	Enumeration::NeuronType neuronType;
public:
	Neuron(unsigned int neuronID, unsigned int layerID , float bias ,Enumeration::NeuronType neuronType);
	void Initialize(SynapseArray forwardSynapses, SynapseArray backwardSynapses);
	void FeedValue(float value);
	void CalculateValue();
	void PreFeedValueInitialize();
	void FeedForward();
	float GetOutput();
	void CalculateGradient_Output(int originalValue);
	void CalculateGradient_Hidden();
	void CalculateForwardWeights(float learningRate);
	void CalculateBiases(float learningRate);
	float GetGradientValue();
	void Dispose();
};


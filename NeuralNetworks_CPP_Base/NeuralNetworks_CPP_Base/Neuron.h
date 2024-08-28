#pragma once
#include "Synapse.h"
#include "vector"
#include "memory"
#include "utility"
#include "stdexcept"
#include "Synapse.h"
#include "Functions.h"
#include "Enumerations.h"

using SynapseArrayUnique = std::unique_ptr<std::vector<std::shared_ptr<Synapse>>>;
using SynapseArrayShared = std::shared_ptr<std::vector<std::shared_ptr<Synapse>>>;
using NestedSynapseArray = std::shared_ptr<std::vector<std::vector<std::shared_ptr<Synapse>>>>;
#define NestedNeuronArray = std::shared_ptr<std::vector<std::vector<std::shared_ptr<Neuron>>>>;

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
	SynapseArrayUnique forwardSynapses;
	SynapseArrayUnique backwardSynapses;
	Enumeration::NeuronType neuronType;
public:
	Neuron(unsigned int neuronID, unsigned int layerID , float bias, float value ,Enumeration::NeuronType neuronType);
	void Initialize(SynapseArrayUnique forwardSynapses, SynapseArrayUnique backwardSynapses);
	void SetValue(float value);
	void FeedValue(float value);
	void CalculateValue();
	void PreFeedValueInitialize();
	void FeedForward();
	float GetOutput();
	float GetBiasValue();
	Enumeration::NeuronType GetNeuronType();
	SynapseArrayShared GetSynapses();
	void CalculateGradient_Output(int originalValue);
	void CalculateGradient_Hidden();
	void CalculateForwardWeights(float learningRate);
	void CalculateBiases(float learningRate);
	float GetGradientValue();
	void Dispose();
};


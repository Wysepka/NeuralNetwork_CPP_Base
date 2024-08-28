#pragma once
#include <functional>
#include "Enumerations.h"
#include "NeuronInfo.h"

class Synapse
{
private:
	NeuronInfo backwardNeuronInfo;
	NeuronInfo forwardNeuronInfo;

	float weight;

	std::function<void(float)> forwardNeuronFeed;
	std::function<float()> forwardNeuronGradientValue;

public:
	Synapse(NeuronInfo backwardNeuronInfo, NeuronInfo forwardNeuronInfo, float weight, std::function<void(float)> forwardNeuronFeed , std::function<float()> forwardNeuronGradientValue);
	void FeedToForwardLayer(float value);
	float GetForwardGradientValue();
	void UpdateWeight(float value);
	float GetWeight();
};


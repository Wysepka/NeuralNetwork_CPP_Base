#include "Synapse.h"
#include <functional>

Synapse::Synapse(NeuronInfo backwardNeuronInfo, NeuronInfo forwardNeuronInfo, float weight, std::function<void(float)> forwardNeuronFeed , std::function<float()> forwardNeuronGradientValue) 
	: backwardNeuronInfo(backwardNeuronInfo) , forwardNeuronInfo(forwardNeuronInfo) , weight(weight) , forwardNeuronFeed(forwardNeuronFeed) , forwardNeuronGradientValue(forwardNeuronGradientValue)
{}

void Synapse::FeedToForwardLayer(float value)
{
	if (forwardNeuronFeed != nullptr) 
	{
		forwardNeuronFeed(value * weight);
	}
}

float Synapse::GetForwardGradientValue()
{
	return weight * forwardNeuronGradientValue();
}

void Synapse::UpdateWeight(float value)
{
	this->weight -= value;
}

float Synapse::GetWeight()
{
	return weight;
}

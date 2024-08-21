#include "Synapse.h"
#include <functional>

Synapse::Synapse(unsigned int backwardID, unsigned int forwardID , float weight, std::function<void(float)> forwardNeuronFeed) 
	: backwardID(backwardID) , forwardID(forwardID) , weight(weight) , forwardNeuronFeed(forwardNeuronFeed)
{}

void Synapse::FeedToForwardLayer(float value)
{
	if (forwardNeuronFeed != nullptr) 
	{
		forwardNeuronFeed(value * weight);
	}
}

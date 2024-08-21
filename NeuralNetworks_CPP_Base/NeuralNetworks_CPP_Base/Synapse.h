#pragma once
#include <functional>

class Synapse
{
private:
	unsigned int backwardID;
	unsigned int forwardID;

	float weight;

	std::function<void(float)> forwardNeuronFeed;

public:
	Synapse(unsigned int backwardID, unsigned int forwardID, float weight, std::function<void(float)> forwardNeuronFeed);
	void FeedToForwardLayer(float value);

};


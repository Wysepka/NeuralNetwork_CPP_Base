#pragma once
#include "Synapse.h"
#include "vector"
#include "memory"
#include "utility"
#include "stdexcept"

using SynapseArray = std::unique_ptr<std::vector<std::shared_ptr<Synapse>>>;

class Neuron
{
private:
	unsigned int neuronID;
	unsigned int layerID;
	SynapseArray forwardSynapses;
	SynapseArray backwardSynapses;
public:
	Neuron(unsigned int neuronID, unsigned int layerID);
	void Initialize(SynapseArray forwardSynapses, SynapseArray backwardSynapses);
	void Dispose();
};


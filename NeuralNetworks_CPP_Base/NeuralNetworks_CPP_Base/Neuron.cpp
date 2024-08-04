#include "Neuron.h"

Neuron::Neuron(unsigned int neuronID, unsigned int layerID)
{
	this->neuronID = neuronID;
	this->layerID = layerID;
}

void Neuron::Initialize(SynapseArray forwardSynapses, SynapseArray backwardSynapses)
{
	this->forwardSynapses = std::move(forwardSynapses);
	this->backwardSynapses = std::move(backwardSynapses);
}

void Neuron::Dispose()
{
	forwardSynapses.reset();
	backwardSynapses.reset();
	/*
	for (auto& forwardSynapsePtr : *forwardSynapses) {
		if (forwardSynapsePtr) {
			forwardSynapsePtr.reset();
		}
		else {
			throw std::runtime_error("Could not access ForwardSynapsePtr");
		}
	}

	forwardSynapses.reset();

	for (auto& backwardSynapsesPtr : *backwardSynapses) {
		if (backwardSynapsesPtr) {
			backwardSynapsesPtr.reset();
		}
		else {
			throw std::runtime_error("Could not access BackwardSynapsePtr");
		}
	}
	*/
}

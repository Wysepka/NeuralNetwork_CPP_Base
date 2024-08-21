#include "Neuron.h"

Neuron::Neuron(unsigned int neuronID, unsigned int layerID , float bias) : neuronID(neuronID) , layerID(layerID) , bias(bias) , value(0.f) , gradientValue(0.f)
{}

void Neuron::Initialize(SynapseArray forwardSynapses, SynapseArray backwardSynapses)
{
	if (forwardSynapses.get() != nullptr) 
	{
		this->forwardSynapses = std::move(forwardSynapses);
	}
	if (backwardSynapses.get() != nullptr) 
	{
		this->backwardSynapses = std::move(backwardSynapses);
	}
}

void Neuron::FeedValue(float value)
{
	valueSum += value;
	feedCount++;
}

void Neuron::CalculateValue()
{
	value = valueSum / feedCount;
	value = Functions::sigmoid(value + bias);
}

void Neuron::PreFeedValueInitialize()
{
	valueSum = 0;
	feedCount = 0;
}

void Neuron::FeedForward()
{
	CalculateValue();
	for (size_t i = 0; i < forwardSynapses->size(); i++)
	{
		(*forwardSynapses)[i]->FeedToForwardLayer(value);
	}
}

float Neuron::GetOutput()
{
	CalculateValue();
	return value;
}

void Neuron::CalculateGradient(int originalValue)
{
	CalculateValue();
	gradientValue = (originalValue - value) * Functions::sigmoid(value);
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

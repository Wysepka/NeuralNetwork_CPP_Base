#include "Neuron.h"

Neuron::Neuron(unsigned int neuronID, unsigned int layerID , float bias ,float value, Enumeration::NeuronType neuronType) : neuronID(neuronID) , layerID(layerID) , bias(bias), neuronType(neuronType), value(value), gradientValue(0.f)
{}

void Neuron::Initialize(SynapseArrayUnique forwardSynapses, SynapseArrayUnique backwardSynapses)
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

void Neuron::SetValue(float value)
{
	this->value = value;
	this->valueSum = value;
	this->feedCount = 1;
}

void Neuron::FeedValue(float value)
{
	valueSum += value;
	feedCount++;
}

void Neuron::CalculateValue()
{
	int additionalBiasCheck = neuronType == Enumeration::NeuronType::Hidden ? bias : bias;

	value = (valueSum /*+ additionalBiasCheck*/) /* / feedCount */;
	value = Functions::sigmoid(value + additionalBiasCheck);
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

float Neuron::GetBiasValue()
{
	return bias;
}

Enumeration::NeuronType Neuron::GetNeuronType()
{
    return neuronType;
}

SynapseArrayShared Neuron::GetSynapses()
{
	return std::make_shared<std::vector<std::shared_ptr<Synapse>>>(*forwardSynapses);
}

void Neuron::CalculateGradient_Output(int originalValue)
{
	CalculateValue();
	gradientValue = (value - originalValue) * Functions::sigmoid_prim(value);
}

void Neuron::CalculateGradient_Hidden()
{
	CalculateValue();
	float sum = 0;
	for (size_t i = 0; i < forwardSynapses->size(); i++)
	{
		sum += (*forwardSynapses)[i]->GetForwardGradientValue();
	}
	gradientValue = sum * Functions::sigmoid_prim(value);
}

void Neuron::CalculateForwardWeights(float learningRate)
{
	for (size_t i = 0; i < forwardSynapses->size(); i++)
	{
		auto forwardSynapseRef = (*forwardSynapses)[i];
		float forwardGradientValue = forwardSynapseRef->GetForwardGradientValue();
		float weightSubtraction = learningRate * forwardGradientValue * value;
		forwardSynapseRef->UpdateWeight(weightSubtraction);
	}
}

void Neuron::CalculateBiases(float learningRate)
{
	bias -= learningRate * gradientValue;
}

float Neuron::GetGradientValue()
{
	return gradientValue;
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

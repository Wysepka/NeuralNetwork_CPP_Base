#include "NeuralNetwork.h"

void NeuralNetwork::InitializeNeuralNetwork(int inputLayerNum, int hiddenLayerNum, int outputLayerNum)
{
	this->inputLayerNum = inputLayerNum;
	this->hiddenLayerNum = hiddenLayerNum;
	this->outputLayerNum = outputLayerNum;

	inputLayerNeurons = std::make_unique<std::vector<std::shared_ptr<Neuron>>>();
	hiddenLayerNeurons = std::make_unique<std::vector<std::shared_ptr<Neuron>>>();
	outputLayerNeurons = std::make_unique<std::vector<std::shared_ptr<Neuron>>>();

	for (size_t i = 0; i < inputLayerNum; i++)
	{
		inputLayerNeurons->push_back(std::make_shared<Neuron>(i , 0));
	}

	for (size_t i = 0; i < hiddenLayerNum; i++)
	{
		hiddenLayerNeurons->push_back(std::make_shared<Neuron>(i , 1));
	}

	for (size_t i = 0; i < outputLayerNum; i++)
	{
		outputLayerNeurons->push_back(std::make_shared<Neuron>(i , 2));
	}

	std::array<std::array<std::shared_ptr<Synapse>,hiddenLayerNum >, inputLayerNum> secondLayerSynapses = std::
	for (size_t i = 0; i < inputLayerNum; i++)
	{
		
		SynapseArray inputLayerSynapses = std::make_unique<std::vector<std::shared_ptr<Synapse>>>();
		for (size_t j = 0; j < hiddenLayerNeurons->size(); j++)
		{
			inputLayerSynapses->push_back(std::make_shared<Synapse>(i, j));
		}
		(*inputLayerNeurons)[i]->Initialize(std::move(inputLayerSynapses), NULL);
	}
}

void NeuralNetwork::DisposeNeuralNetwork()
{
	for (auto& inputLayerPtr : *inputLayerNeurons)
	{
		if (inputLayerPtr) {
			inputLayerPtr.reset();
		}
		else {
			throw std::runtime_error("Could not access InputLayerPtr");
		}
	}

	inputLayerNeurons.reset();

	for (auto& hiddenLayerPtr : *hiddenLayerNeurons)
	{
		if (hiddenLayerNeurons) {
			hiddenLayerNeurons.reset();
		}
		else {
			throw std::runtime_error("Could not access HiddenLayerPtr");
		}
	}

	hiddenLayerNeurons.reset();

	for (auto& outputLayerPtr : *outputLayerNeurons)
	{
		if (outputLayerPtr) {
			outputLayerPtr.reset();
		}
		else {
			throw std::runtime_error("Could not access OutputLayerPtr");
		}
	}

	outputLayerNeurons.reset();
}

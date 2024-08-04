#pragma once
#include "vector"
#include "memory"
#include"stdexcept"
#include "Neuron.h"
#include "array"


class NeuralNetwork
{
private:
	std::unique_ptr<std::vector<std::shared_ptr<Neuron>>> inputLayerNeurons;
	std::unique_ptr<std::vector<std::shared_ptr<Neuron>>> hiddenLayerNeurons;
	std::unique_ptr<std::vector<std::shared_ptr<Neuron>>> outputLayerNeurons;

	const unsigned int inputLayerNum;
	const unsigned int hiddenLayerNum;
	const unsigned int outputLayerNum;

public:
	void InitializeNeuralNetwork(int inputLayerNum, int hiddenLayerNum, int outputLayerNum);
	void DisposeNeuralNetwork();
};


#pragma once
#include "vector"
#include "memory"
#include "Neuron.h"


class NeuralNetwork
{
private:
	std::unique_ptr<std::vector<Neuron>> inputLayerNeurons;
	std::unique_ptr<std::vector<Neuron>> hiddenLayerNeurons;
	std::unique_ptr<std::vector<int>> outputLayer;

public:
	void InitializeNeuralNetwork(int inputLayerNum, int hiddenLayerNum, int outputLayerNum);
};


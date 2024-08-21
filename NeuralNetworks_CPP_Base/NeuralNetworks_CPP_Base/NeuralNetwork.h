#pragma once
#include "vector"
#include "memory"
#include"stdexcept"
#include "Neuron.h"
#include "array"
#include <iostream>
#include <random>
#include <functional>
#include "FileParser.h"
#include "Synapse.h"

class NeuralNetwork
{
private:
	std::unique_ptr<std::vector<std::shared_ptr<Neuron>>> inputLayerNeurons;
	std::unique_ptr<std::vector<std::shared_ptr<Neuron>>> hiddenLayerNeurons;
	std::unique_ptr<std::vector<std::shared_ptr<Neuron>>> outputLayerNeurons;

	unsigned int inputLayerNum;
	unsigned int hiddenLayerNum;
	unsigned int outputLayerNum;

public:
	void InitializeNeuralNetwork(int inputLayerNum, int hiddenLayerNum, int outputLayerNum);
	void Predict(NumberFileData numberFileData, float learningRate);
	void PredictSingleNumber(SingleNumberData singleNumberData, float learningRate);
	int TrainSingleIteration(std::vector<float> outputGradience);
	void DisposeNeuralNetwork();
};


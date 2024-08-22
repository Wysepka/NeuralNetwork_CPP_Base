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
	unsigned int lastPredictedNum;

	std::vector<float> PredictSingleNumber(SingleNumberData singleNumberData);
	int TrainSingleIteration(std::vector<float> outputGradience, float learningRate);
	//int GetClosestValueIndex(std::vector<float> array , );
	int closestToOne(const std::vector<float>& array);

public:
	void InitializeNeuralNetwork(int inputLayerNum, int hiddenLayerNum, int outputLayerNum);
	void ComputeNeuralNetwork(NumberFileData numberFileData, float learningRate);
	void GetPredictionOnSingleFileNumber(SingleNumberData singleNumberData);
	void DisposeNeuralNetwork();

	int GetLastPredictedNum();
};


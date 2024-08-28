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
#include "DataLogWriter.h"

class NeuralNetwork
{
private:
	std::unique_ptr<std::vector<std::shared_ptr<Neuron>>> inputLayerNeurons;
	std::unique_ptr<std::vector<std::shared_ptr<Neuron>>> hiddenLayerNeurons;
	std::unique_ptr<std::vector<std::shared_ptr<Neuron>>> outputLayerNeurons;

	std::shared_ptr<std::vector<std::vector<std::shared_ptr<Neuron>>>> allNeurons;

	unsigned int inputLayerNum;
	unsigned int hiddenLayerNum;
	unsigned int outputLayerNum;
	unsigned int lastPredictedNum;
	unsigned int correctlyPredictedNums;

	std::unique_ptr<DataLogWriter> dataLogWriter;

	std::shared_ptr<std::vector<float>> outputPrediction;
	std::shared_ptr<std::vector<int>> corretlyPredictedNumsPerDigit;

	void PredictSingleNumber(SingleNumberData singleNumberData);
	int TrainSingleIteration(SingleNumberData singleNumberData, float learningRate);
	//int GetClosestValueIndex(std::vector<float> array , );
	int CloseToOne(const std::shared_ptr<std::vector<float>> array);
	int CloseToZero(const std::shared_ptr<std::vector<float>> array);

public:
	void InitializeNeuralNetwork(int inputLayerNum, int hiddenLayerNum, int outputLayerNum);
	void ComputeNeuralNetwork(NumberFileData numberFileData, float learningRate);
	void GetPredictionOnSingleFileNumber(SingleNumberData singleNumberData);
	void DisposeNeuralNetwork();

	int GetLastPredictedNum();
};


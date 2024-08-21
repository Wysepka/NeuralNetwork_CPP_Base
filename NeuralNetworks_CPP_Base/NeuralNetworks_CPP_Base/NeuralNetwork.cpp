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
		inputLayerNeurons->push_back(std::make_shared<Neuron>(i , 0 , 0));
	}

	for (size_t i = 0; i < hiddenLayerNum; i++)
	{
		hiddenLayerNeurons->push_back(std::make_shared<Neuron>(i , 1 , 0));
	}

	for (size_t i = 0; i < outputLayerNum; i++)
	{
		outputLayerNeurons->push_back(std::make_shared<Neuron>(i , 2 , 0));
	}
	std::random_device rd;
	std::mt19937 gen(rd()); // Mersenne Twister engine
	std::uniform_real_distribution<> distr(-1.f, 1.f);

	std::vector<std::vector<std::shared_ptr<Synapse>>> firstLayerSynapses(inputLayerNum, std::vector<std::shared_ptr<Synapse>>(hiddenLayerNum));
	std::vector<std::vector<std::shared_ptr<Synapse>>> secondLayerSynapses(inputLayerNum, std::vector<std::shared_ptr<Synapse>>(hiddenLayerNum));
	for (size_t i = 0; i < inputLayerNum; i++)
	{
		
		SynapseArray inputLayerSynapses = std::make_unique<std::vector<std::shared_ptr<Synapse>>>();
		for (size_t j = 0; j < hiddenLayerNum; j++)
		{
			auto randomNumber = static_cast<float>(distr(gen));
			auto neuronForwardFeedFunc = [&neuron = (*hiddenLayerNeurons)[j]](float value) {neuron->FeedValue(value); };
 			std::shared_ptr<Synapse> SynapsePtr = std::make_shared<Synapse>(i, j , randomNumber , neuronForwardFeedFunc);
			inputLayerSynapses->push_back(SynapsePtr);
		}
		firstLayerSynapses.push_back(*inputLayerSynapses);
		(*inputLayerNeurons)[i]->Initialize(std::move(inputLayerSynapses), NULL);
	}

	for (size_t i = 0; i < hiddenLayerNum; i++)
	{
		std::unique_ptr<std::vector<std::shared_ptr<Synapse>>> hiddenLayerBackward = std::make_unique<std::vector<std::shared_ptr<Synapse>>>();
		for (size_t j = 0; j < firstLayerSynapses.size(); j++)
		{
			hiddenLayerBackward->push_back(firstLayerSynapses[j][i]);
		}

		SynapseArray hiddenLayerSynapses = std::make_unique<std::vector<std::shared_ptr<Synapse>>>();
		for (size_t j = 0; j < outputLayerNum; j++)
		{
			auto randomNumber = static_cast<float>(distr(gen));
			auto neuronForwardFeedFunc = [&neuron = (*hiddenLayerNeurons)[j]](float value) {neuron->FeedValue(value); };
			std::shared_ptr<Synapse> SynapsePtr = std::make_shared<Synapse>(i, j , randomNumber , neuronForwardFeedFunc);
			hiddenLayerSynapses->push_back(SynapsePtr);
		}

		secondLayerSynapses.push_back(*hiddenLayerSynapses);
		(*hiddenLayerNeurons)[i]->Initialize(std::move(hiddenLayerSynapses), std::move(hiddenLayerBackward));
	}

	for (size_t i = 0; i < outputLayerNum; i++)
	{
		std::unique_ptr<std::vector<std::shared_ptr<Synapse>>> outputLayerBackward = std::make_unique<std::vector<std::shared_ptr<Synapse>>>();
		for (size_t j = 0; j < hiddenLayerNum; j++)
		{
			outputLayerBackward->push_back(secondLayerSynapses[j][i]);
		}
		(*outputLayerNeurons)[i]->Initialize(NULL, std::move(outputLayerBackward));
	}

	std::cout << "Neural Network Digit Recognition Initialization Ended ! \n";
}

void NeuralNetwork::Predict(NumberFileData numberFileData, float learningRate)
{

}

void NeuralNetwork::PredictSingleNumber(SingleNumberData singleNumberData, float learningRate)
{
	if (singleNumberData.rowDataRawFlattened.size() != inputLayerNeurons->size()) {
		throw std::runtime_error("Input Size is different than Input Neurons !");
		return;
	}

	for (size_t i = 0; i < inputLayerNum; i++)
	{
		(*inputLayerNeurons)[i]->PreFeedValueInitialize();
	}

	for (size_t i = 0; i < hiddenLayerNeurons->size(); i++)
	{
		(*hiddenLayerNeurons)[i]->PreFeedValueInitialize();
	}
	
	for (size_t i = 0; i < outputLayerNeurons->size(); i++)
	{
		(*outputLayerNeurons)[i]->PreFeedValueInitialize();
	}

	for (size_t i = 0; i < inputLayerNum; i++)
	{
		(*inputLayerNeurons)[i]->FeedValue(static_cast<float>(singleNumberData.rowDataRawFlattened[i]));
	}
	for (size_t i = 0; i < inputLayerNeurons->size(); i++)
	{
		(*inputLayerNeurons)[i]->FeedForward();
	}
	for (size_t i = 0; i < hiddenLayerNeurons->size(); i++)
	{
		(*hiddenLayerNeurons)[i]->FeedForward();
	}
	std::vector<float> outputDifference;
	for (size_t i = 0; i < outputLayerNeurons->size(); i++)
	{
		float outputPredicted = (*outputLayerNeurons)[i]->GetOutput();

	}
}

int NeuralNetwork::TrainSingleIteration(std::vector<float> outputGradience)
{
	for (size_t i = 0; i < outputLayerNeurons->size(); i++)
	{

	}

	return 0;
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

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
		inputLayerNeurons->push_back(std::make_shared<Neuron>(i , 0 , 0 ,Enumeration::NeuronType::Input));
	}

	for (size_t i = 0; i < hiddenLayerNum; i++)
	{
		hiddenLayerNeurons->push_back(std::make_shared<Neuron>(i , 1 , 0 , Enumeration::NeuronType::Hidden));
	}

	for (size_t i = 0; i < outputLayerNum; i++)
	{
		outputLayerNeurons->push_back(std::make_shared<Neuron>(i , 2 , 0 , Enumeration::NeuronType::Output));
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
			auto neuronForwardGradientValueFunc = [&neuron = (*hiddenLayerNeurons)[j]]() {neuron->GetGradientValue(); };
			NeuronInfo ForwardNeuronInfo(i, Enumeration::NeuronType::Input);
			NeuronInfo BackwardNeuronInfo(j, Enumeration::NeuronType::Hidden);
 			std::shared_ptr<Synapse> SynapsePtr = std::make_shared<Synapse>(ForwardNeuronInfo, BackwardNeuronInfo, randomNumber , neuronForwardFeedFunc , neuronForwardGradientValueFunc);
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
			auto neuronForwardFeedFunc = [&neuron = (*outputLayerNeurons)[j]](float value) {neuron->FeedValue(value); };
			auto neuronForwardGradientValueFunc = [&neuron = (*outputLayerNeurons)[j]]() {neuron->GetGradientValue(); };
			NeuronInfo ForwardNeuronInfo(i, Enumeration::NeuronType::Hidden);
			NeuronInfo BackwardNeuronInfo(j, Enumeration::NeuronType::Output);
			std::shared_ptr<Synapse> SynapsePtr = std::make_shared<Synapse>(ForwardNeuronInfo, BackwardNeuronInfo , randomNumber , neuronForwardFeedFunc , neuronForwardGradientValueFunc);
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

void NeuralNetwork::ComputeNeuralNetwork(NumberFileData numberFileData, float learningRate)
{
	for (size_t i = 0; i < numberFileData.numberDatas.size(); i++)
	{
		std::vector<float> outputPrediction = PredictSingleNumber(numberFileData.numberDatas[i]);

		lastPredictedNum = closestToOne(outputPrediction);

		TrainSingleIteration(outputPrediction, learningRate);
	}
}

void NeuralNetwork::GetPredictionOnSingleFileNumber(SingleNumberData numberData)
{
	std::vector<float> outputPrediction = PredictSingleNumber(numberData);
	lastPredictedNum = closestToOne(outputPrediction);
}



std::vector<float> NeuralNetwork::PredictSingleNumber(SingleNumberData singleNumberData)
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
	std::vector<float> outputPrediction;
	for (size_t i = 0; i < outputLayerNeurons->size(); i++)
	{
		outputPrediction.push_back((*outputLayerNeurons)[i]->GetOutput());
	}
}

int NeuralNetwork::TrainSingleIteration(std::vector<float> outputGradience , float learningRate)
{
	for (size_t i = 0; i < outputLayerNeurons->size(); i++)
	{
		(*outputLayerNeurons)[i]->CalculateGradient_Output(outputGradience[i]);
	}
	for (size_t i = 0; i < hiddenLayerNeurons->size(); i++)
	{
		(*hiddenLayerNeurons)[i]->CalculateGradient_Hidden();
	}
	
	for (size_t i = 0; i < hiddenLayerNeurons->size(); i++)
	{
		(*hiddenLayerNeurons)[i]->CalculateForwardWeights(learningRate);
	}
	for (size_t i = 0; i < inputLayerNeurons->size(); i++)
	{
		(*inputLayerNeurons)[i]->CalculateForwardWeights(learningRate);
	}

	for (size_t i = 0; i < outputLayerNeurons->size(); i++)
	{
		(*outputLayerNeurons)[i]->CalculateBiases(learningRate);
	}
	for (size_t i = 0; i < hiddenLayerNeurons->size(); i++)
	{
		(*hiddenLayerNeurons)[i]->CalculateBiases(learningRate);
	}

	return 0;
}

int NeuralNetwork::closestToOne(const std::vector<float>& array) {
	if (array.empty()) {
		// Handle the empty case, e.g., return -1 to indicate no valid index.
		return -1;
	}

	int closestIndex = 0;
	float minDifference = std::numeric_limits<float>::max();

	for (int i = 0; i < array.size(); ++i) {
		float difference = std::fabs(array[i] - 1.0f);
		if (difference < minDifference) {
			minDifference = difference;
			closestIndex = i;
		}
	}

	return closestIndex;
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

int NeuralNetwork::GetLastPredictedNum()
{
	return lastPredictedNum;
}

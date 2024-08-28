#include "NeuralNetwork.h"

void NeuralNetwork::InitializeNeuralNetwork(int inputLayerNum, int hiddenLayerNum, int outputLayerNum)
{
	outputPrediction = std::make_shared<std::vector<float>>();
	corretlyPredictedNumsPerDigit = std::make_shared<std::vector<int>>();

	dataLogWriter = std::make_unique<DataLogWriter>();
	dataLogWriter->Initialize();

	this->inputLayerNum = inputLayerNum;
	this->hiddenLayerNum = hiddenLayerNum;
	this->outputLayerNum = outputLayerNum;

	for (size_t i = 0; i < outputLayerNum; i++)
	{
		corretlyPredictedNumsPerDigit->push_back(i);
	}

	inputLayerNeurons = std::make_unique<std::vector<std::shared_ptr<Neuron>>>();
	hiddenLayerNeurons = std::make_unique<std::vector<std::shared_ptr<Neuron>>>();
	outputLayerNeurons = std::make_unique<std::vector<std::shared_ptr<Neuron>>>();

	std::random_device rd;
	std::mt19937 gen(rd()); // Mersenne Twister engine
	std::uniform_real_distribution<> distr(0.f, 0.25f);

	for (size_t i = 0; i < inputLayerNum; i++)
	{
		inputLayerNeurons->push_back(std::make_shared<Neuron>(i , 0 , static_cast<float>(distr(gen)), static_cast<float>(distr(gen)), Enumeration::NeuronType::Input));
	}

	for (size_t i = 0; i < hiddenLayerNum; i++)
	{
		hiddenLayerNeurons->push_back(std::make_shared<Neuron>(i , 1 , static_cast<float>(distr(gen)), static_cast<float>(distr(gen)), Enumeration::NeuronType::Hidden));
	}

	for (size_t i = 0; i < outputLayerNum; i++)
	{
		outputLayerNeurons->push_back(std::make_shared<Neuron>(i , 2 , static_cast<float>(distr(gen)), static_cast<float>(distr(gen)), Enumeration::NeuronType::Output));
	}

	allNeurons = std::make_shared<std::vector<std::vector<std::shared_ptr<Neuron>>>>(
		std::initializer_list<std::vector<std::shared_ptr<Neuron>>>{
		(*inputLayerNeurons),
		(*hiddenLayerNeurons),
		(*outputLayerNeurons)
	}
	);



	NestedSynapseArray firstLayerSynapses = std::make_shared<std::vector<std::vector<std::shared_ptr<Synapse>>>>();
	NestedSynapseArray secondLayerSynapses = std::make_shared<std::vector<std::vector<std::shared_ptr<Synapse>>>>();

	for (size_t i = 0; i < inputLayerNum; i++)
	{
		
		SynapseArrayUnique inputLayerSynapses = std::make_unique<std::vector<std::shared_ptr<Synapse>>>();
		for (size_t j = 0; j < hiddenLayerNum; j++)
		{
			auto randomNumber = static_cast<float>(distr(gen));
			auto neuronForwardFeedFunc = [&neuron = (*hiddenLayerNeurons)[j]](float value) {neuron->FeedValue(value); };
			auto neuronForwardGradientValueFunc = [&neuron = (*hiddenLayerNeurons)[j]]() -> float {return neuron->GetGradientValue(); };
			NeuronInfo ForwardNeuronInfo(i, Enumeration::NeuronType::Input);
			NeuronInfo BackwardNeuronInfo(j, Enumeration::NeuronType::Hidden);
 			std::shared_ptr<Synapse> SynapsePtr = std::make_shared<Synapse>(ForwardNeuronInfo, BackwardNeuronInfo, randomNumber , neuronForwardFeedFunc , neuronForwardGradientValueFunc);
			inputLayerSynapses->push_back(SynapsePtr);
		}
		firstLayerSynapses->push_back(*inputLayerSynapses);
		(*inputLayerNeurons)[i]->Initialize(std::move(inputLayerSynapses), NULL);
	}

	for (size_t i = 0; i < hiddenLayerNum; i++)
	{
		std::unique_ptr<std::vector<std::shared_ptr<Synapse>>> hiddenLayerBackward = std::make_unique<std::vector<std::shared_ptr<Synapse>>>();
		for (size_t j = 0; j < firstLayerSynapses->size(); j++)
		{
			hiddenLayerBackward->push_back((*firstLayerSynapses)[j][i]);
		}

		SynapseArrayUnique hiddenLayerSynapses = std::make_unique<std::vector<std::shared_ptr<Synapse>>>();
		for (size_t j = 0; j < outputLayerNum; j++)
		{
			auto randomNumber = static_cast<float>(distr(gen));
			auto neuronForwardFeedFunc = [&neuron = (*outputLayerNeurons)[j]](float value) {neuron->FeedValue(value); };
			auto neuronForwardGradientValueFunc = [&neuron = (*outputLayerNeurons)[j]]() -> float {return neuron->GetGradientValue(); };
			NeuronInfo ForwardNeuronInfo(i, Enumeration::NeuronType::Hidden);
			NeuronInfo BackwardNeuronInfo(j, Enumeration::NeuronType::Output);
			std::shared_ptr<Synapse> SynapsePtr = std::make_shared<Synapse>(ForwardNeuronInfo, BackwardNeuronInfo , randomNumber , neuronForwardFeedFunc , neuronForwardGradientValueFunc);
			hiddenLayerSynapses->push_back(SynapsePtr);
		}

		secondLayerSynapses->push_back(*hiddenLayerSynapses);
		(*hiddenLayerNeurons)[i]->Initialize(std::move(hiddenLayerSynapses), std::move(hiddenLayerBackward));
	}

	for (size_t i = 0; i < outputLayerNum; i++)
	{
		std::unique_ptr<std::vector<std::shared_ptr<Synapse>>> outputLayerBackward = std::make_unique<std::vector<std::shared_ptr<Synapse>>>();
		for (size_t j = 0; j < hiddenLayerNum; j++)
		{
			outputLayerBackward->push_back((*secondLayerSynapses)[j][i]);
		}
		(*outputLayerNeurons)[i]->Initialize(NULL, std::move(outputLayerBackward));
	}

	std::cout << "Neural Network Digit Recognition Initialization Ended ! \n";
}

void NeuralNetwork::ComputeNeuralNetwork(NumberFileData numberFileData, float learningRate)
{
	for (size_t i = 0; i < numberFileData.numberDatas.size(); i++)
	{
		if (outputPrediction != nullptr && outputPrediction->size() > 0) 
		{
			outputPrediction->clear();
		}
		PredictSingleNumber(numberFileData.numberDatas[i]);

		lastPredictedNum = CloseToOne(outputPrediction);

		dataLogWriter->LogSinglePredictionStep(allNeurons, numberFileData.numberDatas[i], outputPrediction , i);

		if (lastPredictedNum == numberFileData.numberDatas[i].number)
		{
			correctlyPredictedNums++;
			(*corretlyPredictedNumsPerDigit)[lastPredictedNum]++;
		}

		TrainSingleIteration(numberFileData.numberDatas[i], learningRate);

		dataLogWriter->LogSingleTrainStep(allNeurons, numberFileData.numberDatas[i], outputPrediction, i);
	}
}

void NeuralNetwork::GetPredictionOnSingleFileNumber(SingleNumberData numberData)
{
	if (outputPrediction != nullptr && outputPrediction->size() > 0)
	{
		outputPrediction->clear();
	}
	//std::shared_ptr<std::vector<float>> outputPrediction = std::make_shared<std::vector<float>>();
	PredictSingleNumber(numberData);
	lastPredictedNum = CloseToOne(outputPrediction);
}



void NeuralNetwork::PredictSingleNumber(SingleNumberData singleNumberData)
{
	if (singleNumberData.rowDataRawFlattened.size() != inputLayerNeurons->size()) {
		throw std::runtime_error("Input Size is different than Input Neurons !");
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
		(*inputLayerNeurons)[i]->SetValue(static_cast<float>(singleNumberData.rowDataRawFlattened[i]));
	}
	for (size_t i = 0; i < inputLayerNeurons->size(); i++)
	{
		(*inputLayerNeurons)[i]->FeedForward();
	}
	for (size_t i = 0; i < hiddenLayerNeurons->size(); i++)
	{
		(*hiddenLayerNeurons)[i]->FeedForward();
	}
	for (size_t i = 0; i < outputLayerNeurons->size(); i++)
	{
		outputPrediction->push_back((*outputLayerNeurons)[i]->GetOutput());
	}


}

int NeuralNetwork::TrainSingleIteration(SingleNumberData singleNumberData, float learningRate)
{
	std::vector<int> outputSimplified;
	for (size_t i = 0; i < outputLayerNeurons->size(); i++)
	{
		outputSimplified.push_back(i == singleNumberData.number ? 1 : 0);
	}

	for (size_t i = 0; i < outputLayerNeurons->size(); i++)
	{
		(*outputLayerNeurons)[i]->CalculateGradient_Output(outputSimplified[i]);
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

int NeuralNetwork::CloseToOne(const std::shared_ptr<std::vector<float>> array) {
	if (array->empty()) {
		// Handle the empty case, e.g., return -1 to indicate no valid index.
		return -1;
	}

	int closestIndex = 0;
	float minDifference = std::numeric_limits<float>::max();

	for (int i = 0; i < array->size(); ++i) {
		float difference = std::fabs((*array)[i] - 1.0f);
		if (difference < minDifference) {
			minDifference = difference;
			closestIndex = i;
		}
	}

	return closestIndex;
}

int NeuralNetwork::CloseToZero(const std::shared_ptr<std::vector<float>> array) {
	if (array->empty()) {
		// Handle the empty case, e.g., return -1 to indicate no valid index.
		return -1;
	}

	int closestIndex = 0;
	float minDifference = std::numeric_limits<float>::max();

	for (int i = 0; i < array->size(); ++i) {
		float difference = std::fabs(0 - (*array)[i]);
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

	outputPrediction.reset();
	corretlyPredictedNumsPerDigit.reset();

	dataLogWriter->Dispose();
}

int NeuralNetwork::GetLastPredictedNum()
{
	return lastPredictedNum;
}

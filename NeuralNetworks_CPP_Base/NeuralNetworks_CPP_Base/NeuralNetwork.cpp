#include "NeuralNetwork.h"

void NeuralNetwork::InitializeNeuralNetwork(int inputLayerNum, int hiddenLayerNum, int outputLayerNum)
{
	inputLayerNeurons = std::make_unique<std::vector<Neuron>>();
	hiddenLayerNeurons = std::make_unique<std::vector<Neuron>>();
	outputLayer = std::make_unique<std::vector<int>>();

	//inputLayerNeurons.get(new std::vector<Neuron>(10));
}

#pragma once
#include <iostream>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>
#include "Neuron.h"
#include "FileParser.h"

class DataLogWriter
{
private:
	const std::string LogFileName{ "LogNeuralNetwork_" };
	const int MaxNeuronsInRow = 8;

	const std::string LogFileDir{ "D:/Projekty/NeuralNetwork_CPP_Base/NeuralNetworks_CPP_Base/NeuralNetworks_CPP_Base/Saved" };

	static std::string CurrentDateTimeString;

	std::unique_ptr<std::ofstream> logFile;

	std::unique_ptr<std::filesystem::path> fullPathFileName;

public:
	void Initialize();
	void OutputNumberFileData(NumberFileData numberFileData);
	void OutputNumberPrediction(SingleNumberData numberData, int number);
	void LogSinglePredictionStep(std::shared_ptr<std::vector<std::vector<std::shared_ptr<Neuron>>>> nestedNeuronArray,const SingleNumberData& singleNumberData ,std::shared_ptr<std::vector<float>> outputPrediction , int iterationNum);
	void LogSingleTrainStep(std::shared_ptr<std::vector<std::vector<std::shared_ptr<Neuron>>>> nestedNeuronArray, const SingleNumberData& singleNumberData, std::shared_ptr<std::vector<float>> outputPredition, int iterationNum);
	void PrintNeuron();
	void PrintSynapses();
	void Dispose();
};


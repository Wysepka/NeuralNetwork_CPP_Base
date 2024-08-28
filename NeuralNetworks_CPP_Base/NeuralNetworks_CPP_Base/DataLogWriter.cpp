#include "DataLogWriter.h"
#include "FileParser.h"

using NeuronType = Enumeration::NeuronType;

std::string DataLogWriter::CurrentDateTimeString = "DummyValue";

void DataLogWriter::Initialize()
{
	auto now = std::chrono::system_clock::now();

	// Convert to time_t to get the time in seconds
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

	// Convert to tm structure for local time
	std::tm localTime;
	localtime_s(&localTime , &currentTime);

	// Create a stringstream to format the time as a string
	std::stringstream timeStream;
	timeStream << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S"); // Format: "YYYY-MM-DD HH:MM:SS"

	CurrentDateTimeString = timeStream.str();

	// Replace ':' with '_'
	for (std::string::size_type pos = 0; (pos = CurrentDateTimeString.find(':', pos)) != std::string::npos; pos += 1) {
		CurrentDateTimeString.replace(pos, 1, "_");
	}

	// Replace '-' with '_'
	for (std::string::size_type pos = 0; (pos = CurrentDateTimeString.find('-', pos)) != std::string::npos; pos += 1) {
		CurrentDateTimeString.replace(pos, 1, "_");
	}

	// Replace ' ' (space) with '_'
	for (std::string::size_type pos = 0; (pos = CurrentDateTimeString.find(' ', pos)) != std::string::npos; pos += 1) {
		CurrentDateTimeString.replace(pos, 1, "_");
	}

	std::filesystem::path executablePath = std::filesystem::current_path(); // or std::filesystem::canonical(argv[0])
	std::filesystem::path dataPath = executablePath / "Saved"; // assuming "Data" is your data folder
	
	std::string fullLogFileName = LogFileName + CurrentDateTimeString + ".txt";

	fullPathFileName = std::make_unique<std::filesystem::path>(std::filesystem::path(dataPath / fullLogFileName));
	//fullPathFileName->
	//fullPathFileName->append(std::filesystem::path(dataPath / fullLogFileName));

	// Check if the file exists
	std::ifstream inFile(*fullPathFileName);
	logFile = std::make_unique<std::ofstream>();
	if (inFile) {
		std::cout << "File already exists: " << fullPathFileName << std::endl;
	}
	else {
		// Create a new file
		logFile->open(*fullPathFileName , std::ios_base::app);
		if (logFile) {
			std::cout << "File created: " << fullPathFileName << std::endl;
		}
		else {
			std::cerr << "Failed to create file: " << fullPathFileName << std::endl;
		}
		*logFile << "==============================| Log File For NeuralNetwork Date&Time: " << CurrentDateTimeString << " |==============================\n";

		logFile->close();
	}

}

void DataLogWriter::OutputNumberFileData(NumberFileData numberFileData)
{
	for (size_t i = 0; i < numberFileData.numberDatas.size(); i++)
	{
		std::cout << "IT:" << i << " | Number:" << numberFileData.numberDatas[i].number << "\n";
		auto numberData = numberFileData.numberDatas[i];
		for (size_t j = 0; j < numberData.rowData.size(); j++)
		{
			std::cout << numberData.rowData[j];
			std::cout << "\n";
		}
		std::cout << "\n";
	}
}

void DataLogWriter::OutputNumberPrediction(SingleNumberData numberData, int number)
{
	for (size_t j = 0; j < numberData.rowData.size(); j++)
	{
		std::cout << numberData.rowData[j];
		std::cout << "\n";
	}
	std::cout << "\n";

	std::cout << "=====================================\n";
	std::cout << "Predicted File Is: " << number << " ||| But Original is: "<< numberData.number << '\n';
	std::cout << "=====================================\n";
}

void DataLogWriter::LogSinglePredictionStep(std::shared_ptr<std::vector<std::vector<std::shared_ptr<Neuron>>>> nestedNeuronArray, 
											const SingleNumberData& singleNumberData, 
											std::shared_ptr<std::vector<float>> outputPrediction , int iterationNum)
{
	//std::cout << "Exec Path: " << executablePath << " ||| \n";

	if (logFile == nullptr) {
		throw std::runtime_error("LogFile is not initialized, aborting Logging Prediction Values !");
		return;
	}

	logFile->open(*fullPathFileName , std::ios_base::app);

	if (!logFile->is_open()) {
		throw std::runtime_error("LogFile is not Opened !, aborting Logging Prediction Values");
		return;
	}

	*logFile << "===============================| PREDICTION : ITERATION NUMBER : " << iterationNum << " |=============================== \n";

	for (size_t i = 0; i < nestedNeuronArray->size(); i++)
	{
		NeuronType trainedNeuronType = NeuronType::Invalid;
		int neuronIterator = 0;
		for (size_t j = 0; j < (*nestedNeuronArray)[i].size(); j++)
		{
			auto neuron = (*nestedNeuronArray)[i][j];
			trainedNeuronType = neuron->GetNeuronType();
			if (neuron->GetNeuronType() == NeuronType::Input) 
			{
				*logFile << " | NeuronType::Input / Num: "<< j << " / Value: " << neuron->GetOutput() << " / Synapses ";
				auto forwardSynapses = neuron->GetSynapses();
				int synapseIterator;
				for (size_t k = 0; k < forwardSynapses->size(); k++)
				{
					auto synapse = (*forwardSynapses)[k];
					*logFile << " === / SynapseID : " << k << " - Weight: " << synapse->GetWeight();
				}
				//*logFile << " === \n";
			}

			if (neuron->GetNeuronType() == NeuronType::Hidden) {
				*logFile << " | NeuronType::Hidden / Num: " << j << " / Value:" << neuron->GetOutput() << " / Synapses: ";
				auto forwardSynapses = neuron->GetSynapses();
				int synapseIterator;
				for (size_t k = 0; k < forwardSynapses->size(); k++)
				{
					auto synapse = (*forwardSynapses)[k];
					*logFile << "=== / SynapseID : " << k << " - Weight: " << synapse->GetWeight();
				}
				//*logFile << " === \n";
			}

			if (neuron->GetNeuronType() == NeuronType::Output) 
			{
				*logFile << " | NeuronType::Output / Num: " << j << " / Value:" << neuron->GetOutput();
				//*logFile << "\n";
			}

			neuronIterator++;
			*logFile << "\n";
		}
		*logFile << "\n";

		*logFile << "===============================| PREDICTION ITERATION OF NEURON TYPE: " << Enumeration::NeuronTypeToString(trainedNeuronType) << "  LOG END |=============================== \n";

		*logFile << "\n";
	}

	*logFile << "===============================| PREDICTION ITERATION LOG END |=============================== \n";
	*logFile << "\n";

	logFile->close();
}

void DataLogWriter::LogSingleTrainStep(std::shared_ptr<std::vector<std::vector<std::shared_ptr<Neuron>>>> nestedNeuronArray, const SingleNumberData& singleNumberData, std::shared_ptr<std::vector<float>> outputPredition, int iterationNum)
{
	//std::cout << "Exec Path: " << executablePath << " ||| \n";

	if (logFile == nullptr) {
		throw std::runtime_error("LogFile is not initialized, aborting Training Logging Values !");
		return;
	}

	logFile->open(*fullPathFileName , std::ios_base::app);

	if (!logFile->is_open()) {
		throw std::runtime_error("LogFile is not Opened !, aborting Training Logging Values");
		return;
	}

	*logFile << "===============================| TRAINING : ITERATION NUMBER : " << iterationNum << " |=============================== \n";
	*logFile << "===============================| TRAINING ON NUMBER : " << singleNumberData.number << " |=============================== \n";

	for (size_t i = 0; i < outputPredition->size(); i++)
	{
		*logFile << "===============================| OUTPUT PREDICTION FOR OUTPUT NEURON NUMBER: " << i << " / PREDICTION: " << (*outputPredition)[i] << " |=============================== \n";
	}

	*logFile << "\n";

	for (size_t i = 0; i < nestedNeuronArray->size(); i++)
	{
		NeuronType trainedNeuronType = NeuronType::Invalid;
		int neuronIterator = 0;
		for (size_t j = 0; j < (*nestedNeuronArray)[i].size(); j++)
		{
			auto neuron = (*nestedNeuronArray)[i][j];
			trainedNeuronType = neuron->GetNeuronType();
			if (neuron->GetNeuronType() == NeuronType::Input)
			{
				*logFile << " | NeuronType::Input / Num: " << j << " / Value: " << neuron->GetOutput() << " / GradientValue: " << neuron->GetGradientValue() << " / Bias: " << neuron->GetBiasValue() << " / Synapses (Input->Hidden) ";
				auto forwardSynapses = neuron->GetSynapses();
				int synapseIterator;
				for (size_t k = 0; k < forwardSynapses->size(); k++)
				{
					auto synapse = (*forwardSynapses)[k];
					*logFile << " === / SynapseID : " << k << " - Weight: " << synapse->GetWeight();
				}
				//*logFile << " === \n";
			}

			if (neuron->GetNeuronType() == NeuronType::Hidden) {
				*logFile << " | NeuronType::Hidden / Num: " << j << " / Value: " << neuron->GetOutput() << " / GradientValue: " << neuron->GetGradientValue() << " / Bias: " << neuron->GetBiasValue() << " / Synapses (Hidden->Output) ";
				auto forwardSynapses = neuron->GetSynapses();
				int synapseIterator;
				for (size_t k = 0; k < forwardSynapses->size(); k++)
				{
					auto synapse = (*forwardSynapses)[k];
					*logFile << "=== / SynapseID : " << k << " - Weight: " << synapse->GetWeight();
				}
				//*logFile << " === \n";
			}

			if (neuron->GetNeuronType() == NeuronType::Output)
			{
				*logFile << " | NeuronType::Output / Num: " << j << " / Value: " << neuron->GetOutput() << " / GradientValue: " << neuron->GetGradientValue() << " / Bias: " << neuron->GetBiasValue() << "\n";
			}

			neuronIterator++;
			*logFile << "\n";

			/*
			if (neuronIterator == 8) {
				neuronIterator = 0;
			}
			*/
		}
		*logFile << "\n";

		*logFile << "===============================| TRAINING ITERATION OF NEURON TYPE: "<< Enumeration::NeuronTypeToString(trainedNeuronType) <<"  LOG END |=============================== \n";

		*logFile << "\n";
	}

	*logFile << "===============================| TRAINING ITERATION LOG END |=============================== \n";

	*logFile << "\n";

	logFile->close();
}

void DataLogWriter::PrintNeuron()
{
}

void DataLogWriter::PrintSynapses()
{
}

void DataLogWriter::Dispose()
{
	logFile->close();
}





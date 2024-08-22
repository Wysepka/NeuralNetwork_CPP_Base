#include "DataLogWriter.h"

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
	std::cout << "Predicted File Is: " << number << '\n';
	std::cout << "=====================================\n";
}

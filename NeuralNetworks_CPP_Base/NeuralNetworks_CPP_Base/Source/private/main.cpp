/*
Licensed under the MIT License given below.
Copyright 2023 Daniel Lidstrom
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "../public/neural.h"
#include <iomanip>
#include <iostream>
#include "../../DataLogWriter.h"
#include "../../FileParser.h"
#include "../../NeuralNetwork.h"

using namespace Neural;

namespace {
    const int ITERS = 4000;
    const double lr = 1.0;
    unsigned int P = 2147483647;
    unsigned int A = 16807;
    unsigned int current = 1;
    double Rand() {
        current = current * A % P;
        double result = (double)current / P;
        return result;
    }

    size_t Xor(size_t i, size_t j) { return i ^ j; }
    size_t Xnor(size_t i, size_t j) { return 1 - Xor(i, j); }
    size_t Or(size_t i, size_t j) { return i | j; }
    size_t And(size_t i, size_t j) { return i & j; }
    size_t Nor(size_t i, size_t j) { return 1 - Or(i, j); }
    size_t Nand(size_t i, size_t j) { return 1 - And(i, j); }
}

void show_weights(const Network& network);

void CalculateLogicGates();
void CalculateHandWrittenDigits();

int main() {
    CalculateLogicGates();

    CalculateHandWrittenDigits();

    return 0;
}

void CalculateHandWrittenDigits() {
    Matrix inputs = Matrix();
    Matrix outputs = Matrix();

    DataLogWriter dataLogWriter;
    FileParser fileParser;
    //auto fileData = fileParser.processFilePathToFileData("C:/Dev/Projekty/NeuralNetworks_CPP_Base/NeuralNetwork_CPP_Base/semeion.data/semeion.data");
    auto fileData = fileParser.processFilePathToFileData("D:/Projekty/Unity/TripleEspresso/NeuralNetwork_CPP_Base/NeuralNetworks_CPP_Base/NeuralNetworks_CPP_Base/Data/semeion.data");
    dataLogWriter.OutputNumberFileData(fileData);

	NeuralNetwork neuralNetwork;
	neuralNetwork.InitializeNeuralNetwork(16 * 16, 64, 10);

	neuralNetwork.ComputeNeuralNetwork(fileData, 0.01f);

}

void CalculateLogicGates()
{
    Matrix inputs = Matrix();
    Matrix outputs = Matrix();
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            inputs.push_back({ (double)i, (double)j });
            outputs.push_back({
                (double)Xor(i, j),
                (double)Xnor(i, j),
                (double)Or(i, j),
                (double)And(i, j),
                (double)Nor(i, j),
                (double)Nand(i, j)
                });
        }
    }

    Trainer trainer = Trainer::Create(2, 2, 6, Rand);
    for (size_t i = 0; i < ITERS; i++) {
        const Vector& input = inputs[i % inputs.size()];
        const Vector& output = outputs[i % outputs.size()];
        trainer.Train(input, output, lr);
    }

    std::cout
        << "Result after "
        << ITERS
        << " iterations\n"
        << "        XOR   XNOR    OR   AND   NOR   NAND\n";
    const Network& network = trainer.network;
    for (size_t i = 0; i < inputs.size(); i++) {
        const Vector& input = inputs[i];
        Vector pred = network.Predict(input);
        std::cout
            << std::fixed
            << std::setprecision(0)
            << input[0]
            << ','
            << input[1]
            << " = "
            << std::setprecision(3)
            << pred[0]
            << "  "
            << pred[1]
            << " "
            << pred[2]
            << " "
            << pred[3]
            << " "
            << pred[4]
            << "  "
            << pred[5]
            << '\n';
    }

    show_weights(trainer.network);
}

void show_weights(const Network& network) {
    std::cout << "WeightsHidden:\n" << std::setprecision(6);
    for (size_t i = 0; i < network.inputCount; i++) {
        for (size_t j = 0; j < network.hiddenCount; j++) {
            std::cout << network.weightsHidden[network.inputCount * i + j] << ' ';
        }

        std::cout << '\n';
    }

    std::cout << "BiasesHidden:\n";
    for (auto c : network.biasesHidden) {
        std::cout << c << ' ';
    }

    std::cout << "\nWeightsOutput:\n";
    for (size_t i = 0; i < network.hiddenCount; i++) {
        for (size_t j = 0; j < network.outputCount; j++) {
            std::cout << network.weightsOutput[network.outputCount * i + j] << ' ';
        }

        std::cout << '\n';
    }

    std::cout << "BiasesOutput:\n";
    for (auto c : network.biasesOutput) {
        std::cout << c << ' ';
    }

    std::cout << '\n';
}

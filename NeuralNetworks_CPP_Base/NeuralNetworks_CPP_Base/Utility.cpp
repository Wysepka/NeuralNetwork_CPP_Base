#include "Utility.h"

std::vector<SingleNumberData> Utility::GetRandomSingleNumbers(const std::vector<SingleNumberData>& inputVector, size_t count)
{
	std::vector<SingleNumberData> result;

	if (inputVector.size() <= count) {
		// If the input vector has 10 or fewer elements, return the whole vector
		return inputVector;
	}

	std::vector<int> indices(inputVector.size());
	std::iota(indices.begin(), indices.end(), 0); // Fill with 0, 1, ..., inputVector.size()-1

	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(indices.begin(), indices.end(), gen); // Shuffle the indices

	for (size_t i = 0; i < count; ++i) {
		result.push_back(inputVector[indices[i]]);
	}

	return result;
}

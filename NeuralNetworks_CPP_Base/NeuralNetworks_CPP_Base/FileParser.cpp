#include "FileParser.h"
#include "ctype.h"

std::string FileParser::readFileToString(const std::string& filePath)
{
    std::ifstream fileStream(filePath); // Create an input file stream
    if (!fileStream.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::stringstream buffer;
    buffer << fileStream.rdbuf(); // Read the file into the stringstream buffer

    return buffer.str(); // Convert the buffer into a string
}

NumberFileData FileParser::processFilePathToFileData(const std::string& filePath)
{
    NumberFileData NumberFileDatas;
    std::ifstream file(filePath);
    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        return NumberFileData();
    }

    // Vector to store the rows
    std::vector<std::string> rows;
    std::string line;

    // Read the file line by line
    while (std::getline(file, line)) {
        rows.push_back(line);
        SingleNumberData numberData;
        int numberCounter = 0;
        std::string singleRowContainer;
        for (size_t i = 0; i < line.length(); i++)
        {
            if (numberCounter > 0 && numberCounter % 16 == 0) {
                numberData.rowData.push_back(singleRowContainer);
                singleRowContainer.clear();
            }
            if (line[i] == int('.')) {
                numberCounter++;
                if (line[i - 1] == int('1')) {
                    singleRowContainer.push_back('1');
                }
                else if (line[i - 1] == int('0')) {
                    singleRowContainer.push_back('0');
                }
            }
        }
    }


    return NumberFileDatas;
}

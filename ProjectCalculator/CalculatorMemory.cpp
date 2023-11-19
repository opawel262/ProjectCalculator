#include "CalculatorMemory.h"
#include <fstream>

std::vector<std::string> CalculatorMemory::getVectorMemory()
{
    return this->vectorMemory;
}

CalculatorMemory::CalculatorMemory()
{
	this->readMemoryFromFile();
}

void CalculatorMemory::readMemoryFromFile()
{
    std::ifstream file("history.txt");
    std::string line;
    while (std::getline(file, line)) {
        this->vectorMemory.push_back(line);
    }
    file.close();
}

void CalculatorMemory::saveMemoryToFile(const std::string& operation )
{
	std::ofstream file("history.txt", std::ios::app);
	file << operation << std::endl;
	file.close();

}

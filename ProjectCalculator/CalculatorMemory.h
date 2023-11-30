#pragma once
#include <string>
#include <vector>
class CalculatorMemory
{
public:
	std::vector<std::string> getVectorMemory();
	void saveMemoryToFile(const std::string& operation);
	void deleteMemory();
	CalculatorMemory();
	
private:
	std::vector<std::string> vectorMemory;
	void readMemoryFromFile();

	
};


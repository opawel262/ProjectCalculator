#pragma once
#include <string>
#include "CalculatorMemory.h"
class MyCalculator : public CalculatorMemory
{
private:

	bool isSymbol(char c);
	int precedence(char c);
	std::string workWithMinus(const std::string& infix);
	std::string inToPost(const std::string& infix);
	long double evaluatePostFix(const std::string& postfix);
	std::string changeFunctionsToStringNumber(const std::string& stringToChange);
	long double calculateString(const std::string& stringToCalculate);

public:
	MyCalculator();
	bool isValidString(const std::string& str);
	bool isValidCharacter(char c);
	std::string infix;
	long double calculateString();
	bool isError;

};


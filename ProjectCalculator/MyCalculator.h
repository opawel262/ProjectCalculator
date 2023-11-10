#pragma once
#include <string>
class MyCalculator
{
private:
	bool isSymbol(char c);
	int precedence(char c);
	std::string workWithMinus(std::string infix);
	std::string inToPost(const std::string& infix);
	double evaluatePostFix(std::string postfix);
public:
	std::string infix;
	double calculateString();
	void addChar(const char c);
};


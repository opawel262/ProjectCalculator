﻿#include "MyCalculator.h"
#include <stack>
#include <cmath>
const std::string PI = "3.1415926535";
const std::string E = "2.7182818289";

bool MyCalculator::isSymbol(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^') return true;
    else return false;
}

int MyCalculator::precedence(char c)
{
    if (c == '^') return 3;
    else if (c == '%' || c == '*' || c == '/') return 2;
    else if (c == '+' || c == '-') return 1;
    else return 0;
}

MyCalculator::MyCalculator()
{
    this->isError = false;
}

bool MyCalculator::isValidCharacter(char c)
{
    return (c >= '0' && c <= '9') || c == '.' || c == '(' || c == ')' ||
        c == ' ' || c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';

}
bool MyCalculator::isValidString(const std::string& str)
{
    for (char c : str) {
        if (!isValidCharacter(c)) {
            return false;
        }
    }
    return true;

}


std::string MyCalculator::workWithMinus(const std::string& infix)
{
    if (this->isError) {
        return "0";
    }
    std::string infixEdit = infix;
    std::string formatted = "";
    for (size_t i = 0; i < infixEdit.size(); i++) {
        if (infixEdit[i] == '-' && !isSymbol(infixEdit[i + 1]) && infixEdit[i + 1] != ' ') {
            formatted += "(0 - ";
            i++;
            formatted += infixEdit[i];
            while (i + 1 < infixEdit.size() && infixEdit[i + 1] != ' ') {
                formatted += infixEdit[i + 1];
                i++;
            }
            formatted.append(")");
        }
        else {
            formatted += infixEdit[i];
        }
    }
    return formatted;
}


std::string MyCalculator::inToPost(const std::string& infix)
{
    std::string infixEdit = infix;
    std::string postfix;
    std::stack<char> stack;
    bool isHaveAnyOperator = true;

    for (int i = 0; i < infixEdit.size(); i++) {
        if (infixEdit[i] == ' ') {
            continue;
        }
        else if (infixEdit[i] == '(') {
            stack.push(infixEdit[i]);
        }
        else if (infixEdit[i] == ')') {
            while (!stack.empty() && stack.top() != '(') {
                postfix += " ";
                postfix += stack.top();
                stack.pop();
            }
            if (!stack.empty()) {
                stack.pop(); // Pop the '('
            }
        }
        else if (!isSymbol(infixEdit[i])) {
            // Obsługa liczb zmiennoprzecinkowych i białych znaków
            std::string number = "";
            while (i < infixEdit.size() && (std::isdigit(infixEdit[i]) || infixEdit[i] == '.')) {
                number += infixEdit[i];
                i++;
            }
            i--; // Zmniejsz indeks, aby uwzględnić ostatni znak liczby
            postfix += " " + number;
        }
        else if (isSymbol(infixEdit[i])) {
            isHaveAnyOperator = false;
            while (!stack.empty() && stack.top() != '(' && precedence(stack.top()) >= precedence(infixEdit[i])) {
                postfix += " ";
                postfix += stack.top();
                stack.pop();
            }
            stack.push(infixEdit[i]);
        }
    }
    if (isHaveAnyOperator) {
        postfix.append(" "); postfix.append("0 "); postfix.append("+");
    }

    while (!stack.empty()) {
        postfix += " ";
        postfix += stack.top();
        stack.pop();
    }

    return postfix;
}


long double MyCalculator::evaluatePostFix(const std::string& postfix)
{
    if (this->isError) {
        return 0.0;
    }
    std::stack<long double> stack;
    std::string varNumStringToDouble = "";
    long double l1, l2;
    for (int i = 0; i < postfix.size(); i++) {
        if (postfix[i] >= '0' && postfix[i] <= '9') {
            while (postfix[i] != ' ') {
                varNumStringToDouble += postfix[i];
                i++;
            }
            stack.push(std::stod(varNumStringToDouble));
            i--;
        }
        else if (postfix[i] == ' ') {
            varNumStringToDouble = "";
        }
        else if (isSymbol(postfix[i])) {
            if (!stack.empty()) {
                l1 = stack.top();
                stack.pop();
                if (stack.empty()) {
                    this->isError = true;
                    return 0.0;
                }
                l2 = stack.top();
                if (!stack.empty()) {
                    stack.pop();

                }
                switch (postfix[i]) {
                case '+':
                    stack.push(l2 + l1);
                    break;

                case '-':
                    stack.push(l2 - l1);
                    break;
                case '*':
                    stack.push(l2 * l1);
                    break;
                case '/':
                    stack.push(l2 / l1);
                    break;
                case '%':
                    stack.push(fmod(l2, l1));
                    break;
                case '^':
                    stack.push(pow(l2, l1));
                    break;
                    //std::cout << "cos sie popsulo w eval_postfix";
                }
            }
        }
    }
    return stack.top();
}

std::string MyCalculator::changeFunctionsToStringNumber(const std::string& stringToChange)
{
    std::string string_t = stringToChange; // "sin(30) + 20 + cos(20 + 30 + sin(30))
    if (string_t.find("sin(") != std::string::npos || string_t.find("sqrt(") != std::string::npos ||
        string_t.find("tan(") != std::string::npos || string_t.find("cos(") != std::string::npos
        || string_t.find("exp(") != std::string::npos || string_t.find("e") != std::string::npos
        || string_t.find("PI") != std::string::npos) {
        std::string string_tEdited = "";
        size_t string_t_size = string_t.size();
        for (size_t i = 0; i < string_t_size; i++) {
            if (string_t_size >= 2) {
                if (i == 0) {
                    if (string_t[i] == 'e' && string_t[i + 1] != 'x') {
                        string_tEdited.append(E);
                        i += 1;
                    }
                    else if (string_t[i] == 'P' && string_t[i + 1] == 'I') {
                        string_tEdited.append(PI);
                        i += 2;
                    }
                }
                else if (i >= 1 && (string_t[i - 1] == ' ' || this->isSymbol(string_t[i - 1]))
                    && string_t[i] == 'e' && string_t[i + 1] != 'x') {
                    string_tEdited.append(E);
                    i += 1;
                }
                else if (i >= 1 && (string_t[i - 1] == ' ' || this->isSymbol(string_t[i])) &&
                    string_t[i] == 'P' && string_t[i + 1] == 'I') {
                    string_tEdited.append(PI);
                    i += 2;
                }
            }
            if (string_t_size >= 4) {
                if (string_t[i] == 's' && string_t[i + 1] == 'i' && string_t[i + 2] == 'n' && string_t[i + 3] == '(') {
                    i += 3;
                    int counter = 0;
                    std::string string_e; // potrzebujemy to zeby wyciagnac liczby i zapmietac je z funkcji
                    while (1) {
                        if (string_t[i] == '(') {
                            counter++;
                        }
                        else if (string_t[i] == ')') {
                            counter--;
                        }
                        if (string_t[i] == ')' && counter == 0) {
                            break;
                        }
                        if (i == string_t_size - 1 && counter > 0) {
                            this->isError = true;
                            break;
                        }
                        string_e += string_t[i];
                        i++;
                    }
                    long double calculatedNumber = this->calculateString(string_e);
                    long double toDegrees = calculatedNumber * (3.14 / 180.0);
                    std::string stringCalculatedNumber = std::to_string(sin(toDegrees));
                    string_tEdited.append(stringCalculatedNumber);
                    if (string_t_size - 1 == i) break;
                    i++;

                }
                if (string_t[i] == 'c' && string_t[i + 1] == 'o' && string_t[i + 2] == 's' && string_t[i + 3] == '(') {
                    i += 3;
                    int counter = 0;
                    std::string string_e; // potrzebujemy to zeby wyciagnac liczby i zapmietac je z funkcji
                    while (1) {
                        if (string_t[i] == '(') {
                            counter++;
                        }
                        else if (string_t[i] == ')') {
                            counter--;
                        }
                        if (string_t[i] == ')' && counter == 0) {
                            break;
                        }
                        if (i == string_t_size - 1 && counter > 0) {
                            this->isError = true;
                        }
                        string_e += string_t[i];
                        i++;
                    }
                    long double calculatedNumber = this->calculateString(string_e);
                    long double toDegrees = calculatedNumber * (3.14 / 180.0);
                    std::string stringCalculatedNumber = std::to_string(cos(toDegrees));
                    string_tEdited.append(stringCalculatedNumber);
                    if (string_t_size - 1 == i) break;
                    i++;

                }
                if (string_t[i] == 't' && string_t[i + 1] == 'a' && string_t[i + 2] == 'n' && string_t[i + 3] == '(') {
                    i += 3;
                    int counter = 0;
                    std::string string_e; // potrzebujemy to zeby wyciagnac liczby i zapmietac je z funkcji
                    while (1) {
                        if (string_t[i] == '(') {
                            counter++;
                        }
                        else if (string_t[i] == ')') {
                            counter--;
                        }
                        if (string_t[i] == ')' && counter == 0) {
                            break;
                        }
                        if (i == string_t_size - 1 && counter > 0) {
                            this->isError = true;
                        }
                        string_e += string_t[i];
                        i++;
                    }
                    long double calculatedNumber = this->calculateString(string_e);
                    long double toDegrees = calculatedNumber * (3.14 / 180.0);
                    std::string stringCalculatedNumber = std::to_string(tan(toDegrees));
                    string_tEdited.append(stringCalculatedNumber);
                    if (string_t_size - 1 == i) break;
                    i++;

                }
                if (string_t[i] == 'e' && string_t[i + 1] == 'x' && string_t[i + 2] == 'p' && string_t[i + 3] == '(') {
                    i += 3;
                    int counter = 0;
                    std::string string_e; // potrzebujemy to zeby wyciagnac liczby i zapmietac je z funkcji
                    while (1) {
                        if (string_t[i] == '(') {
                            counter++;
                        }
                        else if (string_t[i] == ')') {
                            counter--;
                        }
                        if (string_t[i] == ')' && counter == 0) {
                            break;
                        }
                        if (i == string_t_size - 1 && counter > 0) {
                            this->isError = true;
                            break;
                        }
                        string_e += string_t[i];
                        i++;
                    }
                    long double calculatedNumber = this->calculateString(string_e);
                    std::string stringCalculatedNumber = std::to_string(exp(calculatedNumber));
                    string_tEdited.append(stringCalculatedNumber);
                    if (string_t_size - 1 == i) break;
                    i++;

                }
            }
            if (string_t_size >= 5) {
                if (string_t[i] == 's' && string_t[i + 1] == 'q' && string_t[i + 2] == 'r'
                    && string_t[i + 3] == 't' && string_t[i + 4] == '(') {
                    i += 4;
                    int counter = 0;
                    std::string string_e; // potrzebujemy to zeby wyciagnac liczby i zapmietac je z z srodka nawiasow funkcji
                    while (1) {
                        if (string_t[i] == '(') {
                            counter++;
                        }
                        else if (string_t[i] == ')') {
                            counter--;
                        }
                        if (string_t[i] == ')' && counter == 0) {
                            break;
                        }
                        if (i == string_t_size - 1 && counter > 0) {
                            this->isError = true;
                            break;
                        }
                        string_e += string_t[i];
                        i++;
                    }
                    long double calculatedNumber = this->calculateString(string_e);
                    std::string stringCalculatedNumber = std::to_string(sqrt(calculatedNumber));
                    string_tEdited.append(stringCalculatedNumber);
                    if (string_t_size - 1 == i) break;
                    i++;

                }
            }
            if (this->isValidCharacter(string_t[i])) {
                string_tEdited += string_t[i];

            }
            else if (string_t[i] == '\0');
            else {
                this->isError = true;
            }
        }
        return string_tEdited;
    }
    if (this->isValidString(stringToChange)) {
        return stringToChange;
    }
    else {
        this->isError = true;
        return "0";
    }

}

long double MyCalculator::calculateString()
{

    return this->evaluatePostFix(this->inToPost(
        this->workWithMinus(
            this->changeFunctionsToStringNumber(this->infix))));



}

long double MyCalculator::calculateString(const std::string& stringToCalculate)
{

    return this->evaluatePostFix(this->inToPost(
        this->workWithMinus(
            this->changeFunctionsToStringNumber(stringToCalculate))));


}

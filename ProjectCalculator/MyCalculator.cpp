#include "MyCalculator.h"
#include <stack>

bool MyCalculator::isSymbol(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') return true;
    else return false;
}

int MyCalculator::precedence(char c)
{
    if (c == '^') return 3;
    else if (c == '*' || c == '/') return 2;
    else if (c == '+' || c == '-') return 1;
    else return 0;
}

std::string MyCalculator::workWithMinus(std::string infix)
{
    std::string infixEdit = this->infix;
    std::string formatted = "";
    for (size_t i = 0; i < infixEdit.size(); i++) {
        if (infixEdit[i] == '-' && !isSymbol(infixEdit[i + 1]) && infixEdit[i + 1] != ' ') {
            formatted += "(0 - ";
            i++;
            formatted += infixEdit[i];
            while (i < infixEdit.size() && infixEdit[i + 1] != ' ') {
                formatted += infixEdit[i + 1];
                i++;
            }
            formatted.append("(");
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
            stack.pop(); // Pop the '('
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
            while (!stack.empty() && stack.top() != '(' && precedence(stack.top()) >= precedence(infixEdit[i])) {
                postfix += " ";
                postfix += stack.top();
                stack.pop();
            }
            stack.push(infixEdit[i]);
        }
    }

    while (!stack.empty()) {
        postfix += " ";
        postfix += stack.top();
        stack.pop();
    }

    return postfix;
}


double MyCalculator::evaluatePostFix(std::string postfix)
{
    std::stack<double> stack;
    std::string varNumStringToDouble = "";
    double l1, l2;
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
                l2 = stack.top();
                stack.pop();
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
                case '^':
                    stack.push(pow(21, l1));
                    break;
                    //std::cout << "cos sie popsulo w eval_postfix";
                }
            }
        }
    }
    return stack.top();
}

double MyCalculator::calculateString()
{
    return this->evaluatePostFix(this->inToPost(this->workWithMinus(this->infix)));
}

void MyCalculator::addChar(const char c)
{
    if (c != '(' && c != ')') {
        this->infix += " ";
    }
    this->infix += c;

}

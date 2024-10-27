#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <cmath>
using namespace std;

template<typename T>
class Stack {
    vector<T> elements;

public:
    void push(T const& elem) {
        elements.push_back(elem);
    }

    T pop() {
        if (elements.empty()) throw out_of_range("Stack<>::pop(): empty stack");
        T elem = elements.back();
        elements.pop_back();
        return elem;
    }

    T peek() const {
        if (elements.empty()) throw out_of_range("Stack<>::peek(): empty stack");
        return elements.back();
    }

    bool empty() const {
        return elements.empty();
    }

    int size() const {
        return elements.size();
    }
};

class Calculator {
    Stack<string> stack;

public:
    bool isOperator(char c) {
        return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^');
    }

    int precedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        if (op == '%') return 2;
        if (op == '^') return 3;  // highest precedence
        return 0;
    }

    string infixToPostfix(string infix) {
        if (!isValidInfix(infix)) {
            return "Invalid expression";
        }

        stringstream result;
        string operand;
        for (int i = 0; i < infix.length(); i++) {
            char c = infix[i];
            if (isspace(c)) {
                if (!operand.empty()) {
                    result << operand << " ";
                    operand.clear();
                }
                continue;
            }

            if (isdigit(c)) {
                operand += c;  // accumulate digits
            } else {
                if (!operand.empty()) {
                    result << operand << " ";
                    operand.clear();
                }
                if (c == '(') {
                    stack.push(string(1, c));
                } else if (c == ')') {
                    while (!stack.empty() && stack.peek() != "(") {
                        result << stack.pop() << " ";
                    }
                    if (!stack.empty()) {
                        stack.pop();  // pop '('
                    }
                } else {  // operator
                    while (!stack.empty() && precedence(stack.peek()[0]) >= precedence(c)) {
                        result << stack.pop() << " ";
                    }
                    stack.push(string(1, c));
                }
            }
        }
        if (!operand.empty()) {
            result << operand << " ";
        }
        while (!stack.empty()) {
            result << stack.pop() << " ";
        }
        return result.str();
    }

    string infixToPrefix(string infix) {
        if (!isValidInfix(infix)) {
            return "Invalid expression";
        }

        infix = reverseString(infix);  // reverse for prefix conversion
        for (char &c : infix) {
            if (c == '(') c = ')';
            else if (c == ')') c = '(';  // swap parentheses
        }

        string postfix = infixToPostfix(infix);
        return reverseString(postfix);  // reverse postfix for prefix
    }

    string postfixToInfix(string postfix) {
        if (!isValidPostfix(postfix)) {
            return "Invalid expression";
        }

        Stack<string> s;
        stringstream ss(postfix);
        string token;

        while (ss >> token) {
            if (isalnum(token[0])) {
                s.push(token);
            } else {
                string operand2 = s.pop();
                string operand1 = s.pop();
                string expr = "(" + operand1 + " " + token + " " + operand2 + ")";
                s.push(expr);
            }
        }
        return s.pop();
    }

    string postfixToPrefix(string postfix) {
        if (!isValidPostfix(postfix)) {
            return "Invalid expression";
        }

        Stack<string> s;
        stringstream ss(postfix);
        string token;

        while (ss >> token) {
            if (isalnum(token[0])) {
                s.push(token);
            } else {
                string operand2 = s.pop();
                string operand1 = s.pop();
                string expr = token + " " + operand1 + " " + operand2;
                s.push(expr);
            }
        }
        return s.pop();
    }

    string prefixToInfix(string prefix) {
        if (!isValidPrefix(prefix)) {
            return "Invalid expression";
        }

        Stack<string> s;
        stringstream ss(prefix);
        vector<string> tokens;
        string token;

        while (ss >> token) {
            tokens.push_back(token);
        }

        for (int i = tokens.size() - 1; i >= 0; i--) {
            if (isalnum(tokens[i][0])) {
                s.push(tokens[i]);
            } else {
                string operand1 = s.pop();
                string operand2 = s.pop();
                string expr = "(" + operand1 + " " + tokens[i] + " " + operand2 + ")";
                s.push(expr);
            }
        }
        return s.pop();
    }

    string prefixToPostfix(string prefix) {
        if (!isValidPrefix(prefix)) {
            return "Invalid expression";
        }

        Stack<string> s;
        stringstream ss(prefix);
        vector<string> tokens;
        string token;

        while (ss >> token) {
            tokens.push_back(token);
        }

        for (int i = tokens.size() - 1; i >= 0; i--) {
            if (isalnum(tokens[i][0])) {
                s.push(tokens[i]);
            } else {
                string operand1 = s.pop();
                string operand2 = s.pop();
                string expr = operand1 + " " + operand2 + " " + tokens[i];
                s.push(expr);
            }
        }
        return s.pop();
    }

    bool isValidInfix(string infix) {
        int operandCount = 0, operatorCount = 0;
        bool lastWasOperand = false;
        int parenthesesBalance = 0;

        for (int i = 0; i < infix.length(); i++) {
            char c = infix[i];
            if (isspace(c)) continue;

            if (isdigit(c)) {
                operandCount++;
                lastWasOperand = true;
                while (i + 1 < infix.length() && isdigit(infix[i + 1])) i++;  // skip multi-digit
            } else if (isOperator(c)) {
                operatorCount++;
                if (!lastWasOperand) return false;  // ensure valid operand before operator
                lastWasOperand = false;
            } else if (c == '(') {
                parenthesesBalance++;
                lastWasOperand = false;
            } else if (c == ')') {
                parenthesesBalance--;
                if (parenthesesBalance < 0) return false;  // unmatched ')'
                lastWasOperand = true;
            } else return false;  // invalid character
        }
        return (parenthesesBalance == 0 && operandCount == operatorCount + 1);
    }

    bool isValidPostfix(string postfix) {
        Stack<int> s;
        stringstream ss(postfix);
        string token;

        while (ss >> token) {
            if (isspace(token[0])) continue;
            if (isalnum(token[0])) {
                s.push(1);
            } else if (isOperator(token[0])) {
                if (s.size() < 2) return false;  // ensure two operands for operator
                s.pop();
                s.pop();
                s.push(1);
            } else return false;  // invalid character
        }
        return s.size() == 1;
    }

    bool isValidPrefix(string prefix) {
        Stack<int> s;
        stringstream ss(prefix);
        vector<string> tokens;
        string token;

        while (ss >> token) {
            tokens.push_back(token);
        }

        for (int i = tokens.size() - 1; i >= 0; i--) {
            if (isspace(tokens[i][0])) continue;
            if (isalnum(tokens[i][0])) {
                s.push(1);
            } else if (isOperator(tokens[i][0])) {
                if (s.size() < 2) return false;  // ensure two operands for operator
                s.pop();
                s.pop();
                s.push(1);
            } else return false;  // invalid character
        }
        return s.size() == 1;
    }

    int evaluatePostfix(string postfix) {
        Stack<int> s;
        stringstream ss(postfix);
        string token;

        while (ss >> token) {
            if (isdigit(token[0])) {
                s.push(stoi(token));  // convert string to int
            } else {
                int operand2 = s.pop();
                int operand1 = s.pop();
                switch (token[0]) {
                    case '+': s.push(operand1 + operand2); break;
                    case '-': s.push(operand1 - operand2); break;
                    case '*': s.push(operand1 * operand2); break;
                    case '/':
                        if (operand2 == 0) throw runtime_error("Division by zero");
                        s.push(operand1 / operand2); 
                        break;
                    case '%': s.push(operand1 % operand2); break;
                    case '^': s.push(pow(operand1, operand2)); break;  // assume `pow` is included
                }
            }
        }
        return s.pop();
    }

    int evaluatePrefix(string prefix) {
        Stack<int> s;
        stringstream ss(prefix);
        vector<string> tokens;
        string token;

        while (ss >> token) {
            tokens.push_back(token);
        }

        for (int i = tokens.size() - 1; i >= 0; i--) {
            if (isdigit(tokens[i][0])) {
                s.push(stoi(tokens[i]));  // convert string to int
            } else {
                int operand1 = s.pop();
                int operand2 = s.pop();
                switch (tokens[i][0]) {
                    case '+': s.push(operand1 + operand2); break;
                    case '-': s.push(operand1 - operand2); break;
                    case '*': s.push(operand1 * operand2); break;
                    case '/':
                        if (operand2 == 0) throw runtime_error("Division by zero");
                        s.push(operand1 / operand2);
                        break;
                    case '%': s.push(operand1 % operand2); break;
                    case '^': s.push(pow(operand1, operand2)); break;  // assume `pow` is included
                }
            }
        }
        return s.pop();
    }
    int evaluateInfix(string infix){
        string postfix = infixToPostfix(infix);
        return evaluatePostfix(postfix);
    }
    string reverseString(const string &str) {
        return string(str.rbegin(), str.rend());
    }
};

int main() {
    Calculator calc;

    string infix = "((10 + 2) ^ (5 - 2))";
    cout << "Infix to Postfix: " << calc.infixToPostfix(infix) << endl;
    cout << "Infix to Prefix: " << calc.infixToPrefix(infix) << endl;

    string postfix = calc.infixToPostfix(infix);
    cout << "Postfix to Infix: " << calc.postfixToInfix(postfix) << endl;
    cout << "Postfix to Prefix: " << calc.postfixToPrefix(postfix) << endl;

    string prefix = calc.infixToPrefix(infix);
    cout << "Prefix to Infix: " << calc.prefixToInfix(prefix) << endl;
    cout << "Prefix to Postfix: " << calc.prefixToPostfix(prefix) << endl;

    cout << "Evaluate Postfix: " << calc.evaluatePostfix(postfix) << endl;
    cout << "Evaluate Prefix: " << calc.evaluatePrefix(prefix) << endl;

    cout << "\n\n Evaluate Infix: " << calc.evaluateInfix(infix) << endl;

    string n = "2 + 4 * 10";
    cout << calc.evaluateInfix(n) << endl;
    return 0;
}

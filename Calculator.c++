#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <cctype>
#include <cmath>
#include <stdexcept>

class Calculator {
public:
    double evaluate(const std::string& expression) {
        std::istringstream iss(expression);
        std::stack<double> values;
        std::stack<char> operators;
        double value = 0.0;
        char op = ' ';
        bool hasValue = false;

        while (iss >> std::ws && !iss.eof()) {
            if (std::isdigit(iss.peek())) {
                iss >> value;
                values.push(value);
                hasValue = true;
            } else if (iss.peek() == '(') {
                operators.push('(');
                iss.get(); // Consume '('
            } else if (iss.peek() == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    applyOperator(values, operators);
                }
                if (!operators.empty() && operators.top() == '(') {
                    operators.pop(); // Pop '('
                }
                iss.get(); // Consume ')'
            } else {
                if (hasValue) {
                    hasValue = false;
                } else {
                    value = 0.0; // Handle unary operators
                }

                if (iss.peek() == '+' || iss.peek() == '-' || iss.peek() == '*' || iss.peek() == '/') {
                    op = iss.get();
                    while (!operators.empty() && precedence(operators.top()) >= precedence(op)) {
                        applyOperator(values, operators);
                    }
                    operators.push(op);
                } else if (iss.peek() == '^') {
                    op = iss.get();
                    while (!operators.empty() && precedence(operators.top()) > precedence(op)) {
                        applyOperator(values, operators);
                    }
                    operators.push(op);
                } else if (iss.peek() == 's' || iss.peek() == 'c' || iss.peek() == 't' || iss.peek() == 'q') {
                    std::string func;
                    iss >> func;
                    if (func == "sin") {
                        values.push(sin(values.top()));
                        values.pop();
                    } else if (func == "cos") {
                        values.push(cos(values.top()));
                        values.pop();
                    } else if (func == "tan") {
                        values.push(tan(values.top()));
                        values.pop();
                    } else if (func == "sqrt") {
                        values.push(sqrt(values.top()));
                        values.pop();
                    } else {
                        throw std::runtime_error("Unknown function: " + func);
                    }
                } else {
                    throw std::runtime_error("Invalid character in expression");
                }
            }
        }

        while (!operators.empty()) {
            applyOperator(values, operators);
        }

        if (values.size() != 1) {
            throw std::runtime_error("Invalid expression");
        }

        return values.top();
    }

private:
    int precedence(char op) {
        switch (op) {
            case '+':
            case '-':
                return 1;
            case '*':
            case '/':
                return 2;
            case '^':
                return 3;
            default:
                return 0;
        }
    }

    void applyOperator(std::stack<double>& values, std::stack<char>& operators) {
        if (values.size() < 2 || operators.empty()) {
            throw std::runtime_error("Invalid expression");
        }

        double right = values.top();
        values.pop();
        double left = values.top();
        values.pop();

        char op = operators.top();
        operators.pop();

        switch (op) {
            case '+':
                values.push(left + right);
                break;
            case '-':
                values.push(left - right);
                break;
            case '*':
                values.push(left * right);
                break;
            case '/':
                if (right == 0) {
                    throw std::runtime_error("Division by zero");
                }
                values.push(left / right);
                break;
            case '^':
                values.push(pow(left, right));
                break;
            default:
                throw std::runtime_error("Invalid operator");
        }
    }
};

int main() {
    Calculator calc;
    std::string expression;

    std::cout << "Enter an expression to evaluate (or 'exit' to quit): ";
    while (std::getline(std::cin, expression)) {
        if (expression == "exit") {
            break;
        }

        try {
            double result = calc.evaluate(expression);
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        std::cout << "Enter another expression (or 'exit' to quit): ";
    }

    return 0;
}

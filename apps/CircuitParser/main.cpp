#include <iostream>
#include <stack>
#include <sstream>

#include "common.h"

#define MAX_DIGITS 10
bool checkParentheses(const std::string &input)
{
    std::stack<char> mystack;

    int length = input.length();
    for (int i = 0; i < length; i++)
    {
        if (input[i] == '(' || input[i] == '[' || input[i] == '{')
            mystack.push(input[i]);
        else if (input[i] == ')' || input[i] == ']' || input[i] == '}')
        {
            if (mystack.empty())
                return false;
            else
            {
                char top = mystack.top();
                mystack.pop();

                if ((input[i] == ')' && top != '(') ||
                        (input[i] == ']' && top != '[') ||
                        (input[i] == '}' && top != '{'))
                    return false;
            }
        }

    }

    if (mystack.empty())
        return true;
    else return false;
}

inline bool isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

inline bool isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

double calc(double op1, double op2, char c, bool &ok)
{
    ok = true;

    switch (c)
    {
    case '+':
        return op1 + op2;

    case '-':
        return op1 - op2;

    case '*':
        return op1 * op2;

    case '/':
        if (op2 == 0)
        {
            ok = false;
            return 0;
        }
        else return op1 / op2;

    default:
        ok = false;
        return 0;
    }
}


double calcPosfix(const std::string &expression, bool &ok)
{
    std::stack<double> mystack;
    std::stringstream sequence(expression);

    ok = false;
    int length = expression.length();

    int i = 0;
    char tmp = 0;

    while (i < length && i >= 0)
    {
        char c = expression[i];
        mDebug << "Current char: " << c << "\n";

        if (c == ' ')
        {
            sequence.get(tmp);
            i = sequence.tellg();


            mDebug << "Space " << sequence.good() << sequence.fail() << sequence.eof() << "\n";
            mDebug << "Space, next = " << i << "\n\n";

            continue; // ignore space
        }

        if (isDigit(c))
        {
            double operand = 0;

            sequence >> operand;
            i = sequence.tellg(); // update current position


            mDebug << sequence.good() << sequence.fail() << sequence.bad() << "\n";
            mDebug << "Digit, num = " << operand << "  next = " << i << "\n";


            if (sequence.bad())
            {
                mInfo << "The expression contains an invalid number\n";
                return 0;
            }

            // push into stack
            mystack.push(operand);
            mInfo << "Num = " << operand << "\n\n";

            continue;
        }

        if (isOperator(c))
        {
            if (mystack.size() < 2)
                return 0;

            double op2 = mystack.top();
            mystack.pop();

            double op1 = mystack.top();
            mystack.pop();

            bool isValid = false;
            double result = calc(op1, op2, c, isValid);
            if (!isValid)
            {
                mInfo << "Division by zero\n";
                return 0;
            }

            mystack.push(result);

            sequence.get(tmp);
            i = sequence.tellg();


            mDebug << "Operator, next = " << i << "\n\n";
            mDebug << "Operator " << c << "\n\n";
            continue;
        }

        mInfo << "The expression contains an invalid character\n";
        return 0;
    }

    if (mystack.size() == 1)
    {
        ok = true;
        return mystack.top();
    }
    else
    {
        mInfo << "Stack size = " << mystack.size() << "\n";
        return 0;
    }

}


int main(int argc, char *argv[])
{
    std::string input = "( )(( )){([( )])    }";

    if (checkParentheses(input))
        mInfo << "Parentheses are matching\n";
    else mInfo << "Parentheses are NOT matching\n";

    std::string expression = "5 10 70 - 2 + / 15.6 5 - * 7 *   ";
    mInfo << "Length  = " << expression.length() << "\n";

    mlogger.setLogLevel(LOG_INFO);
    bool ok = false;
    int result = calcPosfix(expression, ok);
    if (ok)
        mInfo << expression << " = " << result << "\n";
    else mInfo << "Error\n" ;
    return 0;
}

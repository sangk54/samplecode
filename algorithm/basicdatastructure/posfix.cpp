 
#include <iostream>
#include <stack>
#include <string>
#include <cstdlib>

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

int calc(int op1, int op2, char c, bool &ok)
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

int calcPosfix(const std::string &expression, bool &ok)
{
    std::stack<int> mystack;
    char str[MAX_DIGITS + 1];

    ok = false;
    int length = expression.length();

    int i = 0;
    while (i < length)
    {
        char c = expression[i];
        if (c ==' ')
        {
            i++;
            continue; // ignore space
        }

        if (isDigit(c))
        {
            int j = 0;
            while (i < length && j < MAX_DIGITS && isDigit(expression[i]))
                str[j++] = expression[i++];

            if (j > MAX_DIGITS)
            {
                std::cout << "The expression contains a big number (more than 10 digits)\n";
                return 0;
            }

            str[j] = '\0';

            // convert to number
            int operand = atoi(str);

            // push into stack
            mystack.push(operand);
            std::cout << "Num = " << operand << "\n";

            continue;
        }

        if (isOperator(c))
        {
            if (mystack.size() < 2)
                return 0;

            int op2 = mystack.top();
            mystack.pop();

            int op1 = mystack.top();
            mystack.pop();

            bool isValid = false;
            int result = calc(op1, op2, c, isValid);
            if (!isValid)
            {
                std::cout << "Division by zero\n";
                return 0;
            }

            mystack.push(result);

            i++;
            continue;
        }

        std::cout << "The expression contains an invalid character\n";
        return 0;
    }

    if (mystack.size() == 1)
    {
        ok = true;
        return mystack.top();
    }
    else return 0;

}

int main(int argc, char *argv[])
{

    std::string input = "( )(( )){([( )])    }";

    if (checkParentheses(input))
        std::cout << "Parentheses are matching\n";
    else std::cout << "Parentheses are NOT matching\n";

    // 5 10 700 - 2 + / 15 5 - * 7   *
    std::string expression = "5 0 + +";

    bool ok = false;
    int result = calcPosfix(expression, ok);
    if (ok)
        std::cout << expression << " = " << result << "\n";
    else std::cout << "Error\n" ;
    return 0;
}

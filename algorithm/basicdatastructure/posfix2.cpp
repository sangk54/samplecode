#include <iostream>
#include <stack>
#include <sstream>


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

//#define DEBUG
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
#ifdef DEBUG
        std::cout << "Current char: " << c << "\n";
#endif
        if (c == ' ')
        {
            sequence.get(tmp);
            i = sequence.tellg();

#ifdef DEBUG
            std::cout << "Space " << sequence.good() << sequence.fail() << sequence.eof() << "\n";
            std::cout << "Space, next = " << i << "\n\n";
#endif
            continue; // ignore space
        }

        if (isDigit(c))
        {
            double operand = 0;

            sequence >> operand;
            i = sequence.tellg(); // update current position

#ifdef DEBUG
            std::cout << sequence.good() << sequence.fail() << sequence.bad() << "\n";
            std::cout << "Digit, num = " << operand << "  next = " << i << "\n";
#endif

            if (sequence.bad())
            {
                std::cout << "The expression contains an invalid number\n";
                return 0;
            }

            // push into stack
            mystack.push(operand);
            std::cout << "Num = " << operand << "\n\n";

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
                std::cout << "Division by zero\n";
                return 0;
            }

            mystack.push(result);

            sequence.get(tmp);
            i = sequence.tellg();

#ifdef DEBUG
            std::cout << "Operator, next = " << i << "\n\n";
#endif
            std::cout << "Operator " << c << "\n\n";
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
    else
    {
        std::cout << "Stack size = " << mystack.size() << "\n";
        return 0;
    }

}

int main(int argc, char *argv[])
{

    std::string input = "( )(( )){([( )])    }";

    if (checkParentheses(input))
        std::cout << "Parentheses are matching\n";
    else std::cout << "Parentheses are NOT matching\n";

    std::string expression = "5a 10 7 - 2 + / 15 5 - * 7 *   ";
    std::cout << "Length  = " << expression.length() << "\n";

    bool ok = false;
    int result = calcPosfix(expression, ok);
    if (ok)
        std::cout << expression << " = " << result << "\n";
    else std::cout << "Error\n" ;
    return 0;
}
 

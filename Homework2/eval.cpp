//
// Created by jdc10 on 1/28/2021.
//
#include <string>
#include <stack>
#include <cassert>
#include <iostream>

using namespace std;

int precedence(const char &opp);
bool infixToPostfix(string infix, string &postfix);
bool evaluatePostfix(const string &postfix, const bool values[]);
int evaluate(string infix, const bool values[], string& postfix, bool& result);

int main() {
    {
        string test;
        assert(infixToPostfix("2| 3", test) && test == "23|");
        assert(infixToPostfix("2|(3|4)", test) && test == "234||");
        assert(infixToPostfix("2|3|4", test) && test == "23|4|");
        assert(infixToPostfix("((9))", test) && test == "9");
        assert(infixToPostfix("5& (3|2)", test));
        assert(test == "532|&");
        assert(infixToPostfix("4  |  !3 & (0&3) ", test));
        assert(test == "43!03&&|");
        assert(infixToPostfix("!4 & !(5|7)", test));
        assert(test == "4!57|!&");
        assert(!infixToPostfix("!", test));
        assert(!infixToPostfix("!!!", test));
        assert(!infixToPostfix("&", test));
        assert(!infixToPostfix("|", test));
        assert(!infixToPostfix("5&|6", test));
        assert(!infixToPostfix("5|6(", test));
        assert(!infixToPostfix("5|6)", test));
        assert(!infixToPostfix("5|6!", test));
        assert(!infixToPostfix("5!|6", test));
        assert(!infixToPostfix("8|", test));
        assert(!infixToPostfix(" &6", test));
        assert(!infixToPostfix("4 5", test));
        assert(!infixToPostfix("01", test));
        assert(!infixToPostfix("()", test));
        assert(!infixToPostfix("()4", test));
        assert(!infixToPostfix("2(9|8)", test));
        assert(!infixToPostfix("2(&8)", test));
        assert(!infixToPostfix("(6&(7|7)", test));
        assert(!infixToPostfix("x+5", test));
        assert(!infixToPostfix("", test));
        assert(!infixToPostfix("R", test));
        assert(!infixToPostfix("asdfasdfasdfasdefas234234", test));
        assert(!infixToPostfix("23232", test));
        assert(!infixToPostfix("(((", test));
        assert(!infixToPostfix(")))", test));
        assert(!infixToPostfix(")", test));
        assert(!infixToPostfix("(", test));
        assert(!infixToPostfix("|", test));
        assert(!infixToPostfix("\\", test));
    }
    {
        bool ba[10] = {
                //  0      1      2      3      4      5      6      7      8      9
                true,  true,  true,  false, false, false, true,  false, true,  false
        };
        assert(evaluatePostfix("23|", ba) == true);
        assert(evaluatePostfix("23|4|", ba) == true);
        assert(evaluatePostfix("234||", ba) == true);
        assert(evaluatePostfix("234||", ba) == true);
        assert(evaluatePostfix("43!03&&|", ba) == false);
        assert(evaluatePostfix("9", ba) == false);
    }

    {
        bool ba[10] = {
                //  0      1      2      3      4      5      6      7      8      9
                true,  true,  true,  false, false, false, true,  false, true,  false
        };
        string pf;
        bool answer;
        assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  answer);
        assert(evaluate("8|", ba, pf, answer) == 1);
        assert(evaluate(" &6", ba, pf, answer) == 1);
        assert(evaluate("4 5", ba, pf, answer) == 1);
        assert(evaluate("01", ba, pf, answer) == 1);
        assert(evaluate("()", ba, pf, answer) == 1);
        assert(evaluate("()4", ba, pf, answer) == 1);
        assert(evaluate("2(9|8)", ba, pf, answer) == 1);
        assert(evaluate("2(&8)", ba, pf, answer) == 1);
        assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
        assert(evaluate("x+5", ba, pf, answer) == 1);
        assert(evaluate("", ba, pf, answer) == 1);
        assert(evaluate("2|3|4", ba, pf, answer) == 0
               &&  pf == "23|4|"  &&  answer);
        assert(evaluate("2|(3|4)", ba, pf, answer) == 0
               &&  pf == "234||"  &&  answer);
        assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
               &&  pf == "43!03&&|"  &&  !answer);
        assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
        ba[2] = false;
        ba[9] = true;
        assert(evaluate("((9))", ba, pf, answer) == 0  &&  pf == "9"  &&  answer);
        assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);
    }

    cout << "tests fin" << endl;
}

// | = 0
// & = 1
// ! = 2
// else: -1
int precedence(const char &opp) {
    switch (opp) {
        case '|':
            return 0;
        case '&':
            return 1;
        case '!':
            return 2;
        default:
            return -1;
    }
}


/// Convert infix logical expression to postfix
/// @param infix The infix expression to start with
/// @param postfix The postfix expression to write to
/// @return 0 if successful, 1 if syntax error. Postfix will be modified in either case.
bool infixToPostfix(string infix, string &postfix) {
    postfix = string();
    stack<char> operatorStack;

    // Return false if infix is empty string
    if (infix.empty()) return false;

    // Remove all spaces to make debugging more reliable
    // and explicitly check that the number of ( and ) are the same
    string infix_no_spaces = string();
    int counter = 0;
    for (int i = 0; i < infix.length(); ++i) {
        char &ch = infix[i];
        if (ch != ' ') infix_no_spaces += ch;
        if (ch == '(') counter++;
        if (ch == ')') counter--;
    }
    if (counter != 0) return false;
    infix = infix_no_spaces;

    // operand: 0-9
    // binary operator: &, |
    // unitary operator: !
    for (int i = 0; i < infix.length(); ++i) {
        bool flag = false;
        char &ch = infix[i];
        char &ch_prev = infix[i - 1];
        char &ch_next = infix[i + 1];
        switch (ch) {
            // Case binary operator
            case '&':
            case '|':
                // Check for errors
                // These cannot be the first operator, be in front of or behind & or |, be in front of ), or be the last char
                if (i == 0 || ch_prev == '&' || ch_prev == '|' || ch_next == '&' ||
                    ch_next == '|' || ch_next == ')' || i == infix.length() - 1)
                    return false;

                // when we should append an operator to the string
                while (!operatorStack.empty() && operatorStack.top() != '(' &&
                       (precedence(ch) <= precedence(operatorStack.top()))) {
                    // Clear the operator stack until a higher precedence operator is found
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                // else store the operator
                operatorStack.push(ch);
                break;

            case '!':
                // Check for errors, cannot be last char, cannot come before ), &, |
                if (i == infix.length() - 1 || ch_next == '&' || ch_next == '|' || ch_next == ')')
                    return false;

                // Case unitary operator
                operatorStack.push(ch);
                break;

            case '(':
                // Check for errors, cannot be last char, cannot come before &, |, cannot come after a digit, must have another ) in the string after
                if (i == infix.length() - 1 || ch_next == '&' || ch_next == '|')
                    return false;
                if (i != 0 && isdigit(ch_prev)) return false;
                flag = false;
                for (int j = i; j < infix.length(); ++j) {
                    if (infix[j] == ')') flag = true;
                }
                if (!flag) return false;

                // Case open paren
                operatorStack.push(ch);
                break;

            case ')':
                // Check for errors, cannot be first char, cannot come after !, |, &, (
                if (i == 0 || ch_prev == '&' || ch_prev == '|' || ch_prev == '(' || ch_prev == '!')
                    return false;
                // cannot come before ! (only evaluate when not last char)
                if (i != infix.length() - 1 && ch_next == '!') return false;
                // pop the operator stack until close paren
                while (!operatorStack.empty() && operatorStack.top() != '(') {
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                // Must have ( waiting in the operator stack, if one is not found then error
                // top of operator stack should be a ( and should not be empty, if not, then error
                if (operatorStack.empty() || operatorStack.top() != '(') return false;

                // Pop off the remaining ( like normal
                operatorStack.pop();

                // Handle the !(X) case, where an ! sets a whole paren
                if (!operatorStack.empty() && operatorStack.top() == '!') {
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                break;

            default:
                // Case operand (digit) or space or bad char
                if (isdigit(ch)) {
                    // Check for errors, cannot come before or after another digit or before a (
                    if ((i != 0 && isdigit(ch_prev)) || (i != infix.length() - 1 && (isdigit(ch_next) || ch_next == '(')))
                        return false;

                    postfix += ch;
                    // Pull of ! if it was directly previous to this char
                    if (!operatorStack.empty() && operatorStack.top() == '!') {
                        postfix += operatorStack.top();
                        operatorStack.pop();
                    }
                } else if (ch == ' ') {
                    // ignore case with space
                } else {
                    // infix must have had a bad char
                    return false;
                }
                break;
        }
    }

    // Clean out the rest of the stack
    while (!operatorStack.empty()) {
        postfix += operatorStack.top();
        operatorStack.pop();
    }

    return true;
}


/// Evaluate the given postfix expression
/// @param postfix A logical postfix expression. Must be syntactically correct, this function provides no guarantees as
///  to the result if fed an incorrect function.
/// @param values A list of 10 values that we can use to convert the numbers given into bools
/// @return The value of the postfix expression, true or false
bool evaluatePostfix(const string &postfix, const bool *values) {
    stack<bool> operandStack;
    for (int i = 0; i < postfix.length(); ++i) {
        char ch = postfix[i];
        int num = postfix[i] - '0';
        if (isdigit(ch)) operandStack.push(values[num]);
        else if (ch == '!') {
            bool operand = operandStack.top();
            operandStack.pop();
            operandStack.push(!operand);
        }
        else if (ch == '&') {
            bool operand2 = operandStack.top();
            operandStack.pop();

            bool operand1 = operandStack.top();
            operandStack.pop();

            operandStack.push(operand1 && operand2);
        } else if (ch == '|') {
            bool operand2 = operandStack.top();
            operandStack.pop();

            bool operand1 = operandStack.top();
            operandStack.pop();
            operandStack.push(operand1 || operand2);
        }
    }
    return operandStack.top();
}


// Evaluates a boolean expression
//   If infix is a syntactically valid infix boolean expression,
//   then set postfix to the postfix form of that expression, set
//   result to the value of the expression (where in that expression,
//   each digit k represents element k of the values array), and
//   return zero.  If infix is not a syntactically valid expression,
//   return 1.  (In that case, postfix may or may not be changed, but
//   result must be unchanged.)
int evaluate(string infix, const bool *values, string &postfix, bool &result) {
    if (!infixToPostfix(infix, postfix)) return 1;

    result = evaluatePostfix(postfix, values);
    return 0;
}

#include "Expression.h"
#include "LargeNumber.h"
#include <stack>
#include <string>
#include <regex>
#include <map>
#include <iostream>
using namespace std;

const map<string, char> func = {{"!",'!'},
                                {"sqrt", 's'},
                                {"sin",  'i'},
                                {"cos",  'c'},
                                {"tan",  't'},
                                {"log",  'g'},
                                {"ln",   'n'},
                                {"exp",  'e'}};

map<string, LargeNumber> vars;

int preference(char op) {
    if(op == '+' || op == '-') return 1;
    if(op == '*' || op == '/') return 2;
    if(op == '^') return 3;
    return 0;
}



bool isOperator(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/' || op == '^' || op == '(' || op == ')' || op == '!';
}

bool isNumber(char c) {
    return c >= '0' && c <= '9';
}

bool isValidNumber(string str) {
    return regex_match(str, regex("-?\\d+\\.?\\d*(e-?\\d+)?"));
}

string removeSpace(string str) {
    string res;
    for(char c : str) {
        if(c != ' ') res += c;
    }
    return res;
}

//convert unary minus sign to '!'
string checkMinusSign(string str) {
    string res;
    for(int i = 0; i < str.length(); i++) {
        if(str[i] == '-') {
            if(i == 0 || str[i - 1] == '(' ) {
                res += '!';
            } else {
                res += '-';
            }
        } else {
            res += str[i];
        }
    }
    return res;
}

bool isValidExpression(string str) {
    str = removeSpace(str);
    str = checkMinusSign(str);
    int bracketCnt = 0;
    for(char c : str) {
        if(c == '(') bracketCnt++;
        else if(c == ')') bracketCnt--;
        if(bracketCnt < 0) return false;
    }
    if(bracketCnt != 0) return false;
    stack<char> operators;
    stack<string> operands;

    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '(') operators.push('(');
        else if (str[i] == ')') {
            while (operators.top() != '(') {
                char op = operators.top();
                if (op == '+' || op == '-' || op == '*' || op == '/' || op == '^') {
                    if(operators.empty()) return false;
                    if(operands.size() < 2) return false;
                    operators.pop();
                    string l1 = operands.top();
                    operands.pop();
                    string l2 = operands.top();
                    operands.pop();
                    operands.push("");
                } else {
                    if(operators.empty()) return false;
                    if(operands.empty()) return false;
                    operators.pop();
                    LargeNumber l1 = operands.top();
                    operands.pop();
                    operands.push("");
                }
            }
            operators.pop();
        } else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '^') {
            while (!operators.empty() && operators.top() != '(') {
                if(operands.size() < 2) return false;
                char op = operators.top();
                operators.pop();
                string l1 = operands.top();
                operands.pop();
                string l2 = operands.top();
                operands.pop();
                operands.push("");
            }
            operators.push(str[i]);
        } else if (str[i] == '!') {
            operators.push('!');
        } else {
            string tmp = "";
            while (i < str.length() && !isOperator(str[i]) ) {
                tmp += str[i++];
            }
            i--;
            if (func.find(tmp) != func.end()) {
                operators.push(func.at(tmp));
            } else if(isValidNumber(tmp)) {
                operands.push("");
            }
            else if(vars.find(tmp) != vars.end()) {
                operands.push("");
            } else {
                string msg = "Invalid/undefined variable/function name: " + tmp;
                cout << msg << endl;
                return false;
            }
        }
    }
    while (!operators.empty()) {
        char op = operators.top();
        if (op == '+' || op == '-' || op == '*' || op == '/' || op == '^') {
            if(operands.size() < 2) return false;
            operators.pop();
            string l1 = operands.top();
            operands.pop();
            string l2 = operands.top();
            operands.pop();
            operands.push("");
        } else {
            if(operands.empty()) return false;
            operators.pop();
            string l1 = operands.top();
            operands.pop();
            operands.push("");
        }
    }
    if(operands.size() != 1) return false;
    return true;
}

bool isValidAssignmentExpression(string str) {
    //return false if '=' is not found or found more than once
    string tmp = str;
    tmp = removeSpace(tmp);
    int equalCnt = 0;
    int equalIdx;
    for(int i = 1; i < tmp.length(); i++)  {
        if(tmp[i] == '=') {
            equalCnt++;
            equalIdx = i;
        }
    }
    if(equalCnt != 1) return false;
    string var = tmp.substr(0, equalIdx);
    string expr = tmp.substr(equalIdx + 1);
    regex varReg("^[a-zA-Z_][a-zA-Z0-9_]*$");
    if(regex_match(var, varReg)) {
        if(isValidExpression(expr)) {
            assignVariable(var, expr);
            return true;
        }
    }
    return false;
}

LargeNumber evaluateExpression(string str) {

    stack<char> operators;
    // r = sqrt s = sin c = cos t = tan l = log n = ln e = exp
    stack<LargeNumber> operands;

    str = removeSpace(str);
    str = checkMinusSign(str);

    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '(') operators.push('(');
        else if (str[i] == ')') {
            while (operators.top() != '(') {
                char op = operators.top();
                if (op == '+' || op == '-' || op == '*' || op == '/' || op == '^') {
                    operators.pop();
                    LargeNumber l1 = operands.top();
                    operands.pop();
                    LargeNumber l2 = operands.top();
                    operands.pop();
                    operands.push(LargeNumber::calc(l1, l2, op));
                } else {
                    operators.pop();
                    LargeNumber l1 = operands.top();
                    operands.pop();
                    operands.push(LargeNumber::calc(l1, op));
                }
            }
            operators.pop();
        } else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '^') {
            while (!operators.empty() && preference(str[i]) <= preference(operators.top())) {
                char op = operators.top();
                operators.pop();
                LargeNumber l1 = operands.top();
                operands.pop();
                LargeNumber l2 = operands.top();
                operands.pop();
                operands.push(LargeNumber::calc(l1, l2, op));
            }
            operators.push(str[i]);
        } else if (str[i] == '!') {
            operators.push('!');
        } else {
            string tmp = "";
            while (i < str.length() && !isOperator(str[i]) ) {
                tmp += str[i++];
            }
            i--;
            if (func.find(tmp) != func.end()) {
                operators.push(func.at(tmp));
            } else if (vars.find(tmp) != vars.end()) {
                operands.push(vars.at(tmp));
            }
            else {
                operands.push(LargeNumber(tmp.c_str()));
            }
        }
    }
    while (!operators.empty()) {
        char op = operators.top();
        if (op == '+' || op == '-' || op == '*' || op == '/' || op == '^') {
            operators.pop();
            LargeNumber l1 = operands.top();
            operands.pop();
            LargeNumber l2 = operands.top();
            operands.pop();
            try {
                operands.push(LargeNumber::calc(l1, l2, op));
            } catch (runtime_error &e) {
                throw e;
            }
        } else {
            op = operators.top();
            operators.pop();
            LargeNumber l1 = operands.top();
            operands.pop();
            operands.push(LargeNumber::calc(l1, op));
        }
    }
    return operands.top();
}

void assignVariable(string var, string expr) {
    vars[var] = evaluateExpression(expr);
}



#include "Expression.h"
#include "LargeNumber.h"
#include <stack>
#include <string>
#include <regex>
#include <map>
#include <iostream>
#include <utility>
using namespace std;

const map<string, char> func = {{"~",'~'},
                                {"sqrt", 'r'},
                                {"sin",  's'},
                                {"cos",  'c'},
                                {"tan",  't'},
                                {"log",  'g'},
                                {"ln",   'n'},
                                {"exp",  'e'}};

map<string, LargeNumber> vars;
LargeNumber ans = LargeNumber(0);

LargeNumber getAns() {
    return ans;
}

void operatorHandling(char op, stack <LargeNumber> &operands, stack <char> &operators) {
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

int preference(char op) {
    if(op == '+' || op == '-' || op == '~') return 1;
    if(op == '*' || op == '/') return 2;
    if(op == '^') return 3;
    return 0;
}



bool isOperator(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/' || op == '^' || op == '(' || op == ')' || op == '~';
}

bool isNumber(char c) {
    return c >= '0' && c <= '9';
}

bool isValidNumber(const string& str) {
    return regex_match(str, regex(R"(-?\d+\.?\d*(e-?\d+)?)"));
}

string removeSpace(const string& str) {
    string res;
    for(char c : str) {
        if(c != ' ') res += c;
    }
    return res;
}

//convert unary minus sign to '~'
string checkMinusSign(string str) {
    string res;
    for(int i = 0; i < str.length(); i++) {
        if(str[i] == '-') {
            if(i == 0 || str[i - 1] == '(' ) {
                res += '~';
            } else {
                res += '-';
            }
        } else {
            res += str[i];
        }
    }
    return res;
}

void init() {
    vars["ans"] = LargeNumber(0);
    vars["pi"] = LargeNumber("3.1415926535897932384626433832795028841971693993751");
    vars["e"] = LargeNumber("2.7182818284590452353602874713526624977572470936999");
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
    if(bracketCnt != 0) {
        string msg = "Unmatched bracket!";
        cout << msg << endl;
    }
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
                    operands.pop();
                    operands.pop();
                    operands.push("");
                } else {
                    if(operators.empty()) return false;
                    if(operands.empty()) return false;
                    operators.pop();
                    operands.pop();
                    operands.push("");
                }
            }
            operators.pop();
        } else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '^') {
            while (!operators.empty() && operators.top() != '(') {
                char op = operators.top();
                operators.pop();
                if (op == '+' || op == '-' || op == '*' || op == '/' || op == '^') {
                    if(operands.size() < 2) return false;
                    operands.pop();
                    operands.pop();
                } else {
                    if(operands.empty()) return false;
                    operands.pop();
                }
                operands.push("");
            }
            operators.push(str[i]);
        } else if (str[i] == '~') {
            operators.push('~');
        } else {
            string tmp;
            while (i < str.length() && !isOperator(str[i]) ) {
                tmp += str[i++];
            }
            i--;
            if (func.find(tmp) != func.end()) {
                operators.push(func.at(tmp));
            } else if(isValidNumber(tmp))
                operands.push("");
            else if(vars.find(tmp) != vars.end())
                operands.push("");
            else {
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
    string tmp = std::move(str);
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
            if(var == "prec") {
                LargeNumber l = evaluateExpression(expr);
                if(l.neg == 1 && l.isInteger()) {
                    short prec = l.toShort();
                    LargeNumber::setPrecision(prec);
                    cout << "Precision set to " << prec << endl;
                }
                else {
                    cout << "Invalid precision value" << endl;
                }
            }
            else if(var == "e" || var == "pi") {
                cout << "Cannot assign value to constant variable" << endl;
            }
            else if(func.find(var) != func.end()) {
                cout << "Variable has the same name as a defined function!" << endl;
            }
            else if(vars.find(var) != vars.end()) {
                assignVariable(var, expr);
                cout << "Variable " << var << " reassigned!" << endl;
            }
            else {
                assignVariable(var, expr);
                cout << "Variable " << var << " assigned!" << endl;
            }
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
                operatorHandling(op, operands, operators);
            }
            operators.pop();
        } else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '^') {
            while (!operators.empty() && preference(str[i]) <= preference(operators.top())) {
                char op = operators.top();
                operatorHandling(op, operands, operators);
            }
            operators.push(str[i]);
        } else if (str[i] == '~') {
            operators.push('~');
        } else {
            string tmp;
            while (i < str.length() && !isOperator(str[i]) ) {
                tmp += str[i++];
            }
            i--;
            if(tmp == "ans") {
                operands.push(ans);
            }
            else if (func.find(tmp) != func.end()) {
                operators.push(func.at(tmp));
            } else if (vars.find(tmp) != vars.end()) {
                operands.push(vars.at(tmp));
            }
            else {
                operands.push(LargeNumber(tmp));
            }
        }
    }
    while (!operators.empty()) {
        char op = operators.top();
        operatorHandling(op, operands, operators);
    }
    ans = operands.top();
    return operands.top();
}

void assignVariable(const string& var, string expr) {
    vars[var] = evaluateExpression(std::move(expr));
}



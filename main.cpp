#include <iostream>
#include <regex>
#include <cmath>
#include <LargeNumber.h>
#include "Expression.h"
#include <map>
#define MAXL 10000
using namespace std;

int main() {
    string s;
    init();
    cout << "Welcome to the calculator!" << endl;
    cout << "Type \"\\help\" for help." << endl;
    cout << "Please enter your expression: ";
    while(getline(cin, s)) {
        if (s == "exit") {
            return 0;
        }
        if (s == "\\help") {
            cout << "This is a calculator that can calculate expressions with variables and functions." << endl;
            cout << "The supported functions are: sin, cos, tan, log, ln, exp, sqrt." << endl;
            cout << "The supported operators are: +, -, *, /, ^, ~." << endl;
            cout << "The supported constants are: pi, e." << endl;
            cout << "The supported assignment expression is: var = expr." << endl;
            cout << "You can set precision by typing prec = n, where n is the number of digits." << endl;
            cout << "This will only be applied to the functions." << endl;
            cout << "Otherwise, precision is set according to the maximum precision of your term" << endl;
            cout << "Type \"exit\" to exit." << endl;
            cout << "Please enter your expression: ";
            continue;
        }
        if (s.empty()) {
            continue;
        }
        if(isValidAssignmentExpression(s)) ;
        else if(isValidExpression(s)) {
            try {
                LargeNumber res = evaluateExpression(s);
                cout << "The answer is " << res.toString() << endl;
            } catch (const char* msg) {
                cout << msg << endl;
            }
        } else {
            cout << "Invalid expression" << endl;
        }
        cout << "Please enter your expression: ";
    }
    return 0;
}

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
    while(getline(cin, s)) {
        if (s == "exit") {
            return 0;
        }
        if(isValidAssignmentExpression(s))
            cout << "Variable assigned" << endl;
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
    }
    return 0;
}

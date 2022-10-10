#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <LargeNumber.h>

LargeNumber evaluateExpression(std::string str);
bool isValidExpression(std::string str);
bool isValidAssignmentExpression(std::string str);
void assignVariable(std::string var, std::string expr);
#endif

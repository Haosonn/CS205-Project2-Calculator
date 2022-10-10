#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <LargeNumber.h>

void init();
LargeNumber evaluateExpression(std::string str);
bool isValidExpression(std::string str);
bool isValidAssignmentExpression(std::string str);
void assignVariable(const std::string& var, std::string expr);
#endif

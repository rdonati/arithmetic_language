#ifndef USER_FUNCTIONS_H
#define USER_FUNCTIONS_H

#include <string.h>
#include <stdlib.h>
#include "symbol_table.h"
#include "expression.h"

//CREATION/APPENDING
Expression* createExpression(char* input);
Expression* createExpressionHelper(char* input);
Expression* appendExpression(char* input);
Expression* findSubset(SymbolTable* st, char* input, Expression* exp);

//EVALUATION
int evaluateExpression(SymbolTable* st, Expression* exp);
Expression* evaluateFunctionNoParams(SymbolTable* st, Expression* exp, int numParams, char** paramNames, int* paramVals, int funcWParams);
int evaluateFunction(SymbolTable* st, Expression* exp);
int evaluateFunctionHelper(SymbolTable* st, Expression* exp, int numParams, char** paramNames, int* paramVals);
int evaluateFunctionGivenParams(SymbolTable* st, char* symbol, char* params);

//PRINTING
void printExpression(Expression* exp);
void printExpressionHelper(Expression* exp);

//DEBUGGING HELPERS
void printNode(Expression* exp);
void printDebug(Expression* exp);

#endif //USER_FUNCTIONS.H

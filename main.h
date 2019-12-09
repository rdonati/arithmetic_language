#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include "expression.h"
#include "symbol_table.h"
#include "user_functions.h"

int main();

void printInstructions();
char* getInput();
Expression* getExpression(SymbolTable* st);

#endif //MAIN_H


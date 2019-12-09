#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Expression{
	char operator;
	int val;
	char* symbol;
	struct Expression* first;
	struct Expression* rest;
}Expression;

Expression* initializeExpression();
void freeExpression(Expression* exp);

#endif //EXPRESSION_H

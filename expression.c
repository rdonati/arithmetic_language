#include "expression.h"

/*
* Description: Creates a pointer to an expression. Negative values are not valid, so the value -1 indicates a "null" value
*/
Expression* initializeExpression(){
	Expression* exp = malloc(sizeof(Expression));
	exp->symbol = NULL;
	exp->val = -1;
	exp->first = NULL;
	exp->rest = NULL;
	exp->operator = 0;
	return exp;
}

/*
* Description: Recursively frees an expression
*/
void freeExpression(Expression* exp){
	if(exp->first) freeExpression(exp->first);
	if(exp->rest) freeExpression(exp->rest);
	free(exp);
}

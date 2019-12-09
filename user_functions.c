#include "user_functions.h"

/**
 * Params: User's text input (e.g. "(+ 1 2 3 (-3 2))")
 * Return: Pointer to the first node of the expression
 * Description: Converts user input into a chain/tree of Expression structs
 */
Expression* createExpression(char* input){
	//If the input is a single number
	if(*input > 47 && *input < 58){
		Expression* exp = initializeExpression();
		char* num = malloc(sizeof(char)*100);
		int idx = 0;
		while(*input > 47 && *input < 58){
			num[idx] = *input;
			idx++;
			input++;
		}
		int n = atoi(num);
		exp->val = n;
		return exp;
	}
	//If the input is a single symbol
	if(isalpha(*input)){
		Expression* exp = initializeExpression();
		char* symbol = malloc(sizeof(char)*100);
		int idx = 0;
		while(*input != 0 && *input != '\n'){
			symbol[idx] = *input;
			idx++;
			input++;
		}
		exp->symbol = symbol;
		return exp;
	}
	//If the input is an expression (begins with a '(')
	else{
		return createExpressionHelper(++input);
	}
}

/**
 * Params: User's text input, excluding the leading parenthesis (e.g. "+ 1 2 3 (-3 2))")
 * Return: Pointer to the first node of the expression
 * Description: Helper methof for createExpression; converts user input into a chain/tree of Expression structs
 */
Expression* createExpressionHelper(char* input){
	//If the end of an expression is reached
	if(*input == ')') return NULL;
	else{
		Expression* exp = initializeExpression();
		exp->first = initializeExpression();
		
		//Ensures that it is not operating on a space
		while(*input == ' ') input++;
		
		//If an operator is found
		if(*input == '+'  || *input == '-' || *input == '*'){
			exp->first->operator = *input;
			exp->rest = createExpressionHelper(++input);
		}
		//If the beginning of a subexpression is found
		else if(*input == '('){
			//Recursizely creates subexpression
			exp->first = createExpressionHelper(++input);

			//Finds the complementary closing parenthesis and continues from there. Done by counting opening and closing parens
			int parenCount = 1;
			while(parenCount != 0){
				input++;
				if(*input == '(') parenCount++;
				if(*input == ')') parenCount--;
			}
			exp->rest = createExpressionHelper(++input);
		}
		//If a number is found
		else if(*input > 47 && *input < 58){
			//Gets number (multidigit if necessary)
			char* num = malloc(sizeof(char)*10);
			int idx = 0;
			while(*input > 47 && *input < 58){
				num[idx] = *input;
				idx++;
				input++;
			}
			int n = atoi(num);
			exp->first->val = n;
			exp->rest = createExpressionHelper(input);
		}
		//If a symbol is found
		else{
			char* symbol = malloc(sizeof(char)*10);
			int idx = 0;
			while(*input != 0 && *input != '(' && *input != ')' && *input != ' '){
				symbol[idx] = *input;
				idx++;
				input++;
			}
			exp->first->symbol = symbol;
			exp->rest = createExpressionHelper(input);
		}
		return exp;
	}
}

/**
 * Params: Symbol table, first node of the expression to be evaluated
 * Return: The value of the expression
 * Description: This function translates a expression to a function that simply has no parameters
 */
int evaluateExpression(SymbolTable* st, Expression* exp){
	Expression* result = evaluateFunctionNoParams(st, exp, 0, 0, 0, 1);
	return evaluateFunctionHelper(st, result, 0, 0, 0);
}

/**
 * Params: Symbol table, first node of the expression to be evaluated
 * Return: A pointer to the first node of the resulting expression
 * Description: Enables the use of (f(r(r(variable))))
 */
Expression* evaluateFunctionNoParams(SymbolTable* st, Expression* exp, int numParams, char** paramNames, int* paramVals, int funcWParams){
	//If expression is a singular term
	if(!exp->first){
		char* symbol = exp->symbol;
		if(!symbol) return exp;
		//If symbol = 'f'
		if(symbol[0] == 'f' && symbol[1] == 0) return evaluateFunctionNoParams(st, exp->rest->first, numParams, paramNames, paramVals, 0)->first;
		//If symbol = 'r'
		else if(symbol[0] == 'r' && symbol[1] == 0) return evaluateFunctionNoParams(st, exp->rest->first, numParams, paramNames, paramVals, 0)->rest;
		//If variable
		else return lookup(st, symbol);
	}
	//If expression is multiple terms
	else{
		char* symbol = exp->first->symbol;
		if(!symbol) return exp;
		//If symbol = 'f'
		if(symbol[0] == 'f' && symbol[1] == 0) return evaluateFunctionNoParams(st, exp->rest->first, numParams, paramNames, paramVals, 0)->first;
		//If symbol = 'r'
		else if(symbol[0] == 'r' && symbol[1] == 0) return evaluateFunctionNoParams(st, exp->rest->first, numParams, paramNames, paramVals, 0)->rest;
		//If variable
		if(funcWParams){
			return exp;
		}else{
			return lookup(st, symbol);
		}
	}
}

/**
 * Params: Symbol table, first node of the expression to be evaluated
 * Return: The value of the function
 * Description: Evaluates a function w/ parameters
 */
int evaluateFunction(SymbolTable* st, Expression* exp){
	//Calculating necessary information
	int numParams = getNumParams(st, exp->first->symbol);
	char** paramNames = getParamNames(st, exp->first->symbol);
	int* paramVals = malloc(sizeof(int) * numParams);

	//Calculating parameter values and storing them in paramVals
	Expression* curExp = exp->rest;
	for(int i = 0; i < numParams; i++){
		paramVals[i] = evaluateExpression(st, curExp);
		curExp = curExp->rest;
	}

	Expression* expToBeEvaluated = lookup(st, exp->first->symbol);
	return evaluateFunctionHelper(st, expToBeEvaluated, numParams, paramNames, paramVals);
}

/**
 * Params: Symbol table, first node of the expression to be evaluated, user's input
 * Return: The value of the function 
 * Description: Evaluates a function where the user directly passes params
 */
int evaluateFunctionGivenParams(SymbolTable* st, char* symbol, char* params){
	//Calculating necessary information
	int numParams = getNumParams(st, symbol);
	char** paramNames = getParamNames(st, symbol);
	int* paramVals = malloc(sizeof(int) * numParams);

	//Calculating parameter values and storing them in paramVals
	for(int i = 0; i < numParams; i++){
		char* word = malloc(sizeof(char) * 100);
		int idx = 0;
		//Processing the user input word by word (i.e. "22 3 hi" would be [22, 3, "hi"])
		while(*params != 0 && *params != ' '){
			word[idx] = *params;
			idx++;
			params++;
		}
		//Moving input pointer to the first character of the next word
		params++;
		word[idx] = 0;
		//If the word is a number
		if(word[0] > 47 && word[0] < 58){
			paramVals[i] = atoi(word);
		}else{
			Expression* e = lookup(st, word);
			int type = getSymbolType(st, word);
			if(type == 1){
				paramVals[i] = evaluateExpression(st, e);
			}else if(type == 2){
				Expression* result = evaluateFunctionNoParams(st, e, 0, 0, 0, 1);
				paramVals[i] = evaluateExpression(st, result);
			}else if(type == 3){
				printf("Cannot evaluate nested functions w/ parameters\n");
			}
		}
	}

	Expression* expToBeEvaluated = lookup(st, symbol);
	return evaluateFunctionHelper(st, expToBeEvaluated, numParams, paramNames, paramVals);
}


/**
 * Params: Symbol table, first node of the expression to be evaluated, # of parameters for the function, names of those parameters, values of those parameters
 * Return: The value that the function evaluates to
 */
int evaluateFunctionHelper(SymbolTable* st, Expression* exp, int numParams, char** paramNames, int* paramVals){
	int value = 0;
	int type;
	int paramIndex;

	exp = evaluateFunctionNoParams(st, exp, numParams, paramNames, paramVals, 1);

	//Handling expressions w/o operators (i.e. single numbers and single symbols such as '1' or 'x')
	if(exp->val >= 0) return exp->val;
	
	else if(symbolIsValid(exp->symbol)){
		
		//Handling each type of evaluation
		if(getSymbolType(st, exp->symbol) == 1){
			return evaluateExpression(st, lookup(st, exp->symbol));	
		}else if(getSymbolType(st, exp->symbol) == 2){
			Expression* result = evaluateFunctionNoParams(st, lookup(st, exp->symbol), numParams, paramNames, paramVals, 0);
			return evaluateExpression(st, result);
		}else if(getSymbolType(st, exp->symbol) == 3){
			return evaluateFunctionHelper(st, exp, numParams, paramNames, paramVals);
		}

	}
	else if(exp->first->val >= 0){
		return exp->first->val;
	}
	//Handling expressions w/o operators (i.e. single numbers and single symbols such as '(1)' or '(x)')
	else if(symbolIsValid(exp->first->symbol)){
		
		//Handling each type of evaluation
		if(getSymbolType(st, exp->first->symbol) == 1){
			return evaluateFunctionHelper(st, lookup(st, exp->first->symbol), numParams, paramNames, paramVals);
		}else if(getSymbolType(st, exp->first->symbol) == 2){
			Expression* result = evaluateFunctionNoParams(st, lookup(st, exp->first->symbol), numParams, paramNames, paramVals, 0);
			return evaluateFunctionHelper(st, result, numParams, paramNames, paramVals);
		}else if(getSymbolType(st, exp->first->symbol) == 3){
			return evaluateFunction(st, exp);
		}
	}
	//Handling nested expressions
	else if(!exp->first->operator) return evaluateFunctionHelper(st, exp->first, numParams, paramNames, paramVals);

	char operator = exp->first->operator;
	Expression* curExp = exp;

	//Different cases for each operator	
	switch(operator){
		case '-':
			//Sets value to the first term
			curExp = curExp->rest;
			if(curExp->first->val >= 0) value = curExp->first->val;
			else{
				paramIndex = getParamIndex(curExp->first->symbol, paramNames, numParams);
				if(paramIndex == -1){
					value += evaluateFunctionHelper(st, curExp->first, numParams, paramNames, paramVals);
				}
				else{
					value += paramVals[paramIndex];
				}				
			}

			//Subtracts all subsequent terms
			while(curExp->rest){
				curExp = curExp->rest;
				if(curExp->first->val >= 0) value -= evaluateExpression(st, curExp);
				else if(curExp->first->symbol != NULL){
					paramIndex = getParamIndex(curExp->first->symbol, paramNames, numParams);

					//If the symbol is a variable and NOT a parameter
					if(paramIndex == -1){
						Expression* symbolExpression = lookup(st, curExp->first->symbol);
						type = getSymbolType(st, curExp->first->symbol);

						//Handling each type of evaluation
						if(type == 1){
							value -= evaluateFunctionHelper(st, symbolExpression, numParams, paramNames, paramVals);
						}else if(type == 2){
							Expression* result = evaluateFunctionNoParams(st, symbolExpression, numParams, paramNames, paramVals, 0);
							value -= evaluateFunctionHelper(st, result, numParams, paramNames, paramVals);
						}else if(type == 3){
							value -= evaluateFunction(st, curExp);
						}
					}
					//If the symbol is a parameter
					else value -= paramVals[paramIndex];
				}
				else value -= evaluateFunctionHelper(st, curExp->first, numParams, paramNames, paramVals);
           	}
			break;
		case '+':
			//Adds all terms
			while(curExp->rest){
				curExp = curExp->rest;
				if(curExp->first->val >= 0) value += evaluateFunctionHelper(st, curExp, numParams, paramNames, paramVals);
				else if(curExp->first->symbol != NULL){
                	paramIndex = getParamIndex(curExp->first->symbol, paramNames, numParams);

					//If the symbol is a variable and NOT a parameter
					if(paramIndex == -1){
						Expression* symbolExpression = lookup(st, curExp->first->symbol);
						type = getSymbolType(st, curExp->first->symbol);

						//Handling each type of evaluation
						if(type == 1){
							value += evaluateFunctionHelper(st, symbolExpression, numParams, paramNames, paramVals);
						}else if(type == 2){
							Expression* result = evaluateFunctionNoParams(st, symbolExpression, numParams, paramNames, paramVals, 0);
							value += evaluateFunctionHelper(st, result, numParams, paramNames, paramVals);
						}else if(type == 3){
							value += evaluateFunction(st, curExp);
						}
					}
					//If the symbol is a parameter
					else value += paramVals[paramIndex];
				}
				else value += evaluateFunctionHelper(st, curExp->first, numParams, paramNames, paramVals);
			}
			break;
		case '*':
			//Sets value to first term
			curExp = curExp->rest;
			if(curExp->first->val >= 0) value = curExp->first->val;
			else{
				paramIndex = getParamIndex(curExp->first->symbol, paramNames, numParams);
				if(paramIndex == -1){
					value += evaluateFunctionHelper(st, curExp->first, numParams, paramNames, paramVals);
				}
				else{
					value += paramVals[paramIndex];
				}				
			}
			//Multiplies value by subsequent terms
			while(curExp->rest){
				curExp = curExp->rest;
				if(curExp->first->val >= 0) value *= evaluateFunctionHelper(st, curExp, numParams, paramNames, paramVals);
				else if(curExp->first->symbol != NULL){
                	paramIndex = getParamIndex(curExp->first->symbol, paramNames, numParams);
					
					//If the symbol is a variable and NOT a parameter
					if(paramIndex == -1){
						Expression* symbolExpression = lookup(st, curExp->first->symbol);
						type = getSymbolType(st, curExp->first->symbol);

						//Handling each type of evaluation
						if(type == 1){
							value *= evaluateFunctionHelper(st, symbolExpression, numParams, paramNames, paramVals);
						}else if(type == 2){
							Expression* result = evaluateFunctionNoParams(st, symbolExpression, numParams, paramNames, paramVals, 0);
							value *= evaluateFunctionHelper(st, result, numParams, paramNames, paramVals);
						}else if(type == 3){
							value *= evaluateFunction(st, curExp);
						}
					}
					//If the symbol is a parameter
					else value *= paramVals[paramIndex];
				}
				else value *= evaluateFunctionHelper(st, curExp->first, numParams, paramNames, paramVals);
			}
			break;
	}
	return value;
}

/**
 * Params: First node of the expression to be printed
 */
void printExpression(Expression* exp){
	//Prints expression that contains only a value
	if(exp->val >= 0){
		printf("%i\n", exp->val);
		return;
	}
	//Prints expression that contains only an operator
	else if(exp->operator){
		printf("%c\n", exp->operator);
		return;
	}
	//Prints expression that contains only a symbol
	else if(exp->symbol){
		printf("%s\n", exp->symbol);
		return;
	}
	//Prints a proper expression (with parenthesis)
	else{
		printf("(");
		printExpressionHelper(exp);
		printf(")\n");
	}
}

/**
 * Params: First node of the expression to be printed 
 * Description: Helper method for printExpression
 */
void printExpressionHelper(Expression* exp){
	if(exp->first){

		//If first node is an operator
		if(exp->first->operator) printf("%c ", exp->first->operator); 

		//If first node is a symbol
		else if(exp->first->symbol != 0){
			printf("%s", exp->first->symbol);
			if(exp->rest) printf(" ");
		}

		//If first node is a symbol
		else if(exp->first->val >= 0){
			printf("%i", exp->first->val); 
			if(exp->rest) printf(" ");
		}

		//If first node is a subexpression
		else{
			printf("(");
			printExpressionHelper(exp->first);
			printf(")");
			if(exp->rest) printf(" ");
		}
	}
	//Recursively print rest of the expression, if it exists
	if(exp->rest) printExpressionHelper(exp->rest);
}

/**
 * Params: First node of the expression to be printed 
 * Description: Prints all of the instance variables for each node (value, address, first, rest, operator, symbol) in a pre-order manner
 */
void printDebug(Expression* exp){
	printNode(exp);
	if(exp->first) printDebug(exp->first);
	if(exp->rest) printDebug(exp->rest);
}

/**
 * Params: Node to be printed
 */
void printNode(Expression* exp){
	printf("Value: %i\nAddress: %p\nFirst: %p\nRest: %p\n, Operator: %c\nSymbol: %s\n\n", exp->val, exp, exp->first, exp->rest, exp->operator, exp->symbol);
}

/**
 * Params: Symbol table, user input of f's and r's, the expression to find a subset of
 * Return: A pointer to the first node of the subset
 */
Expression* findSubset(SymbolTable* st, char* input, Expression* exp){
	//Iterates through input - each time 'r' is in encountered, jump to rest; each time 'f' is encountered, jump to first 
	while(*input){
		switch(*input){
			case 'f':
				exp = exp->first;
				break;
			case 'r':
				exp = exp->rest;
				break;
			default:
				break;
		}
		input++;
	}
	return exp;
}


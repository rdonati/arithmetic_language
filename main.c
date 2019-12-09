#include "main.h"

int main(){
	SymbolTable* st = initializeSymbolTable();
	char* userCommand = malloc(sizeof(char)*20);
	Expression* exp;
	char* symbol;
	printInstructions();

	//Processes user commands until the user quits (by entering 'q')
	while(*userCommand != 'q'){
		//Getting user command
		fgets(userCommand, 100, stdin);
		char* input = malloc(sizeof(char) * 100);

		//Handles user command accordingly
		switch(*userCommand){
			case 'h':
				printInstructions();
				break;
			case 'c':
				printf("Expression to create: ");
				fgets(input, 100, stdin);
				exp = createExpression(input);
				//Adding expression to symbol table as variable "foo"
				addVariable(st, "foo", exp);
				break;
			case 'p':
				printf("Expression to print: ");
				input = getInput();
				exp = lookup(st, input);
				if(!exp) break;

				//Printing params if it is a function w/ params
				if(getSymbolType(st, input) == 3){
					printf("%s(", input);
					printParams(st, input);
					printf(") = ");
					printExpression(exp);
				}
				else printExpression(exp);
				
				break;
			case 'b':
				printf("Expression to debug: ");
				input = getInput();
				exp = lookup(st, input);
				if(!exp) break;

				printDebug(exp);
				break;
			case 'e':
				printf("Expression to evaluate: ");
				input = getInput();
				exp = lookup(st, input);
				if(!exp) break;
				//Getting type of evaluation (variable, w/o params, w/ params)
				int type;
				int value;
				type = getSymbolType(st, input);
				if(type == 1){
					value = evaluateExpression(st, exp);
				}
				else if(type == 2){
					Expression* result = evaluateFunctionNoParams(st, exp, 0, 0, 0, 0);
					if(result->operator){
						printf("%c\n", result->operator);
						break;
					}
					value = evaluateExpression(st, result);
				}else if(type == 3){
					printf("Parameters (");
					printParams(st, input);
					printf("): ");
					char* params = getInput();
					value = evaluateFunctionGivenParams(st, input, params);
				}
				printf("Value: %i\n", value);
				break;
			case 'a':
				exp = lookup(st, "foo");
				if(!exp) break;
				printf("Expression to append: ");
				input = getInput();
				//Goes to last node of current expression and appends a new expression 
				Expression* nullExp = initializeExpression();
				Expression* newExp = createExpression(input);
				nullExp->first = newExp;
				while(exp->rest) exp = exp->rest;
				exp->rest = nullExp;
				break;
			case 's':
				exp = lookup(st, "foo");
				if(!exp) break;
				printf("Subset: ");
				input = getInput();
				Expression* subset = findSubset(st, input, exp);
				printExpression(subset);
				break;
			case 'z':
				printSymbolTable(st);
				break;
			case 'l':
				printf("Function name: ");
				symbol = getInput();
				printf("Parameters: ");
				char* params = getInput();
				printf("Expression that \"%s\" will represent: ", symbol);
				fgets(input, 100, stdin);
				exp = createExpression(input);
				addFunctionParams(st, symbol, exp, params);
				break;	
			case 'd':
				printf("Function name: ");
				symbol = getInput();
				printf("Function that \"%s\" will represent: ", symbol);
				fgets(input, 100, stdin);
				exp = createExpression(input);
				addFunctionNoParams(st, symbol, exp);
				break;
			case 'v':
				printf("Variable name: ");
				symbol = getInput();
				printf("Expression that \"%s\" will represent: ", symbol);
				fgets(input, 100, stdin);
				exp = createExpression(input);
				addVariable(st, symbol, exp);
				break;
			default:
				break;
		}
	}
}


/**
* Description: Retrieves input from user and cleans extra characters at the end (which fgets leaves).
*/
char* getInput(){
	char* input = malloc(sizeof(char) * 10);
	fgets(input, 100, stdin);

	if(*input == '\n') return 0;
	//Used to store address while input is iterated through
	char* address = input;
	//Move through input until a non-alphanumeric character is reached
	do input++;
	while(*input > 31 && *input < 127);
	//Set null terminator
	*input = 0;

	return address;
}

//Prints instructions
void printInstructions(){
	printf("\nInstructions:\nq = quit\nh = help (open this dialog)\nc = create new expression\np = print expression\ne = evaluate expression\na = append to \"foo\"\ns = get a subset of \"foo\"\nv = define variable\nd = define function without params\nl = define function with params\nz = print symbol table\n\n");
}
#include "symbol_table.h"

/**
 * Params: Key (name of variable), value (pointer to an expression)
 * Return: A new entry with the given key and value
 */
Entry* initializeEntry(char* key, Expression* value, int type){
    Entry* e = calloc(1, sizeof(Entry));
    e->key = key;
    e->value = value;
    e->type = type;
    return e;
}

/**
 * Return: Pointer to an empty symbol table (w/ null head and tail nodes)
 */
SymbolTable* initializeSymbolTable(){
    SymbolTable* st = malloc(sizeof(SymbolTable));
    st->head = calloc(1, sizeof(Entry));
    st->tail = calloc(1, sizeof(Entry));
    st->head->next = st->tail;
    st->tail->prev = st->head;
    return st;
}

/**
* Params: symbol table, key, value, type of entry (1 = variable; 2 = func w/o params; 2 = func w/ params), params (if applicable), numParams(if applicable)
* Return: If entry exists, replaces it and returns 1; if entry doesn't exist returns 0
*/
int replaceEntry(SymbolTable* st, char* key, Expression* value, int type, char** params, int numParams){
	Entry* curEntry = st->head->next;
	while(curEntry->key != 0){
        //Iterates through symbol table until it finds a matching key or reaches the tail node
		if(strcmp(key, curEntry->key) == 0){
			curEntry->value = value;
            curEntry->type = type;
            curEntry->params = params;
            curEntry->numParams = numParams;
			return 1;
		}
		curEntry = curEntry->next;
	}
	return 0;
}

/**
 * Params: Symbol table, key (variable/function name), value (pointer to expression)
 * Description: Adds a variable to the symbol table; replaces previous entry if necessary
 **/
void addVariable(SymbolTable* st, char* key, Expression* value){
    //Checks if entry already exists
    if(replaceEntry(st, key, value, 1, 0, 0) == 1) return;
    //Adds entry if it doesn't exist
    Entry* e = initializeEntry(key, value, 1);
    e->prev = st->head;
    e->next = st->head->next;
    st->head->next->prev = e;
    st->head->next = e;
}

/**
 * Params: Symbol table, key (variable/function name), value (pointer to expression)
 * Description: Adds a function w/o params to the symbol table; replaces previous entry if necessary
 */
void addFunctionNoParams(SymbolTable* st, char* key, Expression* value){
    //Checks if entry already exists
    if(replaceEntry(st, key, value, 2, 0, 0) == 1) return;
    //Adds entry if it doesn't exist
    Entry* e = initializeEntry(key, value, 2);
    e->prev = st->head;
    e->next = st->head->next;
    st->head->next->prev = e;
    st->head->next = e;
}

/**
 * Params: Symbol table, key (variable/function name), value (pointer to expression)
 * Description: Adds a function w params to the symbol table; replaces previous entry if necessary
 */
void addFunctionParams(SymbolTable* st, char* key, Expression* value, char* params){
    //Checks if entry already exists
    if(replaceEntry(st, key, value, 3, convertParams(params, calculateNumParams(params)), calculateNumParams(params)) == 1) return;
    //Adds entry if it doesn't exist
    Entry* e = initializeEntry(key, value, 3);
    e->numParams = calculateNumParams(params);
    e->params = convertParams(params, e->numParams);
    e->prev = st->head;
    e->next = st->head->next;
    st->head->next->prev = e;
    st->head->next = e;
}

/**
 * Params: Symbol table, key (variable/function name)
 * Return: Associated value (if exists), NULL (if doesn't exist)
 */
Expression* lookup(SymbolTable* st, char* key){
    Entry* curEntry = st->head->next;
    while(curEntry->key != 0){
        if(strcmp(key, curEntry->key) == 0) return curEntry->value;
        curEntry = curEntry->next;
    }
    printf("No entry with name \"%s\" exists\n", key);
    return NULL;
}

/**
 * Params: Symbol table, key (variable/function name)
 * Return: An integer representing the type of symbol (1 = variable; 2 = func w/o params; 3 = func w/ params)
 */
int getSymbolType(SymbolTable* st, char* key){
    Entry* curEntry = st->head->next;
    while(curEntry->key != 0){
        if(strcmp(key, curEntry->key) == 0) return curEntry->type;
        curEntry = curEntry->next;
    }
    printf("No entry with name \"%s\" exists\n", key);
    return 0;
}

/**
 * Params: Symbol table, key (variable/function name)
 * Return: A cha** of the params for that entry
 */
char** getParamNames(SymbolTable* st, char* key){
    Entry* curEntry = st->head->next;
    while(curEntry->key != 0){
        if(strcmp(key, curEntry->key) == 0) return curEntry->params;
        curEntry = curEntry->next;
    }
    printf("No entry with name \"%s\" exists\n", key);
    return 0;
}

/**
 * Params: Symbol table
 * Description: Prints the contents of the symbol table in the format: name (type). E.g "foo (function w/ params)"
 */
void printSymbolTable(SymbolTable* st){
	printf("\n--------SYMBOL TABLE CONTENTS--------\n");

	Entry* curEntry = st->head->next;

    //Iterates through all entries in the symbol table
	while(curEntry->key != 0){
		printf("%s (", curEntry->key);
        if(curEntry->type == 1) printf("variable)\n"); 
        else if(curEntry->type == 2) printf("function w/o params)\n");
        else if(curEntry->type == 3) printf("function w params)\n");
		curEntry = curEntry->next;
	}
	printf("\n");
}

/**
 * Params: Symbol (variable/function name)
 * Return: 0 if symbol is NULL, 'r', or 'f'; 1 otherwise
 */
int symbolIsValid(char* symbol){
    if(!symbol || ((symbol[0] == 'r' || symbol[0] == 'f') && symbol[1] == 0)) return 0;
    else return 1;
}

/**
 * Params: Symbol table, key (variable/function name)
 * Return: The number of parameters that that function has
 */
int getNumParams(SymbolTable* st, char* key){
    Entry* curEntry = st->head->next;
    //Iterates through symbol table
    while(curEntry->key != 0){
        if(strcmp(key, curEntry->key) == 0) return curEntry->numParams;
        curEntry = curEntry->next;
    }
    printf("No entry with name \"%s\" exists\n", key);
    return 0;
}

/**
 * Params: Char* of space separated parameters (e.g. "x y hello")
 * Returns: Number of parameters (e.g. "x y hello" will return 3);
 */
int calculateNumParams(char* params){
    if(!params) return 0;
    int count = 1;
    //Assumes at least 1 parameter and then increments the count each time a space is encountered
    while(*params != 0){
        if(*params == ' ') count++;
        params++;
    }
    return count;
}

/**
 * Params: Char* of space-separated params (e.g. "x y hello")
 * Return: An array of char* where each index is one of the parameters
 */
char** convertParams(char* params, int numParams){
    //Allocating space for n 10-character-long params
    if(numParams == 0) return 0;
    char** p = malloc(sizeof(int) * numParams);
    for(int i = 0; i < numParams; i++) p[i] = malloc(sizeof(char) * 10);

    //Index counters
    int wordNum = 0;
    int charNum = 0;

    //Loop through the input, move to the next param each time a space is encountered
    while(*params != 0){
        if(*params != ' '){
            p[wordNum][charNum] = *params;
            charNum++;
            params++;
        }
        else{
            p[wordNum][charNum] = 0;
            params++;
            wordNum++;
            charNum = 0;
        }
    }
    return p;
}

/**
 * Params: Symbol (variable/function name), char** array of parameters, number of parameters
 * Return: The index in the array of parameters at which the given symbol appears; -1 if it does not appear
 */
int getParamIndex(char* symbol, char** paramList, int paramSize){
	for(int i = 0; i < paramSize; i++){
        if(strcmp(paramList[i], symbol) == 0) return i;
    }
	return -1;
}

/**
 * Params: Symbol table, symbol (name of function w/ params)
 * Description: Prints out the paramaters of a funtion
 */
void printParams(SymbolTable* st, char* symbol){
	char** paramNames = getParamNames(st, symbol);
	int numParams = getNumParams(st, symbol);

	for(int i = 0; i < numParams; i++){
		printf("%s", paramNames[i]);
        if(i != numParams - 1) printf(", ");
    }
}

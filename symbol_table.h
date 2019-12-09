#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include <string.h>
#include <stdlib.h> 
#include "expression.h"

/****************************************************************************
 *                              Structs
 ****************************************************************************/

typedef struct Entry{
    char* key;
    //Type:
    //1 = variable
    //2 = function w/o params
    //3 = function w/ params
    int type;
    char** params;
    int numParams;
    Expression* value;
    struct Entry* prev;
    struct Entry* next;
}Entry;

typedef struct SymbolTable{
    Entry* head;
    Entry* tail;
}SymbolTable;

/****************************************************************************
 *                              FUNCTIONS
 ****************************************************************************/ 

//INITIALIZATION
Entry* initializeEntry(char* key, Expression* value, int type);
SymbolTable* initializeSymbolTable();

//INSERTION
void addVariable(SymbolTable* st, char* key, Expression* value);
void addFunctionParams(SymbolTable* st, char* key, Expression* value, char* params);
void addFunctionNoParams(SymbolTable* st, char* key, Expression* value);
int replaceEntry(SymbolTable* st, char* key, Expression* value, int type, char** params, int numParams);

//GETTERS/LOOKUP
Expression* lookup(SymbolTable* st, char* key);
int getNumParams(SymbolTable* st, char* key);
int getSymbolType(SymbolTable* st, char* key);
char** getParamNames(SymbolTable* st, char* key);

//UTILITY
int calculateNumParams(char* params);
char** convertParams(char* params, int numParams);
void printSymbolTable();
int symbolIsValid(char* symbol);
int getParamIndex(char* symbol, char** paramList, int paramSize);
void printParams(SymbolTable* st, char* symbol);

#endif

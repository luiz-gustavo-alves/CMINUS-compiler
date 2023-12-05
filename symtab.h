#ifndef SYMTAB_H
#define SYMTAB_H

#include "utils.h"
#include "syntaxTree.h"

#define HASHTABLE_LEN 499

typedef enum { isFunc, isVar } idType;

typedef struct lineList { 
    struct lineList *next;
    int line;
} *lineList;

typedef struct symTable { 
    struct symTable* next;
	char *name, *scope;
    int idNumber;

    lineList lines;
    primitiveType type; 
    idType id;
} *symTable;

symTable hashtable[HASHTABLE_LEN];

symTable getHashValue(char *name);
int createHashKey(char *key);

int checkVarIsFunc(char *name);
int checkIdDeclaration(char *name);
int checkScope(char *name, char *scope);
void getIdType(char *name, char *scope, primitiveType *type);

void insertFuncSymtable(char *name, int line, primitiveType type);
void insertVarSymtable(char *name, int line, char *scope, primitiveType type);
void printSymtable();

#endif
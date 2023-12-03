#ifndef SYMTAB_H
#define SYMTAB_H

#include "utils.h"

#define HASHTABLE_LEN 211
#define SHIFT 4

typedef struct lineList { 
    struct lineList *next;
    int line;

} *lineList;

typedef struct symTable { 
    struct symTable* next;

	char *name, *scope;
    int idNumber, isFunc;

    lineList lines;
    primitiveType type; 
} *symTable;

static symTable hashtable[HASHTABLE_LEN];

static int varCount = 1;

symTable getHashValue(char *name);
int createHashKey(char *key);
int checkDeclaredFunc(char *name);
int checkDeclaredVar(char *name);
int checkVarScope(char *name, char *_var_scope);
int checkGlobalVar(char *name);

void getFuncType(char *name, primitiveType *p_kind);
void getVarType(char *name, char *_var_scope, primitiveType *p_kind);

void insertSymtable(char *name, int line_number, char *_var_scope, primitiveType kind, int is_function);
void printSymtable();

#endif /* SYMTAB_H */

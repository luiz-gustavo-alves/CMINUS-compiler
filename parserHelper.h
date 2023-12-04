#ifndef PARSERHELPER_H
#define PARSERHELPER_H

#include "syntaxTree.h"

#define YYPARSER
#define YYSTYPE treeNode*

char *expName;
char *variableName;
char *functionName;
char *lastToken;

int currentLine;
int functionCurrentLine;

int yylex();
int yyerror(char *errorMsg);

typedef struct callListNode {
    char *funcName;
    struct callListNode* next;
	struct callListNode *prev;
} callListNode;

typedef struct callList {
    struct callListNode* head;
    struct callListNode* tail;
} callList;

void initCallList(callList *list);
void insertNodeCallList(callList *list, char *name);
char *getLastNodeCallList(callList *list);
char *getTokenName(char *token);

#endif
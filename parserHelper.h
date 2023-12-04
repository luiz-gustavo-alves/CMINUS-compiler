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

typedef struct callList {
	
	char *name;
	struct callList *next;
} callList;

void insertCallNode(callList *list, char *name);
char *getLastCallNode(callList *list);
char *getTokenName(char *token);

#endif
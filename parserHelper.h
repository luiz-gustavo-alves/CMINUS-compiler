#ifndef PARSERHELPER_H
#define PARSERHELPER_H

#define YYPARSER
#define YYSTYPE treeNode*

int yylex();
int yyerror(char *errorMsg);

typedef struct callList {
	
	char *name;
	struct callList *next;
} callList;

/* Metodos para estrutura de dados Pilha */
void insertNode(callList *list, char *name);
char *getLastNode(callList *list);
char *getTokenName(char *token);

#endif
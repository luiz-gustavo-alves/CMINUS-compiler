#ifndef PARSERHELPER_H
#define PARSERHELPER_H

#define YYPARSER
#define YYSTYPE treeNode*

int yylex();
int yyerror(char *errorMsg);

typedef struct stack {
	
	char *name;
	struct stack *next;
	struct stack *top;
} stack;

/* Metodos para estrutura de dados Pilha */
void initStack(stack *st);
void push(stack *st, char *name);
char *pop(stack *st);
char *getTokenName(char *token);

#endif
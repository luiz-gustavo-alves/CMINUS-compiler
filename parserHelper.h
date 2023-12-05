#ifndef PARSERHELPER_H
#define PARSERHELPER_H

#include "syntaxTree.h"

#define YYPARSER
#define YYSTYPE treeNode*

char *expName;
char *variableName;
char *functionName;
char *currentToken;

int currentLine;
int functionCurrentLine;
int argsCount;
int paramsCount;

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

treeNode *createDeclNode(declType node);
treeNode *createExpNode(expType node);
treeNode *createStmtNode(stmtType node);
treeNode *traversal(treeNode *node1, treeNode *node2);
treeNode *createDeclVarNode(declType declVar, treeNode *expType);
treeNode *createArrayDeclVarNode(expType expNum, declType declVar, treeNode *expType);
treeNode *createDeclFuncNode(declType declFunc, treeNode *expType, treeNode *params, treeNode *blocDecl);
treeNode *createEmptyParams(expType expId);
treeNode *createArrayArg(declType declVar, treeNode *expType);
treeNode *createIfStmt(stmtType stmtIf, treeNode *exp, treeNode *stmt1, treeNode *stmt2);
treeNode *createWhileStmt(stmtType stmtWhile, treeNode *exp, treeNode *stmt);
treeNode *createAssignStmt(stmtType stmtAttrib, treeNode *var, treeNode *exp);
treeNode *createExpVar(expType expId);
treeNode *createArrayExpVar(expType expId, treeNode *exp);
treeNode *createExpOp(expType expOp, treeNode *exp1, treeNode *exp2);
treeNode *createExpNum(expType expNum);
treeNode *createActivationFunc(stmtType stmtFunc, treeNode *arguments, callList *list);
void initCallList(callList *list);
void insertNodeCallList(callList *list, char *name);
char *getLastNodeCallList(callList *list);
char *getTokenName(char *token);

#endif
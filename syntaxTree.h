#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include "utils.h"

#define CHILD_MAX_NODES 3

typedef enum { decl, exp, stmt } nodeType;
typedef enum { declFunc, declVar, declIdType } declType;
typedef enum { expId, expNum, expOp } expType;
typedef enum { stmtAttrib, stmtFunc, stmtIf, stmtReturn, stmtWhile } stmtType;

typedef enum { plus, minus, mult, slash, assign, lt, gt, lte, gte, dif, equal } opType;
typedef enum { Array, Boolean, Integer, Void } primitiveType;

typedef struct treeNode {
    struct treeNode *child[CHILD_MAX_NODES];
    struct treeNode *sibling;

    int line;
    int params;
    int args;
    nodeType node;
    primitiveType type;

    union { declType decl; expType exp; stmtType stmt; } nodeSubType;
    union { char *name; int value; opType op; } key;
} treeNode;

treeNode *syntaxTree;

void printSyntaxTree(treeNode *tree);
treeNode *parse();

#endif
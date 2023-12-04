#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#define CHILD_MAX_NODES 3

#include "utils.h"

/* Tokens types: decl (declaration), exp (expression), stmt (statement)*/
typedef enum { decl, exp, stmt } nodeType;
typedef enum { declFunc, declVar, declIdType } declType;
typedef enum { expId, expNum, expOp } expType;
typedef enum { stmtAttrib, stmtFunc, stmtIf, stmtReturn, stmtWhile } stmtType;

/* Operation types (+) (-) (*) (/) (=) (<) (>) (<=) (>=) (!=) (==) */
typedef enum { plus, minus, mult, slash, assign, lt, gt, lte, gte, dif, equal } opType;

/* Primitive data types */
typedef enum { Array, Boolean, Integer, Void } primitiveType;

/* Syntax tree  */
typedef struct treeNode {
    struct treeNode *child[CHILD_MAX_NODES];
    struct treeNode *sibling;

    int line;
    nodeType node;
    primitiveType type;

    union { declType decl; expType exp; stmtType stmt; } subType;
    union { char *name; int value; opType op; } key;
} treeNode;

void printSyntaxTree(treeNode *tree);
treeNode *parse();

treeNode *syntaxTree;

#endif /* SYNTAXTREE_H */

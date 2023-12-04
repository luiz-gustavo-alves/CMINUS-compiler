#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#define CHILD_MAX_NODES 3

#include "utils.h"
#include "parserHelper.h"

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

treeNode *parse();
treeNode *traversal(treeNode *node1, treeNode *node2);
treeNode *createDeclNode(declType node);
treeNode *createExpNode(expType node);
treeNode *createStmtNode(stmtType node);
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

void printSyntaxTree(treeNode *tree);

treeNode *syntaxTree;

#endif /* SYNTAXTREE_H */

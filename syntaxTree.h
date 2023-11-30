#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#define CHILD_MAX_NODES 3

/* Tokens types: decl (declaration), exp (expression), stmt (statement)*/
typedef enum { decl, exp, stmt } nodeType;
typedef enum { decl_func, decl_var, decl_type } declType;
typedef enum { exp_id, exp_num, exp_op } expType;
typedef enum { stmt_attrib, stmt_func, stmt_if, stmt_return, stmt_while } stmtType;

/* Primitive data types */
typedef enum { Array, Boolean, Integer, Void } primitiveType;

/* Operation types (+) (-) (*) (/) (=) (<) (>) (<=) (>=) (!=) (==) */
typedef enum { PLUS, MINUS, MULT, SLASH, ASSIGN, LT, GT, LTE, GTE, DIF, EQUAL } opType;

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
treeNode *createDeclNode(declType node, int line);
treeNode *createExpNode(expType node, int line);
treeNode *createStmtNode(stmtType node, int line);
void printSyntaxTree(treeNode *tree);

treeNode *syntaxTree;

#endif /* SYNTAXTREE_H */

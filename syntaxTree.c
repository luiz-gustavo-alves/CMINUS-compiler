#include "syntaxTree.h"
#include "scanner.h"
#include "token.h"
#include "utils.h"

int treeSize = 0;

treeNode *createNode() {

    treeNode *newNode = (treeNode*) malloc(sizeof(treeNode));
    int i;
    for (i = 0; i < CHILD_MAX_NODES; i++) {
        newNode->child[i] = NULL;
    }
    newNode->sibling = NULL;
    newNode->line = lineCount;
    return newNode;
}

treeNode *createDeclNode(declType node) {

    treeNode *newNode = createNode();
    newNode->node = decl;
    newNode->subType.decl = node;
    return newNode;
}

treeNode *createStmtNode(stmtType node) {

    treeNode *newNode = createNode();
    newNode->node = stmt;
    newNode->subType.stmt = node;
    return newNode;
}

treeNode *createExpNode(expType node) {

    treeNode *newNode = createNode();
    newNode->node = exp;
    newNode->subType.exp = node;
    return newNode;
}

void printSyntaxTree(treeNode *tree) { 

	treeSize += CHILD_MAX_NODES;
    while (tree != NULL) {
		int i;
        for (i = 0; i < treeSize; i++) {
            printf(" ");
        }

        if (tree->node == exp) {

            switch (tree->subType.exp) {
                case expNum: printf("NUM %d\n", tree->key.value); break;

                case expId:
                    if (strcmp(tree->key.name, "void") == 1) printf("void \n");
                    else printf("ID %s\n", tree->key.name);
                    break;

                case expOp:
                    printf("%s \n", tokenNames[0][tree->key.op - 1]);
                    break;

                default: printf("INVALID EXPRESION");
            }
        } else if (tree->node == stmt) { 

			switch (tree->subType.stmt) {
                case stmtIf: printf("if \n"); break;
                case stmtWhile: printf("while \n"); break;
                case stmtAttrib: printf("= \n"); break;
                case stmtReturn: printf("return \n"); break;
                case stmtFunc: printf("Function Call: %s\n", tree->key.name); break;
                default: printf("INVALID STATEMENT \n");
            }
        } else if (tree->node == decl) {

			switch(tree->subType.decl) {
                case declFunc: printf("Function Declaration: %s\n", tree->key.name); break;
                case declVar: printf("Variable Declaration: %s\n", tree->key.name); break;
                case declIdType:
                    if (tree->type == Integer) printf("int \n");
                    else if(tree->type == Array) printf("int[] \n");
                    else printf("void \n");
                    break;

                default: printf("INVALID DECLARATION \n");
            }
        }
        for (i = 0; i < CHILD_MAX_NODES; i++) {
            printSyntaxTree(tree->child[i]);
        }
        tree = tree->sibling;
    }
    treeSize -= CHILD_MAX_NODES;
}

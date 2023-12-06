#include "syntaxTree.h"
#include "token.h"

treeNode *syntaxTree;
int treeSize = 0;

void printTreeNode(treeNode *tree) {

    treeSize += 2;
    int i;
    for (i = 0; i < treeSize; i++) {
        printf(" ");
    }

    if (tree->node == exp) {
        switch (tree->nodeSubType.exp) {
            case expNum: printf("NUM: %d\n", tree->key.value); break;
            case expId:
                if (strcmp(tree->key.name, "void") == 1) printf("void \n");
                else printf("ID: %s\n", tree->key.name);
                break;
            case expOp:
                printf("OPERATOR: %s \n", tokenNames[0][tree->key.op - 1]);
                break;
            default: printf("INVALID EXPRESION");
        }
    } else if (tree->node == stmt) { 
        switch (tree->nodeSubType.stmt) {
            case stmtIf: printf("if \n"); break;
            case stmtWhile: printf("while \n"); break;
            case stmtAttrib: printf("ASSIGN \n"); break;
            case stmtReturn: printf("return \n"); break;
            case stmtFunc: printf("Function Call: %s\n", tree->key.name); break;
            default: printf("INVALID STATEMENT \n");
        }
    } else if (tree->node == decl) {
        switch(tree->nodeSubType.decl) {
            case declFunc: printf("Function Declaration: %s\n", tree->key.name); break;
            case declVar: printf("Variable Declaration: %s\n", tree->key.name); break;
            case declIdType:
                if (tree->type == Integer) printf("Type int \n");
                else if(tree->type == Array) printf("Type int[] \n");
                else printf("Type void \n");
                break;
            default: printf("INVALID DECLARATION \n");
        }
    }
}

void printSyntaxTree(treeNode *tree) {
    if (tree == NULL) {
        return;
    }

    printTreeNode(tree);

    int i;
    for (i = 0; i < CHILD_MAX_NODES; i++) {
        printSyntaxTree(tree->child[i]);
    }
    treeSize -= 2;
    printSyntaxTree(tree->sibling);
}
#include "syntaxTree.h"
#include "token.h"

int treeSize = 0;

void printSyntaxTree(treeNode *tree) { 

	treeSize += CHILD_MAX_NODES;
    while (tree != NULL) {
		int i;
        for (i = 0; i < treeSize; i++) {
            printf(" ");
        }

        if (tree->node == exp) {

            switch (tree->subType.exp) {
                case expNum: printf("NUM: %d\n", tree->key.value); break;

                case expId:
                    if (strcmp(tree->key.name, "void") == 1) printf("void \n");
                    else printf("ID: %s\n", tree->key.name);
                    break;

                case expOp:
                    printf("OPERATOR: %s \n", tokenNames[1][tree->key.op - 1]);
                    break;

                default: printf("INVALID EXPRESION");
            }
        } else if (tree->node == stmt) { 

			switch (tree->subType.stmt) {
                case stmtIf: printf("if \n"); break;
                case stmtWhile: printf("while \n"); break;
                case stmtAttrib: printf("ASSIGN \n"); break;
                case stmtReturn: printf("return \n"); break;
                case stmtFunc: printf("Function Call: %s\n", tree->key.name); break;
                default: printf("INVALID STATEMENT \n");
            }
        } else if (tree->node == decl) {

			switch(tree->subType.decl) {
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
        for (i = 0; i < CHILD_MAX_NODES; i++) {
            printSyntaxTree(tree->child[i]);
        }
        tree = tree->sibling;
    }
    treeSize -= CHILD_MAX_NODES;
}

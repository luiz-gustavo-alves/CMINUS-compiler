#include "syntaxTree.h"
#include "utils.h"

int treeSize = 0;

treeNode *createNode(int currentLine) {

    treeNode *newNode = (treeNode*) malloc(sizeof(treeNode));
    int i;
    for (i = 0; i < CHILD_MAX_NODES; i++) {
        newNode->child[i] = NULL;
    }
    newNode->sibling = NULL;
    newNode->line = currentLine;
    return newNode;
}

treeNode *createDeclNode(declType node, int currentLine) {

    treeNode *newNode = createNode(currentLine);
    newNode->node = decl;
    newNode->subType.decl = node;
    return newNode;
}

treeNode *createStmtNode(stmtType node, int currentLine) {

    treeNode *newNode = createNode(currentLine);
    newNode->node = stmt;
    newNode->subType.stmt = node;
    return newNode;
}

treeNode *createExpNode(expType node, int currentLine) {

    treeNode *newNode = createNode(currentLine);
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
                case exp_num: printf("NUM: %d\n", tree->key.value); break;

                case exp_id:
                    if (strcmp(tree->key.name, "void")) printf("void \n");
                    else printf("ID: %s\n", tree->key.name);
                    break;

                case exp_op:
                    switch (tree->key.op) {
                        case PLUS: printf("+ \n"); break;
                        case MINUS: printf("- \n"); break;
                        case MULT: printf("* \n"); break;
                        case SLASH: printf("/ \n"); break;
                        case ASSIGN: printf("= \n"); break;
                        case LT: printf("< \n"); break;
                        case GT: printf("> \n"); break;
                        case LTE: printf("<= \n"); break;
                        case GTE: printf(">= \n"); break;
                        case DIF: printf("!= \n"); break;
                        case EQUAL: printf("== \n"); break;
                        default: printf("INVALID OPERATOR \n"); break;
                    }
                    break;

                default: printf("INVALID EXPRESION");
            }
        } else if (tree->node == stmt) { 

			switch (tree->subType.stmt) {
                case stmt_if: printf("if \n"); break;
                case stmt_while: printf("while \n"); break;
                case stmt_attrib: printf("assign \n"); break;
                case stmt_return: printf("return \n"); break;
                case stmt_func: printf("Function Call: %s\n", tree->key.name); break;
                default: printf("INVALID STATEMENT \n");
            }
        } else if (tree->node == decl) {

			switch(tree->subType.decl) {
                case decl_func: printf("Function Declaration: %s\n", tree->key.name); break;
                case decl_var: printf("Variable Declaration: %s\n", tree->key.name); break;
                case decl_type:
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

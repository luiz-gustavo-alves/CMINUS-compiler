#ifndef ANALYZE_H
#define ANALYZE_H

#define COMP_EQUAL(str1, str2) (strcmp(str1, str2) == 0)
#define COMP_DIF(str1, str2) (strcmp(str1, str2) == 1)

#include "utils.h"
#include "syntaxTree.h"

static char* currentScope = "global";

void semanticAnalysis(treeNode *tree);
void preOrderTraversal(treeNode *tree);
void postOrderTraversal(treeNode *tree);
void checkNodesDeclaration(treeNode *tree);
void checkNodesTypes(treeNode *tree);
int throwSemanticError(treeNode *tree, char *msg);

#endif /* ANALYZE_H */
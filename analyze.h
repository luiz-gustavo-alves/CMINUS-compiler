#ifndef ANALYZE_H
#define ANALYZE_H

#include "utils.h"
#include "syntaxTree.h"

void semanticAnalysis(treeNode *tree);
void preOrderTraversal(treeNode *tree);
void checkPreOrderNode(treeNode *tree);
void postOrderTraversal(treeNode *tree);
void checkPostOrderNode(treeNode * tree);
void throwSemanticError(treeNode *tree, char *msg);

#endif /* ANALYZE_H */
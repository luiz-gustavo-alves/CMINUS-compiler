#ifndef ANALYZE_H
#define ANALYZE_H

#include "utils.h"
#include "syntaxTree.h"

void tree_insert_node(treeNode *tree);
void tree_post_order(treeNode *tree);
void tree_pre_order(treeNode *tree);
void verify_node_kind(treeNode *tree);
void verify_nodes(treeNode * tree);
void sym_tab_build(treeNode *tree);
void show_error(treeNode *tree, char *msg);

#endif /* ANALYZE_H */
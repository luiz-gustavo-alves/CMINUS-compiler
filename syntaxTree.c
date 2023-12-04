#include "syntaxTree.h"
#include "scanner.h"
#include "parserHelper.h"
#include "token.h"
#include "utils.h"

int treeSize = 0;

treeNode *traversal(treeNode *node1, treeNode *node2) {
    treeNode *temp = node1;
    if (temp != NULL) {
        while (temp->sibling != NULL) {
            temp = temp->sibling;
        }
        temp->sibling = node2;
        return node1;
    } else {
        return node2;
    }
}

treeNode *traversalLeftChild(treeNode *node1, treeNode *node2) {
    treeNode *temp = node1;
    if (temp != NULL) {
        while (temp->child[0]->sibling != NULL) {
            temp = temp->child[0]->sibling;
        }
        temp->child[0]->sibling = node2;
        return node1;
    } else {
        return node2;
    }
}

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

treeNode *createDeclVarNode(declType declVar, treeNode *expType) {
    treeNode *declVarNode = createDeclNode(declVar);
    declVarNode->key.name = expName;
    declVarNode->line = currentLine;
    declVarNode->type = expType->type;

    expType->child[0] = declVarNode;
    return expType;
}

treeNode *createArrayDeclVarNode(expType expNum, declType declVar, treeNode *expType) {
    treeNode *expNumNode = createExpNode(expNum);
    expNumNode->key.value = atoi(tokenNUM);
    expNumNode->type = Integer;

    treeNode *declVarNode = createDeclNode(declVar);
    declVarNode->key.name = expName;
    declVarNode->line = currentLine;
    declVarNode->child[0] = expNumNode;

    if (expType->type == Integer) {
        declVarNode->type = Array;
    } else {
        declVarNode->type = Void;    
    }

    expType->child[0] = declVarNode;
    return expType;
}

treeNode *createDeclFuncNode(declType declFunc, treeNode *expType, treeNode *params, treeNode *blocDecl) {
    treeNode* declFuncNode = createDeclNode(declFunc);
    declFuncNode->child[0] = params;
    declFuncNode->child[1] = blocDecl;
    declFuncNode->key.name = functionName;
    declFuncNode->line = functionCurrentLine;
    declFuncNode->type = expType->type;
                
    expType->child[0] = declFuncNode;
    return expType;
}

treeNode *createEmptyParams(expType expId) {
    treeNode *node = createExpNode(expId);
    node->key.name = "void";
    node->type = Void;
    return node;
}

treeNode *createArrayArg(declType declVar, treeNode *expType) {
    treeNode *declVarNode = createDeclNode(declVar);
    declVarNode->key.name = expName;
    declVarNode->line = currentLine;
            
    if (expType->type == Integer) {
        declVarNode->type = Array;
    }
    else {
        declVarNode->type = expType->type;
    }

    expType->child[0] = declVarNode;
    return expType;
}

treeNode *createIfStmt(stmtType stmtIf, treeNode *exp, treeNode *stmt1, treeNode *stmt2) {
    treeNode *stmtIfNode = createStmtNode(stmtIf);
    stmtIfNode->child[0] = exp;
    stmtIfNode->child[1] = stmt1;
    if (stmt2 != NULL) {
        stmtIfNode->child[2] = stmt2;
    }
    return stmtIfNode;
}

treeNode *createWhileStmt(stmtType stmtWhile, treeNode *exp, treeNode *stmt) {
    treeNode *stmtWhileNode = createStmtNode(stmtWhile);
    stmtWhileNode->child[0] = exp;
    stmtWhileNode->child[1] = stmt;
    stmtWhileNode->type = Boolean;
    return stmtWhileNode;
}

treeNode *createAssignStmt(stmtType stmtAttrib, treeNode *var, treeNode *exp) {
    treeNode *stmtAttribNode = createStmtNode(stmtAttrib);
    stmtAttribNode->child[0] = var;
    stmtAttribNode->child[1] = exp;
    stmtAttribNode->type = Integer;
    return stmtAttribNode;
}

treeNode *createExpVar(expType expId) {
    treeNode *expVarNode = createExpNode(expId);
    expVarNode->key.name = expName;
    expVarNode->line = currentLine;
    expVarNode->type = Void;
    return expVarNode;
}

treeNode *createArrayExpVar(expType expId, treeNode *exp) {
    treeNode *expVarNode = createExpNode(expId);
    expVarNode->key.name = variableName;
    expVarNode->line = currentLine;
    expVarNode->child[0] = exp;
    expVarNode->type = Integer;
    return expVarNode;
}

treeNode *createExpOp(expType expOp, treeNode *exp1, treeNode *exp2) {
    treeNode *expOpNode = createExpNode(expOp);
    expOpNode->child[0] = exp1;
    expOpNode->child[1] = exp2;
    return expOpNode;
}

treeNode *createExpNum(expType expNum) {
    treeNode *expNumNode = createExpNode(expNum);
    expNumNode->key.value = atoi(tokenNUM);
    expNumNode->type = Integer;
    return expNumNode;
}

treeNode *createActivationFunc(stmtType stmtFunc, treeNode *arguments, callList *list) {
    treeNode *activationFuncNode = createStmtNode(stmtFunc);
    activationFuncNode->child[1] = arguments; 
    activationFuncNode->key.name = getLastNodeCallList(list);
    activationFuncNode->line = currentLine;
    return activationFuncNode;
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

#include "syntaxTree.h"
#include "parserHelper.h"
#include "scanner.h"
#include "token.h"

char *expName;
char *variableName;
char *functionName;
char *currentToken;

treeNode *createNode() {
    treeNode *newNode = (treeNode*) malloc(sizeof(treeNode));
    int i;
    for (i = 0; i < CHILD_MAX_NODES; i++) {
        newNode->child[i] = NULL;
    }
    newNode->sibling = NULL;
    newNode->line = currentLine;
    return newNode;
}

treeNode *createDeclNode(declType node) {
    treeNode *newNode = createNode();
    newNode->node = decl;
    newNode->nodeSubType.decl = node;
    return newNode;
}

treeNode *createStmtNode(stmtType node) {
    treeNode *newNode = createNode();
    newNode->node = stmt;
    newNode->nodeSubType.stmt = node;
    return newNode;
}

treeNode *createExpNode(expType node) {
    treeNode *newNode = createNode();
    newNode->node = exp;
    newNode->nodeSubType.exp = node;
    return newNode;
}

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

treeNode *createDeclVarNode(declType declVar, treeNode *expType) {
    treeNode *declVarNode = createDeclNode(declVar);
    declVarNode->key.name = expName;
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
    declFuncNode->params = paramsCount;

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
    expVarNode->type = Void;
    return expVarNode;
}

treeNode *createArrayExpVar(expType expId, treeNode *exp) {
    treeNode *expVarNode = createExpNode(expId);
    expVarNode->key.name = variableName;
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
    activationFuncNode->args = argsCount;
    return activationFuncNode;
}

void initCallList(callList *list) {
    list->head = NULL;
    list->tail = NULL;
}

void insertNodeCallList(callList *list, char *name) {

    callListNode *node = (callListNode *)malloc(sizeof(callListNode));
    node->funcName = name;
    node->prev = list->tail;
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    }
    else {
        list->tail->next = node;
        list->tail = node;
    }
}

char *getLastNodeCallList(callList *list) {
    if (list->tail == NULL) {
        return NULL;
    }

    if (list->head == list->tail) {
        callListNode *lastNode = list->tail;
        list->head = NULL;
        list->tail = NULL;
        return lastNode->funcName;
    }

    callListNode *lastNode = list->tail;
    list->tail = lastNode->prev;

    if (list->tail != NULL) {
        list->tail->next = NULL;
    }

    if (lastNode != list->head) {
        lastNode->prev = NULL;
    }

    return lastNode->funcName;
}

char *getTokenName(char *token) {

    char *tokenName = malloc(strlen(token) + 1);
    strcpy(tokenName, token);
    return tokenName;
}

#include "utils.h"
#include "syntaxTree.h"
#include "parserHelper.h"

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

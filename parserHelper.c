#include "utils.h"
#include "parserHelper.h"

callList *head = NULL;

void insertNode(callList *list, char *name) {

	callList *newItem = (callList*) malloc(sizeof(callList));
  newItem->name = name;
  if (head == NULL) {
    head = newItem;
  } else {
    callList *temp = head;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = newItem;
  }
}

char *getLastNode(callList *list) {

	if (head == NULL) return NULL;
  if (head->next == NULL) {
    char *name = strdup(head->name);
    free(head);
    return name;
  }

  callList *secondLast = head;
  while (secondLast->next->next != NULL)
      secondLast = secondLast->next;
  
  char *name = strdup(secondLast->next->name);
  free(secondLast->next);
  secondLast->next = NULL;

  return name;
}

char *getTokenName(char *token) { 

  char *tokenName = malloc(strlen(token) + 1);
  strcpy(tokenName, token);
  return tokenName;
}
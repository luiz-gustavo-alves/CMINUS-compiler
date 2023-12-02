#include "utils.h"
#include "parserHelper.h"

void initStack(stack *st) {

	st = (stack*)malloc(sizeof(stack));
	st->top = NULL;
}

void push(stack *st, char *name) {

	stack *newItem = (stack*) malloc(sizeof(stack));	
	newItem->next = st->top;
	newItem->name = strdup(name);
	st->top = newItem;
}

char *pop(stack *st) {
	
	if (st == NULL) return NULL;

	stack *st_old_top;
    char *name = strdup(st->top->name);
    st_old_top = st->top;
    st->top = st_old_top->next;

    free(st_old_top);
	return name;
}
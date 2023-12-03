
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *file;

int lexicalError;
int syntaxError;
int semanticError;
static char* currentScope = "global";

/* Primitive data types */
typedef enum { Array, Boolean, Integer, Void } primitiveType;

#endif /* UTILS_H */

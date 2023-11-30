#ifndef SCANNER_H
#define SCANNER_H

#include "utils.h"

#define DFA_STATES_COUNT 49
#define DFA_TRANSITIONS_COUNT 33
#define BUFFER_MAX_LENGTH 256

char *getToken(int finalState);
int isSpecialChar(char c);
int isLetterState(int state);
int isDigitState(int state);
int isCommentState(int state);
int getNextDFAstate(int dfaTable[DFA_STATES_COUNT][DFA_TRANSITIONS_COUNT], char c, int state);
int lexicalAnalysis(FILE *file);

#endif /* SCANNER_H */

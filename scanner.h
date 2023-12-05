#ifndef SCANNER_H
#define SCANNER_H

#include "utils.h"
#include "token.h"

#define DFA_STATES_COUNT 26
#define DFA_TRANSITIONS_COUNT 19
#define TOKEN_MAX_LENGTH 26

extern int lineCount;
extern int currentState;
extern int previousState;
extern char currentChar;
extern char previousChar;

extern char lexem[TOKEN_MAX_LENGTH];
extern char tokenID[TOKEN_MAX_LENGTH]; 
extern char tokenNUM[TOKEN_MAX_LENGTH];

extern int dfaTable[DFA_STATES_COUNT][DFA_TRANSITIONS_COUNT];
extern int finalStates[DFA_STATES_COUNT];

int getTokenType(int finalState);
int isSpecialChar(char c);
int isLetterState(int state);
int isDigitState(int state);
int isCommentState(int state);
int getNextDFAstate(int dfaTable[DFA_STATES_COUNT][DFA_TRANSITIONS_COUNT], char c, int state);
token getToken(int finalState);
token lexicalAnalysis();

#endif 
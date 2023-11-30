#include "utils.h"
#include "scanner.h"

int isSpecialChar(char c) {
	return (isspace(c) || c == '\n' || c == '\0' || c == '\t');
}

int isLetterState(int state) {
	return (state == 44 && state == 44);
}

int isDigitState(int state) {
	return (state == 45 && state == 45);
}

int isCommentState(int state) {
	return (state >= 28 && state <= 30);
}

int getNextDFAstate(int dfaTable[DFA_STATES_COUNT][DFA_TRANSITIONS_COUNT], char c, int state) {

	switch (c) {
		case 'i': return dfaTable[state][0];
		case 'r': return dfaTable[state][1];
		case 'v': return dfaTable[state][2];
		case 'e': return dfaTable[state][3];
		case 'w': return dfaTable[state][4];
		case 'f': return dfaTable[state][5];
		case 'n': return dfaTable[state][6];
		case 't': return dfaTable[state][7];
		case 'u': return dfaTable[state][8];
		case 'o': return dfaTable[state][9];
		case 'd': return dfaTable[state][10];
		case 'l': return dfaTable[state][11];
		case 's': return dfaTable[state][12];
		case 'h': return dfaTable[state][13];
		case '+': return dfaTable[state][14];
		case '-': return dfaTable[state][15];
		case '*': return dfaTable[state][16];
		case '/': return dfaTable[state][17];
		case '<': return dfaTable[state][18];
		case '>': return dfaTable[state][19];
		case '=': return dfaTable[state][20];
		case '!': return dfaTable[state][21];
		case ';': return dfaTable[state][22];
		case ',': return dfaTable[state][23];
		case '(': return dfaTable[state][24];
		case ')': return dfaTable[state][25];
		case '[': return dfaTable[state][26];
		case ']': return dfaTable[state][27];
		case '{': return dfaTable[state][28];
		case '}': return dfaTable[state][29];
		default: break;
	}

	/* Check if char is letter */
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
		return dfaTable[state][30];
	}

	/* Check if char is digit */
	if (c >= 48 && c <= 57) {
		return dfaTable[state][31];
	}

	/* Check if char is special (whitespace, newline, tab, null terminator) */
	if (isSpecialChar(c) && !isCommentState(state)) {
		return 0;
	}

	/* Other char */
	return dfaTable[state][32];
}

char *getToken(int finalState) {

	/* Edge cases (Check possible IDENTIFIERS in reserved words) */
	int isIDfrom_IF_or_INT_DFA = (finalState == 1 || finalState == 3);
	int isIDfrom_RETURN_DFA = (finalState >= 5 && finalState <= 8);
	int isIDfrom_VOID_DFA = (finalState >= 11 && finalState <= 13);
	int isIDfrom_ELSE_DFA = (finalState >= 15 && finalState <= 17);
	int isIDfrom_WHERE_DFA = (finalState >= 19 && finalState <= 22);

	if (isIDfrom_IF_or_INT_DFA || isIDfrom_RETURN_DFA || isIDfrom_VOID_DFA || isIDfrom_ELSE_DFA || isIDfrom_WHERE_DFA) {
		return "IDENTIFIER";
	}

	switch (finalState) {
		case 2: return "IF";
		case 4: return "INT";
		case 10: return "RETURN";
		case 14: return "VOID";
		case 18: return "ELSE";
		case 23: return "WHILE";
		case 24: return "PLUS";
		case 25: return "MINUS";
		case 26: return "MULT";
		case 27: return "SLASH";
		case 31: return "ASSIGN";
		case 32: return "EQUAL";
		case 33: return "LT";
		case 34: return "GT";
		case 36: return "SEMICOLON";
		case 37: return "COMMA";
		case 38: return "OPARENT";
		case 39: return "CPARENT";
		case 40: return "OBRACKET";
		case 41: return "CBRACKET";
		case 42: return "OKEY";
		case 43: return "CKEY";
		case 44: return "IDENTIFIER";
		case 45: return "NUMBER";
		case 46: return "LTE";
		case 47: return "GTE";
		case 48: return "DIF";
		default: return "INVALID TOKEN";
	}
}

int lexicalAnalysis(FILE *file) {
	
	printf("\n* * * * * FASE (1): ANALISE LEXICA * * * * *\n\n");

	int dfaTable[DFA_STATES_COUNT][DFA_TRANSITIONS_COUNT] = {
		{1, 5, 11, 15, 19, 44, 44, 44, 44, 44, 44, 44, 44, 44, 24, 25, 26, 27, 33, 34, 31, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, -1},
		{44, 44, 44, 44, 44, 2, 3, 44, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 44, 4, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 6, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 44, 7, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 44, 44, 8, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 9, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 10, 44, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 44, 44, 44, 12, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{13, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 14, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 16, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 17, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 18, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{21, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 22, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 23, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 29, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28},
		{28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 30, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 48, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, -1, -1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 45, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
	};

	int finalStates[DFA_STATES_COUNT] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

	int currentState = 0;
	int previousState = 0;
	int lineCount = 1;
	int lastLineCount = 1;
	int lexicalError = 1;

	char currentChar = '\0';
	char previousChar = '\0';
	char lexem[BUFFER_MAX_LENGTH];
	memset(lexem, 0, sizeof(lexem));

	/* Lexical Analysis */
	while ((currentChar = fgetc(file)) != EOF) {

		/* Get new state from DFA table */
		currentState = getNextDFAstate(dfaTable, currentChar, currentState);

		/* Ignore comments */
		int isComment = isCommentState(currentState);
		if (!isComment) {

			/* Concat Lexem String */
			if (!isSpecialChar(currentChar)) {
				strncat(lexem, &currentChar, 1);
				lastLineCount = lineCount;
			}
			else {
				memset(lexem, 0, sizeof(lexem));
			}

			/* Check if previous state is a Final State */
			if (currentState == 0) {
				int isFinalState = finalStates[previousState];
				if (isFinalState)
				{
					char *token = getToken(previousState);
					printf("Linha: %d | Token: %s\n", lineCount, token);
				}
				currentState = getNextDFAstate(dfaTable, currentChar, currentState);
				previousState = currentState;
			}

			/* Invalid state (Lexical Error) */
			if (currentState == -1) {
				printf("(!) ERRO LEXICO: Lexema: %s | Linha: %d", lexem, lineCount);
				return lexicalError;
			}
		}

		previousState = currentState;
		if (currentChar == '\n') {
			lineCount++;
		}
	}

	/* Check and validate last character from file */
	if (!lexicalError) {
		currentState = getNextDFAstate(dfaTable, previousChar, currentState);

		if (isCommentState(currentState)) {
			printf("(!) ERRO LEXICO: unterminated string");
			return lexicalError;
		}

		if (currentState == -1) {
			printf("(!) ERRO LEXICO: Lexema: %s | Linha: %d", lexem, lineCount);
			return lexicalError;
		}

		if (currentState == 0) {
			int isFinalState = finalStates[previousState];
			if (isFinalState) {
				char *token = getToken(previousState);
				printf("Linha: %d | Token: %s\n", lineCount, token);
			}
		}
	}

	return 0;
}
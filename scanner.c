#include "utils.h"
#include "scanner.h"
#include "token.h"

#include <ctype.h>
#include <limits.h>

int isSpecialChar(char c) {
	return (isspace(c) || c == '\n' || c == '\0' || c == '\t');
}

int isLetterChar(char c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int isLetterState(int state) {
	return (state == 44);
}

int isDigitChar(char c) {
	return (c >= '0' && c <= '9');
}

int isDigitState(int state) {
	return (state == 45);
}

int isCommentState(int state) {
	return (state >= 28 && state <= 30);
}

int getNextDFAstate(int dfaTable[DFA_STATES_COUNT][DFA_TRANSITIONS_COUNT], char c, int state) {

	/* Check if char is special (whitespace, newline, tab, null terminator) */
	if (isSpecialChar(c) && !isCommentState(state)) {
		return 0;
	}

	/* Check if char is still in letter state */
	if (isLetterChar(c) && isLetterState(state)) {
		return dfaTable[state][30];
	}

	/* Check if char is still in digit state */
	if (isDigitChar(c) && isDigitState(state)) {
		return dfaTable[state][31];
	}

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
	if (isLetterChar(c)) {
		return dfaTable[state][30];
	}

	/* Check if char is digit */
	if (isDigitChar(c)) {
		return dfaTable[state][31];
	}

	/* Other char */
	return dfaTable[state][32];
}

int getTokenType(int finalState) {

	/* Edge cases (Check possible IDENTIFIERS in reserved words) */
	int isIDfrom_IF_or_INT_DFA = (finalState == 1 || finalState == 3);
	int isIDfrom_RETURN_DFA = (finalState >= 5 && finalState <= 8);
	int isIDfrom_VOID_DFA = (finalState >= 11 && finalState <= 13);
	int isIDfrom_ELSE_DFA = (finalState >= 15 && finalState <= 17);
	int isIDfrom_WHERE_DFA = (finalState >= 19 && finalState <= 22);

	if (isIDfrom_IF_or_INT_DFA || 
		isIDfrom_RETURN_DFA || 
		isIDfrom_VOID_DFA || 
		isIDfrom_ELSE_DFA || 
		isIDfrom_WHERE_DFA || 
		finalState == 44) {
		strncpy(tokenID, lexem, TOKEN_MAX_LENGTH);
		return TOKEN_IDENTIFIER;
	}

	if (finalState == 45) {
		strncpy(tokenNUM, lexem, TOKEN_MAX_LENGTH); 
		return TOKEN_NUMBER;
	}

	switch (finalState) {
		case 2: return TOKEN_IF;
		case 4: return TOKEN_INT;
		case 10: return TOKEN_RETURN;
		case 14: return TOKEN_VOID;
		case 18: return TOKEN_ELSE;
		case 23: return TOKEN_WHILE;
		case 24: return TOKEN_PLUS;
		case 25: return TOKEN_MINUS;
		case 26: return TOKEN_MULT;
		case 27: return TOKEN_SLASH;
		case 31: return TOKEN_ASSIGN;
		case 32: return TOKEN_EQUAL;
		case 33: return TOKEN_LT;
		case 34: return TOKEN_GT;
		case 36: return TOKEN_SEMICOLON;
		case 37: return TOKEN_COMMA;
		case 38: return TOKEN_OPARENT;
		case 39: return TOKEN_CPARENT;
		case 40: return TOKEN_OBRACKET;
		case 41: return TOKEN_CBRACKET;
		case 42: return TOKEN_OKEY;
		case 43: return TOKEN_CKEY;
		case 46: return TOKEN_LTE;
		case 47: return TOKEN_GTE;
		case 48: return TOKEN_DIF;
		default: return 0;
	}
}

token getToken(int finalState) {

	int type = getTokenType(finalState);
	struct token token;
	token.type = type;
	token.line = lineCount;

	if (type == TOKEN_IDENTIFIER) {
		printf("Linha: %d | Token: %s | Lexema: %s\n", lineCount, tokenNames[1][type - 1], tokenID);
	}
	else if (type == TOKEN_NUMBER) {
		printf("Linha: %d | Token: %s | Lexema: %s\n", lineCount, tokenNames[1][type - 1], tokenNUM);
	}
	else { 
		printf("Linha: %d | Token: %s | Lexema: %s\n", lineCount, tokenNames[1][type - 1], tokenNames[0][type - 1]);
	}

	return token;
}

token lexicalAnalysis() {

	struct token errTk = { 0 };
	while ((currentChar = fgetc(file)) != EOF) {

		/* Get new state from DFA table */
		currentState = getNextDFAstate(dfaTable, currentChar, currentState);
		strncpy(&previousChar, &currentChar, 1);

		int isComment = isCommentState(currentState);
		if (!isComment) {

			/* Concat Lexem for IDENTIFIERS and NUMBERS tokens */
			if ((isLetterChar(currentChar) || isDigitChar(currentChar))) {
				strncat(lexem, &currentChar, 1);

				if (isLetterChar(currentChar)) {

					/* Check identifier name overflow */
					if (strlen(lexem) > TOKEN_MAX_LENGTH) {
						printf("(!) ERRO LEXICO: Tamanho maximo para identificadores excedido");
						return errTk;
					}
				} else {

					/* Check integer overflow */
					long int number = strtol(lexem, NULL, 10);
					if (number > INT_MAX) {
						printf("(!) ERRO LEXICO: Tamanho maximo para representacao numerica inteira excedido");
						return errTk;
					}
				}
			}

			/* Check if previous state is a Final State */
			if (currentState == 0) {
				int isFinalState = finalStates[previousState];
				if (isFinalState)
				{
					struct token token = getToken(previousState);
					currentState = getNextDFAstate(dfaTable, currentChar, currentState);
					previousState = currentState;

					if (currentChar == '\n') {
						lineCount++;
					}

					if (!(isLetterChar(currentChar) || isDigitChar(currentChar))) {
						memset(lexem, 0, sizeof(TOKEN_MAX_LENGTH));
					}

					return token;
				}
			}

			/* Invalid state (Lexical Error) */
			if (currentState == -1) {
				printf("(!) ERRO LEXICO: Lexema: %s | Linha: %d", lexem, lineCount);
				return errTk;
			}
		}

		previousState = currentState;
		if (previousChar == '\n') {
			lineCount++;
		}
	}

	struct token END = { .type = 256 };
	if (isSpecialChar(previousChar)) {
		return END;
	}

	/* Check last character from file */
	currentState = getNextDFAstate(dfaTable, previousChar, currentState);
	if (isCommentState(currentState)) {
		printf("(!) ERRO LEXICO: literal de string nao terminada");
		return errTk;
	}

	if (currentState == -1) {
		printf("(!) ERRO LEXICO: Lexema: %s | Linha: %d", lexem, lineCount);
		return errTk;
	}

	if (currentState == 0) {
		int isFinalState = finalStates[previousState];
		if (isFinalState)
		{
			struct token token = getToken(previousState);
			return token;
		}
	}

	/* Finish Lexical Analysis */
	return END;
}
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
	return (state == 16);
}

int isDigitChar(char c) {
	return (c >= '0' && c <= '9');
}

int isDigitState(int state) {
	return (state == 17);
}

int isCommentState(int state) {
	return (state > 4 && state < 7);
}

int getNextDFAstate(int dfaTable[DFA_STATES_COUNT][DFA_TRANSITIONS_COUNT], char c, int state) {

	/* Check if char is special (whitespace, newline, tab, null terminator) */
	if (isSpecialChar(c) && !isCommentState(state)) {
		return 0;
	}

	if (isLetterChar(c)) {
		return dfaTable[state][16];
	}

	if (isDigitChar(c)) {
		return dfaTable[state][17];
	}

	switch (c) {
		case '+': return dfaTable[state][0];
		case '-': return dfaTable[state][1];
		case '*': return dfaTable[state][2];
		case '/': return dfaTable[state][3];
		case '=': return dfaTable[state][4];
		case '<': return dfaTable[state][5];
		case '>': return dfaTable[state][6];
		case '!': return dfaTable[state][7];
		case ';': return dfaTable[state][8];
		case ',': return dfaTable[state][9];
		case '(': return dfaTable[state][10];
		case ')': return dfaTable[state][11];
		case '[': return dfaTable[state][12];
		case ']': return dfaTable[state][13];
		case '{': return dfaTable[state][14];
		case '}': return dfaTable[state][15];
		default: break;
	}

	/* Other char */
	return dfaTable[state][18];
}

int getTokenType(int finalState) {

	if (finalState == 24) {
		int reservedType = isReservedWord(lexem);

		/* Letter is reserved word */
		if (reservedType != 0) {
			return reservedType;
		}

		/* Letter is Identifier */
		strncpy(tokenID, lexem, TOKEN_MAX_LENGTH);
		return TOKEN_IDENTIFIER;
	}

	if (finalState == 25) {
		strncpy(tokenNUM, lexem, TOKEN_MAX_LENGTH); 
		return TOKEN_NUMBER;
	}

	switch (finalState) {
		case 1: return TOKEN_PLUS;
		case 2: return TOKEN_MINUS;
		case 3: return TOKEN_MULT;
		case 4: return TOKEN_SLASH;
		case 8: return TOKEN_ASSIGN;
		case 9: return TOKEN_EQUAL;
		case 10: return TOKEN_LT;
		case 11: return TOKEN_LTE;
		case 12: return TOKEN_GT;
		case 13: return TOKEN_GTE;
		case 15: return TOKEN_DIF;
		case 16: return TOKEN_SEMICOLON;
		case 17: return TOKEN_COMMA;
		case 18: return TOKEN_OPARENT;
		case 19: return TOKEN_CPARENT;
		case 20: return TOKEN_OBRACKET;
		case 21: return TOKEN_CBRACKET;
		case 22: return TOKEN_OKEY;
		case 23: return TOKEN_CKEY;
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
				if (isFinalState) {
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

				} else if (!isSpecialChar(currentChar)) {
					printf("(!) ERRO LEXICO: Lexema: %s | Linha: %d", lexem, lineCount);
					return errTk;
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
		if (isFinalState) {
			struct token token = getToken(previousState);
			return token;
		}
	}

	/* Finish Lexical Analysis */
	return END;
}
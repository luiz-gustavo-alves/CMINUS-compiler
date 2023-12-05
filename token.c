#include "token.h"
#include "utils.h"

int reservedHashKey;
reservedWords reservedHashValue;

char *tokenNames[2][27] = {
  {"if", "int", "return", "void", "else", "while", "+", "-", "*", "/", "=", "==", "<", ">", "<=", ">=", "!=", ";", ",", "(", ")", "[", "]", "{", "}", "ID", "NUM"},
  {"IF", "INT", "RETURN", "VOID", "ELSE", "WHILE", "PLUS", "MINUS", "PLUS", "SLASH", "ASSIGN", "EQUAL", "LT", "GT", "LTE", "GTE", "DIF", "SEMICOLON", "COMMA", "OPARENT", "CPARENT", "OBRACKET", "CBRACKET", "OKEY", "CKEY", "IDENTIFIER", "NUMBER"},
};

reservedWords reservedWordsHash[RESERVED_HASHTABLE_LEN];

int getHashKey(char *key) {
    int salt[5] = {13, 17, 19, 23, 29};
	long int ascii = 0;

    int i = 0;
    while (key[i] != '\0') {
        if (i < 5) {
            ascii += (int) key[i] * salt[i];
        } else {
            ascii += (int) key[i];
        }
        i++;
    }
    return ascii % RESERVED_HASHTABLE_LEN;
}

reservedWords getReservedWordValue(char *lexem) {
	reservedHashKey = getHashKey(lexem);
	return reservedWordsHash[reservedHashKey];
}

void initializeReservedWordsHash() {
    int i;
    for (i = 0; i < MAX_RESERVED_WORDS; i++) {
        reservedHashValue = getReservedWordValue(tokenNames[0][i]);
        reservedHashValue = (reservedWords) malloc(sizeof(struct reservedWords));
        reservedHashValue->lexem = tokenNames[0][i];
        reservedHashValue->type = i + 1;
        reservedWordsHash[reservedHashKey] = reservedHashValue;
    }
}

int isReservedWord(char *lexem) {
    reservedHashValue = getReservedWordValue(lexem);
    if (reservedHashValue == NULL) {
        return 0;
    }

    /* Lexem is reserved word */
    if (strcmp(reservedHashValue->lexem, lexem) == 0) {
        return reservedHashValue->type;
    }

    /* Lexem is identifier */
    return 0;
}
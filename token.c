#include "token.h"
#include "utils.h"

int hashKey;
reservedWords hashValue;

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
	hashKey = getHashKey(lexem);
	return reservedWordsHash[hashKey];
}

void initializeReservedWordsHash() {
    int i;
    for (i = 0; i < MAX_RESERVED_WORDS; i++) {
        hashValue = getReservedWordValue(tokenNames[0][i]);
        hashValue = (reservedWords) malloc(sizeof(struct reservedWords));
        hashValue->lexem = tokenNames[0][i];
        hashValue->type = i + 1;
        reservedWordsHash[hashKey] = hashValue;
    }
}

int isReservedWord(char *lexem) {
    hashValue = getReservedWordValue(lexem);
    if (hashValue == NULL) {
        return 0;
    }

    /* Lexem is reserved word */
    if (strcmp(hashValue->lexem, lexem) == 0) {
        return hashValue->type;
    }

    /* Lexem is identifier */
    return 0;
}
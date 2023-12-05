#ifndef TOKEN_H
#define TOKEN_H

#define RESERVED_HASHTABLE_LEN 499
#define MAX_RESERVED_WORDS 6

#define TOKEN_IF 1
#define TOKEN_INT 2
#define TOKEN_RETURN 3
#define TOKEN_VOID 4
#define TOKEN_ELSE 5
#define TOKEN_WHILE 6
#define TOKEN_PLUS 7
#define TOKEN_MINUS 8
#define TOKEN_MULT 9
#define TOKEN_SLASH 10
#define TOKEN_ASSIGN 11
#define TOKEN_EQUAL 12
#define TOKEN_LT 13
#define TOKEN_GT 14
#define TOKEN_LTE 15
#define TOKEN_GTE 16
#define TOKEN_DIF 17
#define TOKEN_SEMICOLON 18
#define TOKEN_COMMA 19
#define TOKEN_OPARENT 20
#define TOKEN_CPARENT 21
#define TOKEN_OBRACKET 22
#define TOKEN_CBRACKET 23
#define TOKEN_OKEY 24
#define TOKEN_CKEY 25
#define TOKEN_IDENTIFIER 26
#define TOKEN_NUMBER 27

static char *tokenNames[2][27] = {
  {"if", "int", "return", "void", "else", "while", "+", "-", "*", "/", "=", "==", "<", ">", "<=", ">=", "!=", ";", ",", "(", ")", "[", "]", "{", "}", "ID", "NUM"},
  {"IF", "INT", "RETURN", "VOID", "ELSE", "WHILE", "PLUS", "MINUS", "PLUS", "SLASH", "ASSIGN", "EQUAL", "LT", "GT", "LTE", "GTE", "DIF", "SEMICOLON", "COMMA", "OPARENT", "CPARENT", "OBRACKET", "CBRACKET", "OKEY", "CKEY", "IDENTIFIER", "NUMBER"},
};

typedef struct token {
    int type;
    int line;
} token;

typedef struct reservedWords {
    int type;
    char *lexem;
} *reservedWords;

reservedWords reservedWordsHash[RESERVED_HASHTABLE_LEN];

void initializeReservedWordsHash();
int isReservedWord(char *lexem);

#endif
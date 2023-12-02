#include "utils.h"
#include "syntaxTree.h"
#include "scanner.h"

void initializeBuffers() {
	memset(lexem, 0, sizeof(TOKEN_MAX_LENGTH));
	memset(tokenID, 0, sizeof(TOKEN_MAX_LENGTH));
	memset(tokenNUM, 0, sizeof(TOKEN_MAX_LENGTH));
	memset(tokenRESERVED, 0, sizeof(TOKEN_MAX_LENGTH));
	memset(tokenSYMBOL, 0, sizeof(TOKEN_MAX_LENGTH));
}

int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("Metodo para executar o programa: %s codigo.c\n", argv[0]);
		return -1;
	}

	file = fopen(argv[1], "r");

	initializeBuffers();

	printf("\n* * * * * ANALISE LEXICA E SINTATICA * * * * *\n\n");
	syntaxTree = parse();

	printf("\n* * * * * ARVORE DE ANALISE SINTATICA * * * * *\n\n");
	printSyntaxTree(syntaxTree);

	fclose(file);
	return 0;
}
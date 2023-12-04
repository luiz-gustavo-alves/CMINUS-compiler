#include "utils.h"
#include "syntaxTree.h"
#include "parserHelper.h"
#include "scanner.h"
#include "symtab.h"
#include "analyze.h"

void initializeGlobals() {
	memset(lexem, 0, sizeof(TOKEN_MAX_LENGTH));
	memset(tokenID, 0, sizeof(TOKEN_MAX_LENGTH));
	memset(tokenNUM, 0, sizeof(TOKEN_MAX_LENGTH));

 	lineCount = 1;
	lexicalError = 0;
	syntaxError = 0;
	semanticError = 0;

	currentLine = 1;
	functionCurrentLine = 1;
}

int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("Metodo para executar o programa: %s codigo.c\n", argv[0]);
		return -1;
	}

	file = fopen(argv[1], "r");
	if (file == NULL) {
		printf("Arquivo nao encontrado.");
		return -1;
	}

	initializeGlobals();

	printf("\n* * * * * ANALISE LEXICA E SINTATICA * * * * *\n\n");
	syntaxTree = parse();

	fclose(file);

	if (!lexicalError && !syntaxError) {
		printf("\n\n* * * * * ARVORE DE ANALISE SINTATICA * * * * *\n\n");
		printSyntaxTree(syntaxTree);

		printf("\n\n* * * * * ANALISE SEMANTICA * * * * *\n\n");
		semanticAnalysis(syntaxTree);

		if (!semanticError) {
			printf("Fases de analise completas!");
			printf("\n\n* * * * * TABELA DE SIMBOLOS * * * * *\n\n");
			printSymtable();
		}
	}

	/* Finished compiler analysis phase with no errors */
	return 0;
}
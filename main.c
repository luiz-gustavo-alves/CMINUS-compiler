#include "scanner.h"

FILE *file;

int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("Metodo para executar o programa: %s codigo.c\n", argv[0]);
		return -1;
	}

	file = fopen(argv[1], "r");
	int lexicalError = lexicalAnalysis(file);
	if (!lexicalError) {
		printf("\n\n* * * * * FASE (2): ANALISE SINTATICA | GERACAO DA ARVORE SINTATICA: * * * * *\n\n");
	}

	fclose(file);
	return 0;
}
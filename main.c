#include "utils.h"
#include "syntaxTree.h"

int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("Metodo para executar o programa: %s codigo.c\n", argv[0]);
		return -1;
	}

	file = fopen(argv[1], "r");
	syntaxTree = parse();

	fclose(file);
	return 0;
}
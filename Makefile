compiler:
	bison -d -v parser.y && gcc -c scanner.c	
	gcc utils.c symtab.c analyze.c parser.tab.c parserHelper.c syntaxTree.c scanner.c token.c main.c -o main
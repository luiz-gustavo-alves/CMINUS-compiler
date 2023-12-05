#include "analyze.h"
#include "symtab.h"
#include "token.h"

int mainFuncDecl = 0;

int validateDeclFunc(treeNode *tree) {

	if (mainFuncDecl) {
        return throwSemanticError(tree, "Declaracao de funcao depois da funcao main.");
	}

	if (strcmp(tree->key.name, "input") == 0 || strcmp(tree->key.name, "output") == 0) {
	    return throwSemanticError(tree, "Funcao reservada.");
	}

	if (strcmp(tree->key.name, "main") == 0) {
		mainFuncDecl = 1;
	}

	int funcDeclared = checkIdDeclaration(tree->key.name);
	if (!funcDeclared) {
		insertFuncSymtable(tree->key.name, tree->line, tree->type);
		currentScope = tree->key.name;
	} else {
		int funcIsNotVar = checkVarIsFunc(tree->key.name);
		if (!funcIsNotVar) {
			return throwSemanticError(tree, "Declaracao de funcao com nome de variavel ja declarada.");
		} else {
			return throwSemanticError(tree, "Funcao ja declarada.");
		}
	}
}

int validateDeclVar(treeNode *tree) {

	if (tree->type == Void) {
		return throwSemanticError(tree, "Declaracao de variavel do tipo VOID.");
	}

    if (strcmp(currentScope, "global") == 1) {
        return throwSemanticError(tree, "Variavel global ja declarada.");
    }

	int varIsFunc = checkVarIsFunc(tree->key.name);
	if (varIsFunc) {
		return throwSemanticError(tree, "Declaracao de variavel com nome de funcao ja declarada.");
	}

	int varDeclared = checkIdDeclaration(tree->key.name);
	if (!varDeclared) {
		insertVarSymtable(tree->key.name, tree->line, currentScope, tree->type);
	} else {
		int varNotInCurrentScope = checkScope(tree->key.name, currentScope);
		if (!varNotInCurrentScope) {
            insertVarSymtable(tree->key.name, tree->line, currentScope, tree->type);
        } else {
			return throwSemanticError(tree, "Variavel ja declarada.");
		}
	}
}

int validateStmtFunc(treeNode *tree, primitiveType *type) {

	int isReservedFunc = 0;
	if (strcmp(tree->key.name, "input") == 0) {
		tree->type = Integer; 
		isReservedFunc = 1;
	} else if (strcmp(tree->key.name, "output") == 0) {
		tree->type = Void;
		isReservedFunc = 1;
	}

	if (isReservedFunc) {
		return 0;
	}

	int funcDeclared = checkIdDeclaration(tree->key.name);
	if (!funcDeclared) { 
		return throwSemanticError(tree, "Funcao nao declarada.");
	} else {
		getIdType(tree->key.name, currentScope, type);
		tree->type = *type;
		insertFuncSymtable(tree->key.name, tree->line, tree->type);
	}
}

int validateExpId(treeNode *tree, primitiveType *type) {

	if (strcmp(tree->key.name, "void") == 0) {
		return 0;
	}

	int varDeclared = checkIdDeclaration(tree->key.name);
	if (!varDeclared) {
		return throwSemanticError(tree, "Variavel nao declarada");
	}

	int isLocalVarScope = checkScope(tree->key.name, currentScope);
	if (isLocalVarScope) {
		getIdType(tree->key.name, currentScope, type);
		if (tree->type == Void) {
			tree->type = *type;
		}
		insertVarSymtable(tree->key.name, tree->line, currentScope, tree->type);
	} else {
		int varGlobal = checkScope(tree->key.name, "global");
		if (!varGlobal) {
			return throwSemanticError(tree, "Variavel nao declarada.");
		} else {
			getIdType(tree->key.name, currentScope, type);
			if (tree->type == Void) {
				tree->type = *type;
			}
			insertVarSymtable(tree->key.name, tree->line, "global", tree->type);
		}
	}
}

void checkNodesDeclaration(treeNode *tree) { 

	if (!semanticError) {
		primitiveType *type = (primitiveType*) malloc(sizeof(primitiveType));

	    if (tree->node == decl) {
			switch (tree->subType.decl) { 
				case declFunc: validateDeclFunc(tree); break;
				case declVar: validateDeclVar(tree); break;
	            default: break;
            }
        } else if (tree->node == exp && tree->subType.exp == expId) {
			validateExpId(tree, type);
	    } else if (tree->node == stmt && tree->subType.stmt == stmtFunc) {
			validateStmtFunc(tree, type);
		}
	    free(type);
	}
}

int isBooleanOp(treeNode *tree) {
	return (tree->key.op >= TOKEN_EQUAL && tree->key.op <= TOKEN_DIF);
}

int validateExpOp(treeNode *tree) {
	if ((tree->child[0]->type != Integer) || (tree->child[1]->type != Integer)) {
		return throwSemanticError(tree, " Operacao entre tipos que nao sao INT.");
	}

	if (isBooleanOp(tree)) {
		tree->type = Boolean;
	} else {
		tree->type = Integer;
	}
}

int validateStmtIf(treeNode *tree) {
	if (tree->child[0]->type == Integer) {
		return throwSemanticError(tree->child[0], "Condicional IF nao eh do tipo BOOLEAN.");
	}
}

int validateStmtWhile(treeNode *tree) {
	if (tree->child[0]->type == Array || tree->child[0]->type == Void) {
		return throwSemanticError(tree->child[0], "Condicional WHILE nao eh do tipo BOOLEAN.");
	}
}

int validateStmtAttrib(treeNode *tree) {
	if (tree->child[0]->type != tree->child[1]->type) {
		return throwSemanticError(tree->child[1], "Tipos invalidos para atribuicao de variavel.");
	}
}

void checkNodesTypes(treeNode *tree) {

	if (tree->node == exp && tree->subType.exp == expOp) {
		validateExpOp(tree);
	} else if (tree->node == stmt) {
		switch (tree->subType.stmt) {
			case stmtIf: validateStmtIf(tree); break;
			case stmtAttrib: validateStmtAttrib(tree); break;
			case stmtWhile: validateStmtWhile(tree); break;
			default: break;
		}
	}
}

void preOrderTraversal(treeNode *tree) {

    if (tree != NULL && !semanticError) {
        checkNodesDeclaration(tree);
		int i;
        for (i = 0; i < CHILD_MAX_NODES; i++) {
			preOrderTraversal(tree->child[i]);
		}
        preOrderTraversal(tree->sibling);
    }
}

void postOrderTraversal(treeNode *tree) {

    if (tree != NULL && !semanticError) { 
		int i;
        for (i = 0; i < CHILD_MAX_NODES; i++) 
			postOrderTraversal(tree->child[i]);

        checkNodesTypes(tree);
        postOrderTraversal(tree->sibling);
    }
}

void semanticAnalysis(treeNode *tree) {

	/* Check variable, function, statements and expression declarations */
    preOrderTraversal(tree);

    if (!mainFuncDecl && !semanticError) {
		printf("(!) ERRO SEMANTICO: Funcao main nao foi declarada.\n");
        semanticError = 1;
    } else {
		/* Check variable, numeric, expression types and operations */
		postOrderTraversal(tree);	
	}
}

int throwSemanticError(treeNode *tree, char *msg) {

	semanticError = 1;
    printf("(!) ERRO SEMANTICO: %s | LINHA %d:\n", msg, tree->line);
    return semanticError;
}
#include "analyze.h"
#include "symtab.h"
#include "token.h"

#define COMP_EQUAL(str1, str2) (strcmp(str1, str2) == 0)
#define COMP_DIF(str1, str2) (strcmp(str1, str2) == 1)

#define NOT_FUNC 0
#define IS_FUNC 1

int mainFuncDecl = 0;

void semanticAnalysis(treeNode *tree) {

	/* Check variable, function, statements and expression declarations */
    preOrderTraversal(tree);

    if (!mainFuncDecl && !semanticError) {
		printf("\n\n(!) ERRO SEMANTICO: Funcao main nao declarada.\n");
        semanticError = 1;
    }
	else {
		/* Check variable, numeric, expression types and operations */
		postOrderTraversal(tree);	
	}
}

int validateDeclFunc(treeNode *tree) {

	if (mainFuncDecl) {
        return throwSemanticError(tree, "Declaracao de funcao depois da funcao main.");
	}

	if (COMP_EQUAL(tree->key.name, "input") || COMP_EQUAL(tree->key.name, "output")) {
	    return throwSemanticError(tree, "Funcao reservada.");
	}

	if (COMP_EQUAL(tree->key.name, "main")) {
		mainFuncDecl = 1;
	}

	int funcFirstDecl = checkDeclaredFunc(tree->key.name) == 0;
	if (funcFirstDecl) {
		insertSymtable(tree->key.name, tree->line, "global", tree->type, IS_FUNC);
		currentScope = tree->key.name;
	} else {
		return throwSemanticError(tree, "Funcao ja declarada.");
	}
}

int validateDeclVar(treeNode *tree) {

	if (tree->type == Void) {
		return throwSemanticError(tree, "Variavel nao do tipo <INT>.");
	}

    if (COMP_DIF(currentScope, "global")) {
        return throwSemanticError(tree, "Variavel global ja declarada.");
    }

	int varFirstDecl = checkDeclaredVar(tree->key.name) == 0;
	if (varFirstDecl) {
		insertSymtable(tree->key.name, tree->line, currentScope, tree->type, NOT_FUNC);
	} else {
		int varDeclInScopeOrArgs = checkVarScope(tree->key.name, currentScope) == 0;
		if (varDeclInScopeOrArgs) {
            insertSymtable(tree->key.name, tree->line, currentScope, tree->type, NOT_FUNC);
        } else {
			return throwSemanticError(tree, "Variavel ja declarada.");
		}
	}
}

int validateStmtFunc(treeNode *tree, primitiveType *type) {

	int isReservedFunc = 0;
	if (COMP_EQUAL(tree->key.name, "input")) {
		tree->type = Integer; 
		isReservedFunc = 1;
	} else if (COMP_EQUAL(tree->key.name, "output")) {
		tree->type = Void;
		isReservedFunc = 1;
	}

	if (isReservedFunc) {
		return 0;
	}

	int funcNotDeclared = checkDeclaredFunc(tree->key.name) == 0;
	if (funcNotDeclared) { 
		return throwSemanticError(tree, "Funcao nao declarada.");
	} else {
		getFuncType(tree->key.name, type);
		tree->type = *type;
		insertSymtable(tree->key.name, tree->line, "global", tree->type, IS_FUNC);
	}
}

int validateExpId(treeNode *tree, primitiveType *type) {

	if (COMP_EQUAL(tree->key.name, "void")) {
		return 0;
	}

	int varNotDeclared = checkDeclaredVar(tree->key.name) == 0;
	if (varNotDeclared) {
		return throwSemanticError(tree, "Variavel nao declarada");
	}
	
	int isLocalVarScope = checkVarScope(tree->key.name, currentScope) == 1;
	if (isLocalVarScope) {
		getVarType(tree->key.name, currentScope, type);
		if (tree->type == Void) {
			tree->type = *type;
		}
		insertSymtable(tree->key.name, tree->line, currentScope, tree->type, NOT_FUNC);
	} else {

		int varNotGlobal = checkGlobalVar(tree->key.name) == 0;
		if (varNotGlobal) {
			return throwSemanticError(tree, "Variavel nao declarada.");
		} 
		else {
			getVarType(tree->key.name, "global", type);
			if (tree->type == Void) {
				tree->type = *type;
			}
			insertSymtable(tree->key.name, tree->line, "global", tree->type, NOT_FUNC);
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

void postOrderTraversal(treeNode *tree) {

    if (tree != NULL && !semanticError) { 
		
		int i;
        for (i = 0; i < CHILD_MAX_NODES; i++) 
			postOrderTraversal(tree->child[i]);

        checkNodesType(tree);
        postOrderTraversal(tree->sibling);
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

int isBooleanOp(treeNode *tree) {
	return (tree->key.op >= 11 && tree->key.op <= 16);
}

int validateExpOp(treeNode *tree) {
	if ((tree->child[0]->type != Integer) || (tree->child[1]->type != Integer)) {
		return throwSemanticError(tree, " Operacao entre nao inteiros.");
	}
	if (isBooleanOp(tree)) {
		tree->type = Boolean;
	}
	else {
		tree->type = Integer;
	}
}

int validateStmtIf(treeNode *tree) {
	if (tree->child[0]->type == Integer) {
		return throwSemanticError(tree->child[0], "IF nao do tipo <boolean>."); 
	}
}

int validateStmtAttrib(treeNode *tree) {
	if (tree->child[0]->type != tree->child[1]->type) {
		return throwSemanticError(tree->child[1], "Tipo da variavel e valor a ser atribuido nao condizentes.");
	}
}

int validateStmtWhile(treeNode *tree) {
	if (tree->child[0]->type == Integer) {
		return throwSemanticError(tree->child[0], "WHILE nao do tipo <boolean>.");
	}
}

void checkNodesType(treeNode *tree) {

	if (!semanticError) {

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
}

int throwSemanticError(treeNode *tree, char *msg) {
    semanticError = 1;
    if (tree->node != exp && tree->subType.exp != expId) {
        printf("(!) ERRO SEMANTICO | LINHA %d: %s\n", tree->line, msg);
	}
    else {
		printf("(!) ERRO SEMANTICO: %s | LINHA %d: %s\n", tree->key.name, tree->line, msg);
	}
    return semanticError;
}

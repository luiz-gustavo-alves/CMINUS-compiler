#include "analyze.h"
#include "symtab.h"
#include "token.h"

#define NOT_FUNC 0
#define IS_FUNC 1

int hasMainFunc = 0;

void semanticAnalysis(treeNode *tree) {

    preOrderTraversal(tree);
    if (!hasMainFunc && !semanticError) {
		printf("\n\n(!) ERRO SEMANTICO: Funcao main nao declarada.\n");
        semanticError = 1;
    }
	else {
		postOrderTraversal(tree);	
	}
}

void checkPreOrderNode(treeNode *tree) { 

	if (!semanticError) {
	
		primitiveType *type = (primitiveType*) malloc(sizeof(primitiveType));

	    switch (tree->node) {
		
	        case decl:
		
	            switch (tree->subType.decl) { 
	
	                case declFunc:

						if (!hasMainFunc) {
							
							if (strcmp(tree->key.name, "main") == 0) hasMainFunc = 1;
		                    if (strcmp(tree->key.name, "input") == 0 || strcmp(tree->key.name, "output") == 0) {
								
		                        printf("\n\n(!) ERRO SEMANTICO | LINHA %d: Funcao %s reservada.\n", tree->line, tree->key.name);
		                        semanticError = 1;
		                    } 
							else {
								
		                        if (checkDeclaredFunc(tree->key.name) == 0) {
									
									insertSymtable(tree->key.name, tree->line, "global", tree->type, IS_FUNC);
		                            currentScope = tree->key.name;
		                        } 
								else {
									
		                            printf("\n\n(!) ERRO SEMANTICO | LINHA %d: Funcao %s ja declarada.\n", tree->line, tree->key.name);
		                            semanticError = 1;
		                        }
		                    }
						}
						else {
							
							printf("\n\n(!) ERRO SEMANTICO | Declaracao de funcao depois da funcao main.\n");
		                    semanticError = 1;
						}
	                break;
	
	                case declVar:
		
	                    if (tree->type == Void) {
		
	                        throwSemanticError(tree, "Variavel nao do tipo <INT>.");
	                        semanticError = 1;
	                    }
	
	                    if (checkDeclaredVar(tree->key.name) == 0) {
		
	                        if (tree->child[0] == NULL) {
								insertSymtable(tree->key.name, tree->line, currentScope, tree->type, NOT_FUNC);
							}
	
	                        else {
								
								insertSymtable(tree->key.name, tree->line, currentScope, tree->type, NOT_FUNC);
							}
	
	                    } else {
		
	                        if (strcmp(currentScope, "global") != 0) {
		
	                            if (checkVarScope(tree->key.name, currentScope) == 0) {
		
	                                if (tree->child[0] == NULL) {
										
										insertSymtable(tree->key.name, tree->line, currentScope, tree->type, NOT_FUNC);
									}
	
	                                else {
										
										insertSymtable(tree->key.name, tree->line, currentScope, tree->type, NOT_FUNC);
									}
	                            } 
								else {
		
	                                printf("\n\n(!) ERRO SEMANTICO | LINHA %d: Variavel %s ja declarada.\n", tree->line, tree->key.name);
	                                semanticError = 1;
	                            }
	                        } 
							else {
		
	                            printf("\n\n(!) ERRO SEMANTICO | LINHA %d: Variavel global %s ja declarada.\n", tree->line, tree->key.name);
	                            semanticError = 1;
	                        }
	                    }
	              	break;
	            }
	        break;
	
	        case stmt:
		
	            switch (tree->subType.stmt) { 
	
	                case stmtFunc:
		
	                    if (strcmp(tree->key.name, "input") == 0 || strcmp(tree->key.name, "output") == 0) 
							tree->type = Integer; 
	
	                    else if (checkDeclaredFunc(tree->key.name) == 0) { 
	                        
							printf("\n\n(!) ERRO SEMANTICO | LINHA: %d: Funcao %s nao declarada.\n", tree->line, tree->key.name);
	                        semanticError = 1;
	                    } 
						else {
		
	                        getFuncType(tree->key.name, type);
	                        tree->type = *type;
							
							insertSymtable(tree->key.name, tree->line, "global", tree->type, IS_FUNC);
	                    }
	                break;
	            }
	        break;
	
	        case exp:
		
	            switch (tree->subType.exp) {
		
	                case expId:
		
	                    if (strcmp(tree->key.name, "void") != 0) {
		
	                        if (checkDeclaredVar(tree->key.name) == 0) {
		
	                                printf("\n\n(!) ERRO SEMANTICO | LINHA %d: Variavel %s nao declarada.\n", tree->line, tree->key.name);
	                                semanticError = 1;
	                            } 
								else if (checkVarScope(tree->key.name, currentScope) == 1) {
	                                
									getVarType(tree->key.name, currentScope, type);
	                                
									if (tree->type == Void) tree->type = *type;
									
									insertSymtable(tree->key.name, tree->line, currentScope, tree->type, NOT_FUNC);
	                            } 
								else {
				
	                                if (checkGlobalVar(tree->key.name) == 0) {
	                                    
										printf("\n\n(!) ERRO SEMANTICO | LINHA %d: Variavel %s nao declarada.\n", tree->line, tree->key.name);
	                                    semanticError = 1;
	                                } 
									else {
		
	                                    getVarType(tree->key.name, "global", type);
	                                    
	                                    if (tree->type == Void) tree->type = *type;
										
										insertSymtable(tree->key.name, tree->line, "global", tree->type, NOT_FUNC);
	                                }
	                            }
	                        }
	                    break;
	                }
	            break;
	
	       default: 
				break;
	    }
	    free(type);
	}
}

void postOrderTraversal(treeNode *tree) {

    if (tree != NULL && !semanticError) { 
		
		int i;
        for (i = 0; i < CHILD_MAX_NODES; i++) 
			postOrderTraversal(tree->child[i]);

        checkPostOrderNode(tree);
        postOrderTraversal(tree->sibling);
    }
}

void preOrderTraversal(treeNode *tree) { 

    if (tree != NULL && !semanticError) {
        checkPreOrderNode(tree);

		int i;
        for (i = 0; i < CHILD_MAX_NODES; i++) {
			preOrderTraversal(tree->child[i]);
		}
        preOrderTraversal(tree->sibling);
    }
}

void checkPostOrderNode(treeNode *tree) {
	
    switch (tree->node) { 

	    case exp:
	
	        switch (tree->subType.exp) {
	
	            case expOp:
	
	                if ((tree->child[0]->type != Integer) || (tree->child[1]->type != Integer))
	                    throwSemanticError(tree, " Operacao entre nao inteiros.");
	                else
	                    tree->type = Integer;
	                    
	                tree->type = Integer;
	
	                if ((strcmp(tokenNames[1][tree->key.op - 1], "EQUAL") == 0) ||
	                    ((strcmp(tokenNames[1][tree->key.op - 1], "DIF")) == 0)   || 
	                    ((strcmp(tokenNames[1][tree->key.op - 1], "LT")) == 0)    || 
	                    ((strcmp(tokenNames[1][tree->key.op - 1], "LTE")) == 0)   || 
	                    ((strcmp(tokenNames[1][tree->key.op - 1], "GT")) == 0)    || 
	                    ((strcmp(tokenNames[1][tree->key.op - 1], "GTE")) == 0)) {
						tree->type = Boolean;
					}
	                else
	                    tree->type = Integer;

	           break;
	        }
	    break;

	    case stmt:
	
	        switch (tree->subType.stmt) {
	
		        case stmtIf:
		            
					if (tree->child[0]->type == Integer) 
						throwSemanticError(tree->child[0], "IF nao do tipo <boolean>."); 
					break;

		        case stmtAttrib:
	
		            if (tree->child[0]->type != tree->child[1]->type) 
						throwSemanticError(tree->child[1], "Tipo da variavel e valor a ser atribuido nao condizentes.");
		            break;

		        case stmtWhile:
	
		            if (tree->child[0]->type == Integer) 
						throwSemanticError(tree->child[0], "WHILE nao do tipo <boolean>.");
		            break;
		        }
	    break;
    }
}

void throwSemanticError(treeNode *tree, char *msg) {
    semanticError = 1;
    if (tree->node != exp && tree->subType.exp != expId) {
        printf("(!) ERRO SEMANTICO | LINHA %d: %s\n", tree->line, msg);
	}
    else {
		printf("(!) ERRO SEMANTICO: %s | LINHA %d: %s\n", tree->key.name, tree->line, msg);
	}
}

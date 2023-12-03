#include "analyze.h"
#include "symtab.h"
#include "token.h"

#define NOT_FUNC 0
#define IS_FUNC 1

int hasMainFunc = 0;

void semantic_err(treeNode *tree, char *msg);

void sym_tab_build(treeNode *tree) {

    tree_pre_order(tree);

    if (!hasMainFunc && !semanticError) {
        
		printf("\n\n(!) ERRO SEMANTICO: Funcao main nao declarada.\n");
        semanticError = 1;
    } 
	else
		verify_node_kind(syntaxTree);
}

void tree_insert_node(treeNode *tree) { 

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
								
		                        if (check_declared_func(tree->key.name) == 0) {
									
									sym_tab_insert(tree->key.name, tree->line, "global", tree->type, IS_FUNC);
		                            cur_var_scope = tree->key.name;
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
		
	                        semantic_err(tree, "Variavel nao do tipo <INT>.");
	                        semanticError = 1;
	                    }
	
	                    if (check_declared_var(tree->key.name) == 0) {
		
	                        if (tree->child[0] == NULL) {
								sym_tab_insert(tree->key.name, tree->line, cur_var_scope, tree->type, NOT_FUNC);
							}
	
	                        else {
								
								sym_tab_insert(tree->key.name, tree->line, cur_var_scope, tree->type, NOT_FUNC);
							}
	
	                    } else {
		
	                        if (strcmp(cur_var_scope, "global") != 0) {
		
	                            if (check_declared_var_scope(tree->key.name, cur_var_scope) == 0) {
		
	                                if (tree->child[0] == NULL) {
										
										sym_tab_insert(tree->key.name, tree->line, cur_var_scope, tree->type, NOT_FUNC);
									}
	
	                                else {
										
										sym_tab_insert(tree->key.name, tree->line, cur_var_scope, tree->type, NOT_FUNC);
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
	
	                    else if (check_declared_func(tree->key.name) == 0) { 
	                        
							printf("\n\n(!) ERRO SEMANTICO | LINHA: %d: Funcao %s nao declarada.\n", tree->line, tree->key.name);
	                        semanticError = 1;
	                    } 
						else {
		
	                        get_func_kind(tree->key.name, type);
	                        tree->type = *type;
							
							sym_tab_insert(tree->key.name, tree->line, "global", tree->type, IS_FUNC);
	                    }
	                break;
	            }
	        break;
	
	        case exp:
		
	            switch (tree->subType.exp) {
		
	                case expId:
		
	                    if (strcmp(tree->key.name, "void") != 0) {
		
	                        if (check_declared_var(tree->key.name) == 0) {
		
	                                printf("\n\n(!) ERRO SEMANTICO | LINHA %d: Variavel %s nao declarada.\n", tree->line, tree->key.name);
	                                semanticError = 1;
	                            } 
								else if (check_declared_var_scope(tree->key.name, cur_var_scope) == 1) {
	                                
									get_var_kind(tree->key.name, cur_var_scope, type);
	                                
									if (tree->type == Void) tree->type = *type;
									
									sym_tab_insert(tree->key.name, tree->line, cur_var_scope, tree->type, NOT_FUNC);
	                            } 
								else {
				
	                                if (check_declared_global_var(tree->key.name) == 0) {
	                                    
										printf("\n\n(!) ERRO SEMANTICO | LINHA %d: Variavel %s nao declarada.\n", tree->line, tree->key.name);
	                                    semanticError = 1;
	                                } 
									else {
		
	                                    get_var_kind(tree->key.name, "global", type);
	                                    
	                                    if (tree->type == Void) tree->type = *type;
										
										sym_tab_insert(tree->key.name, tree->line, "global", tree->type, NOT_FUNC);
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

void tree_post_order(treeNode *tree) {

    if (tree != NULL && !semanticError) { 
		
		int i;
        for (i = 0; i < CHILD_MAX_NODES; i++) 
			tree_post_order(tree->child[i]);

        verify_nodes(tree);
        tree_post_order(tree->sibling);
    }
}

void tree_pre_order(treeNode *tree) { 

    if (tree != NULL) { 

        tree_insert_node(tree);
		
		int i;
        for (i = 0; i < CHILD_MAX_NODES; i++) 
			tree_pre_order(tree->child[i]);
        
        tree_pre_order(tree->sibling);
    }
}

void verify_nodes(treeNode *tree) {
	
    switch (tree->node) { 

	    case exp:
	
	        switch (tree->subType.exp) {
	
	            case expOp:
	
	                if ((tree->child[0]->type != Integer) || (tree->child[1]->type != Integer))
	                    semantic_err(tree, " Operacao entre nao inteiros.");
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
						semantic_err(tree->child[0], "IF nao do tipo <boolean>."); 
					break;

		        case stmtAttrib:
	
		            if (tree->child[0]->type != tree->child[1]->type) 
						semantic_err(tree->child[1], "Tipo da variavel e valor a ser atribuido nao condizentes.");
		            break;

		        case stmtWhile:
	
		            if (tree->child[0]->type == Integer) 
						semantic_err(tree->child[0], "WHILE nao do tipo <boolean>.");
		            break;
		        }
	    break;
    }
}

void verify_node_kind(treeNode *tree) {

    tree_post_order(tree);

    if (!semanticError) printf("\n\n(*) FASES DE ANALISE COMPLETA.");
}

void semantic_err(treeNode *tree, char *msg) {
	
    semanticError = 1;

    if (tree->node != exp && tree->subType.exp != expId)
        printf("\n\n(!) ERRO SEMANTICO | LINHA %d: %s\n", tree->line, msg);

    else
        printf("\n\n(!) ERRO SEMANTICO: %s | LINHA %d: %s\n", tree->key.name, tree->line, msg);
}

#include "symtab.h"

int hash_key;
Bucket_list hash_value;

int hash_table(char * key) { 
	
	int tmp = 0;
    int i = 0;
    
    while (key[i] != '\0') { 
	
        tmp = ((tmp << SHIFT) + key[i]) % HASHTABLE_LEN; 
		i++;
    }
    printf("( %s | %d)\n", key, tmp);
    return tmp;
}

Bucket_list get_hash_value(char *name) {
	
	hash_key = hash_table(name);
	return hashtable[hash_key];
}

void sym_tab_insert(char *name, int line_number, char *cur_var_scope, primitiveType kind, int is_function) {
	
	hash_value = get_hash_value(name);
    
    while (hash_value != NULL && (strcmp(name, hash_value->name) != 0 || strcmp(hash_value->var_scope, cur_var_scope) != 0))
   		hash_value = hash_value->next;

    if (hash_value == NULL) {
        
		hash_value = (Bucket_list) malloc(sizeof(struct Bucket_list));
        
        hash_value->name = name;
        hash_value->lines = (Line_list) malloc(sizeof(struct Line_list));
        hash_value->lines->line_num = line_number;

        if (!is_function) hash_value->num_id = ++var_count;

        if (strcmp(cur_var_scope, "global") == 0) {
	
            char* scope_global = malloc(sizeof(char));
            strcat(scope_global, "global");
            hash_value->var_scope = scope_global;
        } 
		else
            hash_value->var_scope = cur_var_scope;

        hash_value->block_type = is_function;
        hash_value->lines->next = NULL;
        hash_value->kind = kind;
        hash_value->next = hashtable[hash_key];
        
        hashtable[hash_key] = hash_value; 
    } 
	else {
 
        Line_list hash_lines = hash_value->lines;
        
        while (hash_lines->next != NULL)
            hash_lines = hash_lines->next;
        
        hash_lines->next = (Line_list) malloc(sizeof(struct Line_list));
        hash_lines->next->next = NULL;
        hash_lines->next->line_num = line_number;
    }
}

int check_declared_func(char *name) {
	
    hash_value = get_hash_value(name);

    while (hash_value != NULL && (strcmp(name, hash_value->name) != 0 || hash_value->block_type == 0))
        hash_value = hash_value->next;
    
    return (hash_value != NULL);
}

int check_declared_var(char *name) {
	
    hash_value = get_hash_value(name);
    
    while (hash_value != NULL && (strcmp(name, hash_value->name) != 0 || hash_value->block_type == 1))
        hash_value = hash_value->next;
    
    return (hash_value != NULL); 
}

int check_declared_var_scope(char *name, char *cur_var_scope) {
	
    hash_value = get_hash_value(name);
    
    while (hash_value != NULL) {
	
        if ((hash_value->block_type == 0 && strcmp(name, hash_value->name) == 0) && strcmp(cur_var_scope, hash_value->var_scope) == 0) break;

        hash_value = hash_value->next;
    }

    return (hash_value != NULL); 
}

int check_declared_global_var(char *name) {
	
    hash_value = get_hash_value(name);
    
    while (hash_value != NULL) {
	
        if ((hash_value->block_type == 0 && strcmp(name, hash_value->name) == 0) && strcmp(hash_value->var_scope, "global") == 0) break;

        hash_value = hash_value->next;
    }
    return (hash_value != NULL); 
}

char *get_string_type(primitiveType kind) {
    
	switch (kind) {
	
        case Integer: return "int"; break;
        case Void: return "void"; break;
        case Array: return "array"; break;
        default: return "bool"; break;
    }
}

void get_func_kind(char *name, primitiveType *p_kind) {
	
    hash_value = get_hash_value(name);
    
    while ((hash_value != NULL) && ((strcmp(name, hash_value->name) != 0) || (hash_value->block_type == 0)))
        hash_value = hash_value->next;

    if (hash_value != NULL) *p_kind = hash_value->kind;
}

void get_var_kind(char *name, char *cur_var_scope, primitiveType *p_kind) {
	
    hash_value = get_hash_value(name);
    
    while ((hash_value != NULL) && ((strcmp(name, hash_value->name) != 0) || (strcmp(cur_var_scope, hash_value->var_scope) != 0) || (hash_value->block_type == 1)))
        hash_value = hash_value->next;

    if (hash_value != NULL) *p_kind = hash_value->kind;
}

void print_sym_tab() {
    
	printf(" |------------------------------------------------------------------------------------\n");
    printf(" |   Nome   |  Id  |  Tipo Variavel  |   Escopo   |   Linhas  	      \n");
    printf(" |------------------------------------------------------------------------------------\n");

	int i;
    for (i = 0; i < HASHTABLE_LEN; i++) { 

        if (hashtable[i] != NULL) { 

            Bucket_list hash_value = hashtable[i];

            while (hash_value != NULL) {
 
                Line_list hash_lines = hash_value->lines;
                printf(" |  %-6s ", hash_value->name);

                if (hash_value->num_id < 10) printf(" |  0%-1d ", hash_value->num_id);
                else printf(" |  %-1d ", hash_value->num_id);
        
                printf(" |  %-13s ", get_string_type(hash_value->kind));
                printf(" |  %-8s  | ", hash_value->var_scope);
                
                while (hash_lines != NULL) { 
                    
					printf("%4d ", hash_lines->line_num);
                    hash_lines = hash_lines->next;
                }
                
                printf("\n");
                hash_value = hash_value->next;
            }
        }
    }
    printf(" |-----------------------------------------------------------------------------------\n\n");
} 

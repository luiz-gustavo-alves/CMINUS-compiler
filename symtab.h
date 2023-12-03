#ifndef SYMTAB_H
#define SYMTAB_H

#include "utils.h"

#define HASHTABLE_LEN 211
#define SHIFT 4

typedef struct Line_list { 

    struct Line_list *next;
    int line_num;

} *Line_list;

typedef struct Bucket_list { 

    struct Bucket_list* next;

	char *name, *var_scope;
    int num_id, block_type;

    Line_list lines;
    primitiveType kind; 

} *Bucket_list;

static Bucket_list hashtable[HASHTABLE_LEN];

static int var_count = 0;
int hash_table(char *key);

int check_declared_func(char *name);
int check_declared_var(char *name);
int check_declared_var_scope(char *name, char *_var_scope);
int check_declared_global_var(char *name);

void get_func_kind(char *name, primitiveType *p_kind);
void get_var_kind(char *name, char *_var_scope, primitiveType *p_kind);
void sym_tab_insert(char *name, int line_number, char *_var_scope, primitiveType kind, int is_function);
void print_sym_tab();

#endif /* SYMTAB_H */

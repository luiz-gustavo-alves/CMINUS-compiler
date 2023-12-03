#include "symtab.h"

int hashKey;
symTable hashValue;

int createHashKey(char * key) { 
	int tmp = 0;
    int i = 0;
    while (key[i] != '\0') {
        tmp = ((tmp << SHIFT) + key[i]) % HASHTABLE_LEN; i++;
    }
    return tmp;
}

symTable getHashValue(char *name) {
	hashKey = createHashKey(name);
	return hashtable[hashKey];
}

void insertSymtable(char *name, int lineNumber, char *currentScope, primitiveType kind, int isFunction) {
	
	hashValue = getHashValue(name);
    while (hashValue != NULL && (strcmp(name, hashValue->name) != 0 || strcmp(hashValue->scope, currentScope) != 0)) {
        hashValue = hashValue->next;
    }

    if (hashValue == NULL) {
		hashValue = (symTable) malloc(sizeof(struct symTable));
        if (!isFunction) {
            hashValue->idNumber = varCount;
            varCount++;
        }

        if (strcmp(currentScope, "global") == 1) {
            char* scope_global = malloc(sizeof(char));
            strcat(scope_global, "global");
            hashValue->scope = scope_global;
        } 
		else {
            hashValue->scope = currentScope;
        }
        
        hashValue->name = name;
        hashValue->isFunc = isFunction;
        hashValue->type = kind;

        hashValue->lines = (lineList) malloc(sizeof(struct lineList));
        hashValue->lines->line = lineNumber;
        hashValue->lines->next = NULL;

        hashValue->next = hashtable[hashKey];
        hashtable[hashKey] = hashValue; 
    } 
	else {

        lineList hashLines = hashValue->lines;
        while (hashLines->next != NULL) {
            hashLines = hashLines->next;
        }

        hashLines->next = (lineList) malloc(sizeof(struct lineList));
        hashLines->next->next = NULL;
        hashLines->next->line = lineNumber;
    }
}

int checkDeclaredFunc(char *name) {
	
    hashValue = getHashValue(name);
    while (hashValue != NULL && (strcmp(name, hashValue->name) != 0)) {
        hashValue = hashValue->next;
    }
    return hashValue != NULL;
}

int checkDeclaredVar(char *name) {
	
    hashValue = getHashValue(name);
    while (hashValue != NULL && (strcmp(name, hashValue->name) != 0)) {
        hashValue = hashValue->next;
    }
    return hashValue != NULL; 
}

int checkVarScope(char *name, char *currentScope) {
	
    hashValue = getHashValue(name);
    while (hashValue != NULL) {
        if ((strcmp(name, hashValue->name) == 0) && strcmp(currentScope, hashValue->scope) == 0) {
            break;
        }
        hashValue = hashValue->next;
    }
    return hashValue != NULL; 
}

int checkGlobalVar(char *name) {
	
    hashValue = getHashValue(name);
    while (hashValue != NULL) {
        if ((strcmp(name, hashValue->name) == 0) && strcmp(hashValue->scope, "global") == 0) {
            break;
        }
        hashValue = hashValue->next;
    }
    return hashValue != NULL; 
}

void getFuncType(char *name, primitiveType *type) {

    hashValue = getHashValue(name);
    while ((hashValue != NULL) && ((strcmp(name, hashValue->name) != 0))) {
        hashValue = hashValue->next;
    }

    if (hashValue != NULL) {
        *type = hashValue->type;
    }
}

void getVarType(char *name, char *currentScope, primitiveType *type) {
	
    hashValue = getHashValue(name);
    while ((hashValue != NULL) && ((strcmp(name, hashValue->name) != 0) || (strcmp(currentScope, hashValue->scope) != 0))) {
        hashValue = hashValue->next;
    }

    if (hashValue != NULL) {
        *type = hashValue->type;
    }
}

char *getPrimitiveType(primitiveType kind) {
	switch (kind) {
        case Integer: return "int";
        case Void: return "void";
        case Array: return "array";
        default: return "boolean";
    }
}

void printSymtable() {
    
	printf(" |------------------------------------------------------------------------------------\n");
    printf(" |   Nome   |  Id  |  Tipo Variavel  |   Escopo   |   Linhas  	      \n");
    printf(" |------------------------------------------------------------------------------------\n");

	int i;
    for (i = 0; i < HASHTABLE_LEN; i++) {

        if (hashtable[i] != NULL) { 
            symTable hashValue = hashtable[i];

            while (hashValue != NULL) {
                lineList hashLines = hashValue->lines;
                printf(" |  %-6s ", hashValue->name);

                if (hashValue->idNumber < 10) {
                    printf(" |  0%-1d ", hashValue->idNumber);
                }
                else {
                    printf(" |  %-1d ", hashValue->idNumber);
                }

                printf(" |  %-13s ", getPrimitiveType(hashValue->type));
                printf(" |  %-8s  | ", hashValue->scope);

                while (hashLines != NULL) { 
					printf("%4d ", hashLines->line);
                    hashLines = hashLines->next;
                }
                printf("\n");
                hashValue = hashValue->next;
            }
        }
    }
    printf(" |-----------------------------------------------------------------------------------\n\n");
} 

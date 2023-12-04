#include "symtab.h"
#include "analyze.h"

int hashKey;
symTable hashValue;

int varCount = 1;

int createHashKey(char * key) { 
	int asciiVal = 0;
    int i = 0;
    while (key[i] != '\0') {
        asciiVal += (int) key[i];
        i++;
    }
    return asciiVal % HASHTABLE_LEN;
}

symTable getHashValue(char *name) {
	hashKey = createHashKey(name);
	return hashtable[hashKey];
}

void insertSymtable(char *name, int line, char *scope, primitiveType type, idType idType) {
	
	hashValue = getHashValue(name);
    while (hashValue != NULL && (strcmp(name, hashValue->name) != 0 || strcmp(hashValue->scope, scope) != 0)) {
        hashValue = hashValue->next;
    }

    if (hashValue == NULL) {
		hashValue = (symTable) malloc(sizeof(struct symTable));
        if (idType == isVar) {
            hashValue->idNumber = varCount;
            varCount++;
        }

        if (COMP_DIF(scope, "global")) {
            char* scope_global = malloc(sizeof(char));
            strcat(scope_global, "global");
            hashValue->scope = scope_global;
        } 
		else {
            hashValue->scope = scope;
        }
        
        hashValue->name = name;
        hashValue->id = idType;
        hashValue->type = type;

        hashValue->lines = (lineList) malloc(sizeof(struct lineList));
        hashValue->lines->line = line;
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
        hashLines->next->line = line;
    }
}

int checkVarIsFunc(char *name) {
    hashValue = getHashValue(name);
    if (hashValue == NULL) {
        return 0;
    }
    return hashValue->id == isFunc;
}

int checkIdDeclaration(char *name) {
    hashValue = getHashValue(name);
    return hashValue != NULL;
}

int checkScope(char *name, char *scope) {
    hashValue = getHashValue(name);
    while (hashValue != NULL) {
        if ((strcmp(hashValue->name, name) == 0) && strcmp(hashValue->scope, scope) == 0) {
            break;
        }
        hashValue = hashValue->next;
    }
    return hashValue != NULL; 
}

void getIdType(char *name, primitiveType *type) {
    hashValue = getHashValue(name);
    while ((hashValue != NULL)) {
        if ((strcmp(hashValue->name, name) == 0)) {
            break;
        }
        hashValue = hashValue->next;
    }

    if (hashValue != NULL) {
        *type = hashValue->type;
    }
}

char *getPrimitiveType(primitiveType type) {
	switch (type) {
        case Integer: return "Int";
        case Void: return "Void";
        case Array: return "Array";
        default: return "Boolean";
    }
}

void printSymtable() {
    
	printf(" |-----------------------------------------------------------------------------------------------------------------------\n");
    printf(" |   Nome            |  ID  |  Tipo Dado |  Tipo ID  |   Escopo            |   Linhas  	      \n");
    printf(" |-----------------------------------------------------------------------------------------------------------------------\n");

	int i;
    for (i = 0; i < HASHTABLE_LEN; i++) {

        if (hashtable[i] != NULL) { 
            symTable hashValue = hashtable[i];

            while (hashValue != NULL) {
                lineList hashLines = hashValue->lines;
                printf(" |  %-15s ", hashValue->name);

                if (hashValue->idNumber < 10) {
                    printf(" |  0%-1d ", hashValue->idNumber);
                } else {
                    printf(" |  %-1d ", hashValue->idNumber);
                }

                if (hashValue->id == isVar) {
                    printf(" |  %-7s  ", "Var");
                } else {
                    printf(" |  %-7s  ", "Func");
                }

                printf(" |  %-7s ", getPrimitiveType(hashValue->type));
                printf(" |  %-17s  | ", hashValue->scope);

                while (hashLines != NULL) { 
					printf("%4d ", hashLines->line);
                    hashLines = hashLines->next;
                }
                printf("\n");
                hashValue = hashValue->next;
            }
        }
    }
    printf(" |-----------------------------------------------------------------------------------------------------------------------\n\n");
} 

#include "symtab.h"
#include "analyze.h"

symTable hashtable[HASHTABLE_LEN];
symTable hashValue;
int hashKey;
int varCount = 1;

int createHashKey(char * key) {
    int salt[5] = {13, 17, 19, 23, 29};
	long int ascii = 0;

    int i = 0;
    while (key[i] != '\0') {
        if (i < 5) {
            ascii += (int) key[i] * salt[i];
        } else {
            ascii += (int) key[i];
        }
        i++;
    }
    return ascii % HASHTABLE_LEN;
}

symTable getHashValue(char *name) {
	hashKey = createHashKey(name);
	return hashtable[hashKey];
}

void createHashValue(int line) {

    hashValue->lines = (lineList) malloc(sizeof(struct lineList));
    hashValue->lines->line = line;
    hashValue->lines->next = NULL;
    hashValue->next = hashtable[hashKey];
    hashtable[hashKey] = hashValue; 
}

void insertNewHashLines(int line) {

    lineList hashLines = hashValue->lines;
    while (hashLines->next != NULL) {
        hashLines = hashLines->next;
    }
    hashLines->next = (lineList) malloc(sizeof(struct lineList));
    hashLines->next->line = line;
    hashLines->next->next = NULL;
}

void insertFuncSymtable(char *name, int line, primitiveType type, funcType funcType, int paramsOrArgsCount) {

	hashValue = getHashValue(name);
    if (hashValue != NULL) {
        insertNewHashLines(line);
    } else {
        hashValue = (symTable) malloc(sizeof(struct symTable));
        hashValue->name = name;
        hashValue->type = type;
        hashValue->id = isFunc;
        hashValue->scope = "global";

        if (funcType == isDeclFunc) {
            hashValue->params = paramsOrArgsCount;
        } else {
            hashValue->args = paramsOrArgsCount;
        }
        createHashValue(line);
    }
}

void insertVarSymtable(char *name, int line, char *scope, primitiveType type) {

    hashValue = getHashValue(name);
    while (hashValue != NULL && (strcmp(hashValue->name, name) != 0 || strcmp(hashValue->scope, scope) != 0)) {
        hashValue = hashValue->next;
    }

    if (hashValue != NULL) {
        insertNewHashLines(line);
    } else {
		hashValue = (symTable) malloc(sizeof(struct symTable));        
        hashValue->name = name;
        hashValue->type = type;
        hashValue->id = isVar;
        hashValue->idNumber = varCount;
        varCount++;

        /* Check variable scope (global or function scope) */
        if (strcmp(scope, "global") == 1) {
            char* globalScope = malloc(sizeof(char));
            strcat(globalScope, "global");
            hashValue->scope = globalScope;
        } else {
            hashValue->scope = scope;
        }
        createHashValue(line);
    }
}

int getFuncParamsCount(char *name) {
    hashValue = getHashValue(name);
    return hashValue->params;
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

void getIdType(char *name, char *scope, primitiveType *type) {
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
    printf(" |   Nome            |  ID  |  Tipo ID  | Tipo Dado  |  Params |   Escopo            |   Linhas  	      \n");
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

                printf(" |  %-7s ", getPrimitiveType(hashValue->type));

                if (hashValue->id == isVar) {
                    printf(" |  %-7s  ", "Var");
                    printf(" | %6d ", 0);
                } else {
                    printf(" |  %-7s  ", "Func");
                    printf(" | %6d ", hashValue->params);
                }

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

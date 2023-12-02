%{
	#include "utils.h"
  #include "token.h"
  #include "scanner.h"
  #include "syntaxTree.h"
  #include "parserHelper.h"

  char expName[BUFFER_MAX_LENGTH]; 
  char variableName[BUFFER_MAX_LENGTH]; 
  char functionName[BUFFER_MAX_LENGTH];

  int currentLine = 1;
  int functionCurrentLine = 1;
  int firstFunc = 1;
  	
	stack functionStack;
  treeNode *tree;
%}

%token IF 257
%token INT 258
%token RETURN 259
%token VOID 260
%token ELSE 261
%token WHILE 262
%token PLUS 263
%token MINUS 264
%token MULT 265
%token SLASH 266
%token ASSIGN 267
%token EQUAL 268
%token LT 269
%token GT 270
%token LTE 271
%token GTE 272
%token DIF 273
%token SEMICOLON 274
%token COMMA 275
%token OPARENT 276
%token CPARENT 277
%token OBRACKET 278
%token CBRACKET 279
%token OKEY 280
%token CKEY 281
%token IDENTIFIER 282
%token NUMBER 283

%start program

%%
program : list_decl { tree = $1; };

list_decl : list_decl decl { 
                YYSTYPE node = $1;
                if (node != NULL)  {
					        while (node->child[0]->sibling != NULL) node = node->child[0]->sibling;
                    node->child[0]->sibling = $2;
                    $$ = $1;
                  }
                  else $$ = $2;
                } 
              	| decl { $$ = $1; };

decl : var_decl { $$ = $1; } | func_decl { $$ = $1; };

var_decl : exp_type IDENTIFIER { 
                strncpy(expName , tokenID, sizeof(tokenID + 1));
                currentLine = lineCount;
              }
              SEMICOLON {		
                $$ = $1;
                YYSTYPE declNode = createDeclNode(declVar);
                declNode->key.name = expName;
                declNode->line = currentLine;
                declNode->type = $1->type;
                $$->child[0] = declNode;
              }
            	| exp_type IDENTIFIER { 
                strncpy(expName , tokenID, sizeof(tokenID + 1));
                currentLine = lineCount;
              }
            	OBRACKET NUMBER CBRACKET SEMICOLON { 
                  $$ = $1;
                  YYSTYPE expNode = createExpNode(expNum);
                  expNode->key.value = atoi(tokenNUM);
                  expNode->type = Integer;

                  YYSTYPE declNode = createDeclNode(declVar);
                  declNode->key.name = expName;
                  declNode->line = currentLine;
                  declNode->child[0] = expNode;

                  if ($1->type == Integer) declNode->type = Array;
                  else declNode->type = Void;            
                  $$->child[0] = declNode;
                };

exp_type : INT { 
              $$ = createDeclNode(declIdType); 
              $$->type = Integer;
            }
			      | VOID { 
              $$ = createDeclNode(declIdType); 
              $$->type = Void; 
            };

func_decl : exp_type IDENTIFIER {
                strncpy(functionName, tokenID, sizeof(tokenID + 1));
                functionCurrentLine = lineCount;
              }
              OPARENT params CPARENT bloc_decl {
				        $$ = $1;
                YYSTYPE declNode = createDeclNode(declFunc);

                declNode->child[0] = $5;
                declNode->child[1] = $7;
                declNode->key.name = functionName;
                declNode->line = functionCurrentLine;
                declNode->type = $1->type;                
                $$->child[0] = declNode;
              };

params : list_params { $$ = $1; } 
		      | VOID { 
			      $$ = createExpNode(expId); 
			      $$->key.name = "void"; 
			      $$->type = Void; 
		      };

list_params : list_params COMMA arg {                 
				        YYSTYPE node = $1;
                if (node != NULL) {
                  while (node->sibling != NULL) node = node->sibling;
                  node->sibling = $3;
                  $$ = $1;
                }
                else $$ = $3;
              }
			        | arg { $$ = $1; };

arg : exp_type IDENTIFIER {
            $$ = $1;
            strncpy(expName , tokenID, sizeof(tokenID + 1));
            currentLine = lineCount;

            YYSTYPE declNode = createDeclNode(declVar); 

            declNode->key.name = expName;
            declNode->line = currentLine;
            declNode->type = $1->type;
            
            $$->child[0] = declNode;
          } | exp_type IDENTIFIER {
            strncpy(expName, tokenID, sizeof(tokenID + 1));
            currentLine = lineCount;
          }
    	    OBRACKET CBRACKET {    
            $$ = $1;
            YYSTYPE declNode = createDeclNode(declVar);

            declNode->key.name = expName;
            declNode->line = currentLine;
            
            if ($1->type == Integer) declNode->type = Array;
            else declNode->type = $1->type;
            $$->child[0] = declNode;
          };
  
bloc_decl : OKEY local_decl list_stmt CKEY { 
              YYSTYPE node = $2;                                
              if (node != NULL) {
                  while (node->sibling != NULL) node = node->sibling;
                  node->sibling = $3;
                  $$ = $2;
              }
              else $$ = $3;
            };

local_decl : local_decl var_decl {
                  YYSTYPE node = $1;                  
                  if (node != NULL) {
                    while (node->sibling != NULL) node = node->sibling;
                    node->sibling = $2;
                    $$ = $1;
                  }
                  else $$ = $2;
                } 
				        | { $$ = NULL; };

list_stmt : list_stmt stmt {    
				      YYSTYPE node = $1;                      
              if (node != NULL) {
					      while (node->sibling != NULL) node = node->sibling;
                node->sibling = $2;
                $$ = $1;
              }
              else $$ = $2;
            }
				    | { $$ = NULL; };

stmt : exp_decl     { $$ = $1; }
     | bloc_decl    { $$ = $1; }
     | cond_decl    { $$ = $1; }
     | loop_decl    { $$ = $1; }
     | return_decl  { $$ = $1; };

exp_decl : exp SEMICOLON { $$ = $1; } | SEMICOLON { $$ = NULL; };

cond_decl : IF OPARENT exp CPARENT stmt { 
                $$ = createStmtNode(stmtIf);
                $$->child[0] = $3;
                $$->child[1] = $5;
              }
              | IF OPARENT exp CPARENT stmt ELSE stmt { 
                $$ = createStmtNode(stmtIf);
                $$->child[0] = $3;
                $$->child[1] = $5;
                $$->child[2] = $7;
              };

loop_decl : WHILE OPARENT exp CPARENT stmt { 
              $$ = createStmtNode(stmtWhile);
              $$->child[0] = $3;
              $$->child[1] = $5;
            };

return_decl : RETURN SEMICOLON { 
                $$ = createStmtNode(stmtReturn);
              }   
              | RETURN exp SEMICOLON {	
                $$ = createStmtNode(stmtReturn);
                $$->child[0] = $2;
              };

exp : var ASSIGN exp { 
              $$ = createStmtNode(stmtAttrib);
              $$->child[0] = $1;
              $$->child[1] = $3;
              $$->type = Integer;
              $$->key.op = ASSIGN; 
            }
            | exp_simple { $$ = $1; };

var : IDENTIFIER { 
        strncpy(variableName, tokenID, sizeof(tokenID + 1));
        currentLine = lineCount;

        $$ = createExpNode(expId);
        $$->key.name = variableName;
        $$->line = currentLine;
        $$->type = Void;
      }
      | IDENTIFIER { 
        strncpy(variableName, tokenID, sizeof(tokenID + 1));
        currentLine = lineCount;
      } 
      OBRACKET exp CBRACKET {
        $$ = createExpNode(expId);
        $$->key.name = variableName;
        $$->line = currentLine;
        $$->child[0] = $4;
        $$->type = Integer; 
      };

exp_simple : exp_sum operator exp_sum {
                $$ = createExpNode(expOp);  
                $$->child[0] = $1;
                $$->child[1] = $3;
                $$->key.op = $2->key.op; 
              } 
				      | exp_sum { $$ = $1; };

operator : LTE { 
              $$ = createExpNode(expId); 
              $$->key.op = LTE; 
            }
            | LT { 
              $$ = createExpNode(expId); 
              $$->key.op = LT; 
            }
            | GT { 
              $$ = createExpNode(expId); 
              $$->key.op = GT; 
            }
            | GTE { 
              $$ = createExpNode(expId); 
              $$->key.op = GTE; 
            }
            | EQUAL {
              $$ = createExpNode(expId); 
              $$->key.op = EQUAL; 
            }
            | DIF {
              $$ = createExpNode(expId); 
              $$->key.op = DIF; 
            };

exp_sum : exp_sum sum term { 
              $$ = createExpNode(expOp);
              $$->child[0] = $1;
              $$->child[1] = $3;
              $$->key.op = $2->key.op; 
            } 
			      | term { $$ = $1; };

sum : PLUS {
          $$ = createExpNode(expId); 
          $$->key.op = PLUS; 
        }
        | MINUS {
          $$ = createExpNode(expId); 
          $$->key.op = MINUS;
        };

term : term mult factor {
            $$ = createExpNode(expOp);
            $$->child[0] = $1;
            $$->child[1] = $3;
            $$->key.op = $2->key.op; 
          }
		      | factor { $$ = $1; };

mult : MULT { 
          $$ = createExpNode(expId); 
          $$->key.op = MULT;
        }
        | SLASH {
          $$ = createExpNode(expId); 
          $$->key.op = SLASH;
        };

factor : OPARENT exp CPARENT { $$ = $1; }
        | var { $$ = $1; }
        | activate { $$ = $1; }
        | NUMBER { 
          $$ = createExpNode(expNum);
          $$->key.value = atoi(tokenNUM);
          $$->type = Integer;
        };

activate : IDENTIFIER {
			    if (firstFunc) {
            initStack(&functionStack);
            firstFunc = 0;
          }
            push(&functionStack, strncpy(functionName, tokenID, sizeof(tokenID + 1)));
            currentLine = lineCount;
          }
          OPARENT arguments CPARENT {   
              $$ = createStmtNode(stmtFunc);
              $$->child[1] = $4; 
              $$->key.name = pop(&functionStack);
              $$->line = currentLine;
            };

arguments : list_arg { $$ = $1; } | { $$ = NULL; };

list_arg : list_arg COMMA exp { 
                YYSTYPE node = $1;
                if (node != NULL) {
                    while (node->sibling != NULL) node = node->sibling;
                    node->sibling = $3;
                    $$ = $1;
                }
                else $$ = $3;
              }
			        | arg { $$ = $1; } 
			        | exp { $$ = $1; };

%%

int yylex() {

  struct token tk = lexicalAnalysis(file);
  if (tk.type == 0) {
    return YYEOF;
  }

  return tk.type + 256;
}

treeNode *parse() { 

	yyparse(); 
	return tree; 
}

int yyerror(char *errorMsg) {
	
  printf("Erro sintatico");
  return 0;
}

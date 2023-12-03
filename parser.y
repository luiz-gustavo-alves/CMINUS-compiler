%{
	#include "utils.h"
  #include "token.h"
  #include "scanner.h"
  #include "syntaxTree.h"
  #include "parserHelper.h"

  char *expName;
  char *variableName;
  char *functionName;
  char *lastToken;

  int currentLine = 1;
  int functionCurrentLine = 1;

  treeNode *tree;
  callList list;
%}

%token IF 1
%token INT 2
%token RETURN 3
%token VOID 4
%token ELSE 5
%token WHILE 6
%token PLUS 7
%token MINUS 8
%token MULT 9
%token SLASH 10
%token ASSIGN 11
%token EQUAL 12
%token LT 13
%token GT 14
%token LTE 15
%token GTE 16
%token DIF 17
%token SEMICOLON 18
%token COMMA 19
%token OPARENT 20
%token CPARENT 21
%token OBRACKET 22
%token CBRACKET 23
%token OKEY 24
%token CKEY 25
%token IDENTIFIER 26
%token NUMBER 27

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
                expName = getTokenName(tokenID);
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
                expName = getTokenName(tokenID);
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
                functionName = getTokenName(tokenID);
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
            expName = getTokenName(tokenID);

            YYSTYPE declNode = createDeclNode(declVar); 

            declNode->key.name = expName;
            declNode->line = currentLine;
            declNode->type = $1->type;
            
            $$->child[0] = declNode;
          } | exp_type IDENTIFIER {
            expName = getTokenName(tokenID);
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
        expName = getTokenName(tokenID);
        $$ = createExpNode(expId);
        $$->key.name = expName;
        $$->line = currentLine;
        $$->type = Void;
      }
      | IDENTIFIER { 
        variableName = getTokenName(tokenID);
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
            insertNode(&list, getTokenName(tokenID));
          }
          OPARENT arguments CPARENT {   
              $$ = createStmtNode(stmtFunc);
              $$->child[1] = $4; 
              $$->key.name = getLastNode(&list);
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
    lexicalError = 1;
    return YYEOF;
  }

  if (tk.type == 256) {
    return YYEOF;
  }

  lastToken = getTokenName(tokenNames[0][tk.type - 1]);
  currentLine = tk.line;
  return tk.type;
}

treeNode *parse() { 

	yyparse(); 
	return tree; 
}

int yyerror(char *errorMsg) {
	if (lexicalError) return 1;

  printf("(!) ERRO SINTATICO: Linha: %d | Token: %s\n", lineCount, lastToken);
  syntaxError = 1;
  return 1;
}

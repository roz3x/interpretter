%{
 #include "y.tab.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include "common.h"
 int yyerror(const char* );
%}

%token INT FLOAT STRING ID OPENPR CLOSEPR OPENBR  CLOSEBR SEMICOLON 
%token EQUALS CONST_INT CONST_STRING
%start function_decl
%union {
	int ival; 
	char* sval;
}

%type <sval> name function_decl CONST_STRING
%type <ival> consts stmt CONST_INT statements

%%

function_decl : type name OPENPR CLOSEPR OPENBR  statements CLOSEBR  
		{ printStatementChain($6); }
	
name : ID  { $$=yylval.sval; } 


type : INT 
	| FLOAT 
	| STRING
	
statements :  { $$ = -1; }
	| stmt SEMICOLON statements { appendStatements($1 , $3) ; $$ = $1 ; }

stmt : /*could be nothing */        { $$ = makeUniqeStatement(-1 , NULL , NULL , NULL , 0); }
	| type name                     { $$ = makeUniqeStatement(0 , $2 , NULL , NULL , 0); }
	| type name EQUALS CONST_INT    { $$ = makeUniqeStatement(1 , $2 , NULL , NULL , $4); }
	| type name EQUALS CONST_STRING { $$ = makeUniqeStatement(2 , $2 , NULL , $4 , 0); }
	| name EQUALS name              { $$ = makeUniqeStatement(3 , $1 , $3 , NULL , 0); }
	| name EQUALS CONST_INT         { $$ = makeUniqeStatement(4 , $1 , NULL , NULL , $3);}
	| name EQUALS CONST_STRING      { $$ = makeUniqeStatement(5 , $1 , NULL , $3 , 0);}

consts : CONST_INT {$$ = yylval.ival;} /* change this */
	| CONST_STRING {$$ = yylval.sval;}
%%
int main() { 
	yyparse() ; 
	exec_statements();
	printf("success\n");
}
extern int yylineno;
extern char* yytext;
int yyerror(const char* msg) { 
	printf("error :%d txt: %s\n", yylineno, yytext);
	exit(0);
}
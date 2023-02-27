%{
 #include "y.tab.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include "common.h"
 int yyerror(const char* );
%}

%token INT FLOAT STRING ID OPENPR CLOSEPR OPENBR  CLOSEBR SEMICOLON 
%token EQUALS CONST_INT CONST_STRING VARIABLE
%token PLUS MINUS MUL DIV EXPR
%start function_decl
%union {
	int ival; 
	char* sval;
} 

%type <sval> name function_decl CONST_STRING
%type <ival> consts stmt CONST_INT statements expr nc

%%

function_decl : type name OPENPR CLOSEPR OPENBR  statements CLOSEBR  
		{ printStatementChain($6); }
	
name : ID  { $$=yylval.sval; } 


type : INT 
	| FLOAT 
	| STRING
	
statements :  { $$ = -1; }
	| stmt SEMICOLON statements { appendStatements($1 , $3) ; $$ = $1 ; }

stmt : /*could be nothing */        { $$ = makeUniqeStatement(-1 ,NULL, 0); }
	| type name                     { $$ = makeUniqeStatement(0 , $2 , 0); } /* this will give it garabage value ? */
	| type name EQUALS nc           { $$ = makeUniqeStatement(1 , $2 , $4); }
	| name EQUALS nc  		        { $$ = makeUniqeStatement(2 , $1 , $3);}

nc : name {$$ = createVariableFrame($1); } /*DONE:  make this so that we get index of datafram from name , need trie for this */
	| consts {$$ = $1;}
	| expr   {$$ = makeDataFrameFromExpr($1);} /* statements themselves will give ref for the eval frame */

expr : nc PLUS nc {$$ = makeExpr($1 , PLUS, $3); }
	| nc MINUS nc {$$ = makeExpr($1 , MINUS, $3); } 
	| nc MUL nc   {$$ = makeExpr($1 , MUL , $3); }
	| nc DIV nc   {$$ = makeExpr($1 , DIV , $3); }
	| OPENPR expr CLOSEPR { $$ = $2; }

consts : CONST_INT {$$ = createIntegerDataFrame(yylval.ival);} /* both have same format  */
	| CONST_STRING {$$ = createStringDataFrame(yylval.sval);}
%%
int main() { 
	yyparse() ; 
	exec_statements();
	printf("Successful\n");
}
extern int yylineno;
extern char* yytext;
int yyerror(const char* msg) { 
	printf("error :%d txt: %s\n", yylineno, yytext);
	exit(0);
}
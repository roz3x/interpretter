%{
 #include "y.tab.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include "common.h"
 int yyerror(const char* );
%}

%token INT FLOAT STRING ID OPENPR CLOSEPR OPENBR  CLOSEBR SEMICOLON 
%token EQUALS CONST_INT CONST_STRING VARIABLE
%token PLUS MINUS MUL DIV EXPR IF  COMMA
%token COMP LT GT GTE LTE OR AND
%token FUNCTION_CALL
%start function_decl
%union {
	int ival; 
	char* sval;
} 

%type <sval> name function_decl CONST_STRING
%type <ival> consts stmt CONST_INT statements expr nc arg_list function_call

%%

function_decl : type name OPENPR CLOSEPR OPENBR  statements CLOSEBR  
		{ registerFunctionName($2, $6); }
	
name : ID  { $$=yylval.sval; } 


type : INT 
	| FLOAT 
	| STRING
	
statements :  { $$ = -1; }
	| stmt  statements { appendStatements($1 , $2) ; $$ = $1 ; }

stmt : /*could be nothing */             { $$ = makeUniqeStatement(-1 ,NULL, 0); }
	| type name SEMICOLON                { $$ = makeUniqeStatement(0 , $2 , 0); } /* this will give it garabage value ? */
	| type name EQUALS nc SEMICOLON      { $$ = makeUniqeStatement(1 , $2 , $4); }
	| name EQUALS nc SEMICOLON	         { $$ = makeUniqeStatement(2 , $1 , $3);}
	| IF OPENPR nc CLOSEPR OPENBR statements CLOSEBR       { $$ = makeIfStatement($3,$6); }
	| SEMICOLON                          { $$ = makeUniqeStatement(-1 , NULL , 0 ); /* dummy statement */}	
	| function_call SEMICOLON			 { $$ = makeFunctionCallStatement($1); }

nc : name {$$ = createVariableFrame($1); } /*DONE:  make this so that we get index of datafram from name , need trie for this */
	| consts {$$ = $1;}
	| expr   {$$ = makeDataFrameFromExpr($1);} /* statements themselves will give ref for the eval frame */

expr : nc PLUS nc {$$ = makeExpr($1 , PLUS, $3); }
	| nc MINUS nc {$$ = makeExpr($1 , MINUS, $3); } 
	| nc MUL nc   {$$ = makeExpr($1 , MUL , $3); }
	| nc DIV nc   {$$ = makeExpr($1 , DIV , $3); }
	| nc COMP nc  {$$ = makeExpr($1 , COMP , $3);}
	| nc GT nc    {$$ = makeExpr($1 , GT , $3);}
	| nc GTE nc   {$$ = makeExpr($1 , GTE , $3);}
	| nc LT nc    {$$ = makeExpr($1 , LT , $3);}
	| nc LTE nc   {$$ = makeExpr($1 , LTE , $3);}
	| nc OR nc    {$$ = makeExpr($1 , OR , $3);}
	| nc AND nc   {$$ = makeExpr($1 , AND , $3);}
	| OPENPR expr CLOSEPR { $$ = $2; }

consts : CONST_INT {$$ = createIntegerDataFrame(yylval.ival);} /* both have same format  */
	| CONST_STRING {$$ = createStringDataFrame(yylval.sval);}

arg_list : nc               { $$  = makeArgList($1 , -1); }
	| nc COMMA arg_list     { $$ = makeArgList($1 , $3);}
	| 	                    { $$ = -1; }

function_call: 
	name OPENPR arg_list CLOSEPR { $$ = makeFunctionCall($1 , $3);}
%%
int main() { 
	freopen("in", "r", stdin);
	init_trie();
	yyparse() ; 
	callFunction("main");
	printf("Successful\n");
}
extern int yylineno;
extern char* yytext;
int yyerror(const char* msg) { 
	printf("error :%d txt: %s\n", yylineno, yytext);
	exit(0);
}
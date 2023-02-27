%{
#include "y.tab.h"
#include <stdio.h>
#include "string.h"
%}


%%
"int"     {return INT;} 
"float"   {return FLOAT;} 
"("       {return OPENPR;}
")"       {return CLOSEPR;}
"{"       {return OPENBR;}
"}"       {return CLOSEBR;}
";"       {return SEMICOLON;}
"="       {return EQUALS;}
"+"       {return PLUS; }
"-"       {return MINUS;}
"/"       {return DIV; }
"*"       {return MUL;}
[0-9]+    {yylval.ival = atoi(yytext); return CONST_INT;}
\"(\\.|[^\"])*\" {yylval.sval = strdup(yytext); return CONST_STRING;}
[_a-zA-Z][0-9_a-zA-Z]*  { yylval.sval = strdup(yytext); return ID; }

.   { }

%%
int yywrap() {return 1;}
%{
#include <stdlib.h>
#include <cstdlib>
#include "ast.hh"
#include "parser.tab.hh"
%}
%option outfile="lexer.yy.cc" header="lexer.yy.hh"
 
%%
[0-9]+     { yylval.num = atoi(yytext); return NUMBER; }
[\n]        { return END; }
[\t ]+     ;
[+\-*/()]  { return *yytext; }
.           { return yytext[0]; }
%%

int yywrap() { return 1; }

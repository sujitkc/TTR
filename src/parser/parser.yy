%{
#include <cstdio>
#include <cstdlib>
#include "ast.hh"
#include "parser.tab.hh"

void yyerror(const char *s);
int yylex();
using namespace std;
unique_ptr<ASTNode> root;
%}

%union {
    int num;
    ASTNode* node;
}

%token <num> NUMBER END
%type <node> expr
%left '+' '-'
%left '*' '/'
%right UMINUS

%%
input:
    | input line
;

line:
    expr END { root.reset($1); root->print(); cout << endl; }
;

expr:
    NUMBER       { $$ = new NumberNode($1); }
  | expr '+' expr { $$ = new BinaryOpNode('+', $1, $3); }
  | expr '-' expr { $$ = new BinaryOpNode('-', $1, $3); }
  | expr '*' expr { $$ = new BinaryOpNode('*', $1, $3); }
  | expr '/' expr { $$ = new BinaryOpNode('/', $1, $3); }
  | '-' expr %prec UMINUS { $$ = new BinaryOpNode('-', new NumberNode(0), $2); }
  | '(' expr ')' { $$ = $2; }
;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

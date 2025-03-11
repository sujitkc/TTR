#include <cstdio>
#include "ast.hh"
#include "parser.tab.hh"


int main() {
    printf("Simple Calculator with AST. Enter expressions:\n");
    yyparse();
    return 0;
}


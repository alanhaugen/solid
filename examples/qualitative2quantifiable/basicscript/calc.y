// Calculator bison input
%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *);
%}

// Declare tokens
%token NUMBER
%token ADD SUB MUL DIV ABS
%token EOL

%%

calclist: // nothing
    | calclist exp EOL { printf("= %d\n", $1); }
    ;

exp: factor
    | exp ADD factor { $$ = $1 + $3; }
    | exp SUB factor { $$ = $1 - $3; }
    ;

factor: term
    | factor MUL term { $$ = $1 * $3; }
    | factor DIV term { $$ = $1 / $3; }
    ;

term: NUMBER
    | ABS term { $$ = $2 >= 0 ? $2 : - $2; }
    ;
%%

void yyerror(char const *s) {
    fprintf(stderr, "error: %s\n", s);
}
/*void yyerror(char *s, ...)
{
 va_list ap;
 va_start(ap, s);
 fprintf(stderr, "%d: error: ", yylineno);
 vfprintf(stderr, s, ap);
 fprintf(stderr, "\n");
}*/
int main(int argc, char **argv) {
    printf("test");
    yyparse();
}

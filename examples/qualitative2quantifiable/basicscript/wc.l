/* just like Unix wc */
%{
int chars = 0;
int words = 0;
int lines = 0;
%}

%%

[a-zA-Z]+  { words++; chars += strlen(yytext); }
\n         { chars++; lines++; }
.          { chars++; }

%%

int yywrap() {
   /*// open next reference or source file and start scanning
   if((yyin = compiler->getNextFile()) != NULL) {
      line = 0; // reset line counter for next source file
      return 0;
   }*/
   return 1;
}

int main(int argc, char **argv) {
    yylex();
    printf("%8d%8d%8d\n", lines, words, chars);

    return 0;
}

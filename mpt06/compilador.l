%{


#include<stdio.h>
#include<string.h>

#include "compilador.h"
#include "compilador.tab.h"

//#define IMPRIME(STR) {};
#define IMPRIME(STR) printf(" %s\t# %s\n", STR, token);

int nl = 1;  

%}

pulo_linha [\n]

descarte [\t ]+

ident [a-zA-Z][a-zA-Z0-9]*

numero [0-9]+

comentario "(*".*"*)"

%%

{pulo_linha} {nl++;} 

{descarte}

{comentario}

program { simbolo  = simb_program;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("program\t\t");
          return PROGRAM;
 }

var      { simbolo = simb_var;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("var\t\t");
          return VAR;
 }

begin { simbolo = simb_begin;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("begin\t\t");
          return T_BEGIN;
 }

end { simbolo = simb_end;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("end\t\t");
          return T_END;
 }

write { simbolo  = simb_write;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("write\t\t");
          return WRITE;
      }

writeln { simbolo  = simb_writeln;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("writeln\t");
          return WRITELN;
        }

read { simbolo  = simb_read;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("read\t\t");
          return READ;
     }

integer { simbolo  = simb_integer;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("integer\t");
          return INTEGER;
        }

string { simbolo  = simb_string;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("string\t");
          return STRING;
       }

boolean { simbolo  = simb_boolean;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("boolean\t");
          return BOOLEAN;
        }

not  { simbolo = simb_not;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("not\t\t");
          return NOT;
     }

and  { simbolo = simb_and;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("and\t\t");
          return AND;
     }

or  { simbolo = simb_or;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("or\t\t");
          return OR;
    }

while  { simbolo = simb_while;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("while\t\t");
          return WHILE;
       }

do  { simbolo = simb_do;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("do\t\t");
          return DO;
    }

repeat  { simbolo = simb_repeat;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("repeat\t\t");
          return REPEAT;
        }

until  { simbolo = simb_until;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("until\t\t");
          return UNTIL;
       }

for  { simbolo = simb_for;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("for\t\t");
          return FOR;
     }

to  { simbolo = simb_to;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("to\t\t");
          return TO;
    }

downto  { simbolo = simb_downto;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("downto\t\t");
          return DOWNTO;
        }

if  { simbolo = simb_if;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("if\t\t");
          return IF;
    }

then  { simbolo = simb_then;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("then\t\t");
          return THEN;
      }

else  { simbolo = simb_else;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("else\t\t");
          return ELSE;
      }

false  { simbolo = simb_false;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("false\t\t");
          return FALSE;
       }

true  { simbolo = simb_true;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("true\t\t");
          return TRUE;
      }

procedure  { simbolo = simb_procedure;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("procedure\t");
          return PROCEDURE;
           }

function  { simbolo = simb_function;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("function\t");
          return FUNCTION;
          }

label  { simbolo = simb_label;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("label\t\t");
          return LABEL;
       }

goto  { simbolo = simb_goto;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("goto\t\t");
          return GOTO;
      }

:= { simbolo = simb_atribuicao;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("atribuicao\t");
          return ATRIBUICAO;  
 }

\; { simbolo = simb_ponto_e_virgula;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("pt_virgula\t");
          return PONTO_E_VIRGULA;
 }

\: { simbolo = simb_dois_pontos;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("dois pontos\t");
          return DOIS_PONTOS;
 }

\, { simbolo = simb_virgula;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("virgula\t");
          return VIRGULA;
 }


\. { simbolo = simb_ponto;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("ponto\t\t");
          return PONTO; 
    }

\(  { simbolo = simb_abre_parenteses;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("abre_parenteses");
          return ABRE_PARENTESES;
    }

\)  { simbolo = simb_fecha_parenteses;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("fecha_parenteses");
          return FECHA_PARENTESES;
 }


\+  { simbolo = simb_mais;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("mais\t\t");
          return MAIS;
    }

\-  { simbolo = simb_menos;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("menos\t\t");
          return MENOS;
    }

\*  { simbolo = simb_mult;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("mult\t\t");
          return MULT;
    }

\/  { simbolo = simb_div;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("div\t\t");
          return DIV;
    }
    
div  { simbolo = simb_div;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("div\t\t");
          return DIV;
    }

\>  { simbolo = simb_maior;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("maior\t\t");
          return MAIOR;
    }

\<  { simbolo = simb_menor;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("menor\t\t");
          return MENOR;
    }

\=  { simbolo = simb_igual;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("igual\t\t");
          return IGUAL;
    }

\#  { simbolo = simb_diferente;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("diferente\t");
          return DIFERENTE;
    }

{ident} { simbolo = simb_identificador;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("ident\t\t");
          return IDENT;
        }

{numero} { simbolo  = simb_numero;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("numero\t\t");
          return NUMERO;
         }

%%
void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

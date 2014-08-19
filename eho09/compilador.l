%{


#include<stdio.h>
#include<string.h>

#include "compilador.h"
#include "compilador.tab.h"

//#define IMPRIME(STR) {};
#define IMPRIME(STR) printf("%s    # %s \n", STR, token);

int nl = 1;  

%}

pulo_linha [\n]

descarte [\t ]+

ident [a-zAZ][a-zAZ1-9]*

numero [0-9]+
numero_real {numero}.{numero}

comentario "(*".*"*)"

%%

{pulo_linha} {nl++;} 

{descarte}

{comentario}

program { simbolo  = simb_program;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("program ");
          return PROGRAM;
 }

var      { simbolo = simb_var;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("var  ");
          return VAR;
 }
 
label      { simbolo = simb_label;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("label  ");
          return LABEL;
 }
 
goto { simbolo = simb_goto;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("goto ");
          return GOTO;
 }

begin { simbolo = simb_begin;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("begin ");
          return T_BEGIN;
 }

end { simbolo = simb_end;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("end  ");
          return T_END;
 }

integer { simbolo = simb_int;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("integer  ");
          return T_INTEGER;
}

real { simbolo = simb_real;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("real  ");
          return T_REAL;
}

char { simbolo = simb_chr;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("integer  ");
          return T_CHR;
}

string { simbolo = simb_str;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("string  ");
          return T_STR;
}

boolean { simbolo = simb_bool;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("boolean  ");
          return T_BOOL;
}

:= { simbolo = simb_atribuicao;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" atribuicao  ");
          return ATRIBUICAO;  
 }

\; { simbolo = simb_ponto_e_virgula;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" pt_virgula  ");
          return PONTO_E_VIRGULA;
 }

\: { simbolo = simb_dois_pontos;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" dois pontos ");
          return DOIS_PONTOS;
 }

\, { simbolo = simb_virgula;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" virgula  ");
          return VIRGULA;
 }


\. { simbolo = simb_ponto;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" ponto  ");
          return PONTO; 
    }

\* { simbolo = simb_div;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" mul  ");
          return MUL;
    }

\/ { simbolo = simb_div;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" div  ");
          return DIV;
    }

div { simbolo = simb_div2;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" true ");
          return DIV2;
 }

\- { simbolo = simb_menos;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" menos  ");
          return MENOS;
    }

\+ { simbolo = simb_mais;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" mais  ");
          return MAIS;
    }

\(  { simbolo = simb_abre_parenteses;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" abre_parenteses  ");
          return ABRE_PARENTESES;
    }

\)  { simbolo = simb_fecha_parenteses;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" fecha_parenteses ");
          return FECHA_PARENTESES;
 }

\>  { simbolo = simb_maior;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" maior ");
          return MAIOR;
 }

\<  { simbolo = simb_menor;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" menor ");
          return MENOR;
 }

\=  { simbolo = simb_igual;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" igual ");
          return IGUAL;
 }

\<\>  { simbolo = simb_diferente;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" diferente ");
          return DIFERENTE;
 }

>=  { simbolo = simb_maior_igual;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" maior_ou_igual ");
          return MAIOR_OU_IGUAL;
 }

\<=  { simbolo = simb_menor_igual;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" menor_ou_igual ");
          return MENOR_OU_IGUAL;
 }

not  { simbolo = simb_not;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" not ");
          return NOT;
 }

or  { simbolo = simb_or;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" not ");
          return OR;
 }

and  { simbolo = simb_and;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" not ");
          return AND;
 }

true { simbolo = simb_true;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" true ");
          return TRUE;
 }

false { simbolo = simb_false;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" false ");
          return FALSE;
 }

write { simbolo = simb_write;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" write ");
          return WRITE;
 }

read { simbolo = simb_read;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" read ");
          return READ;
 }

while { simbolo = simb_while; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" while ");
          return WHILE;
 }

do { simbolo = simb_do; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" do ");
          return DO;
 }

for { simbolo = simb_for; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" for ");
          return FOR;
 }
 
to { simbolo = simb_to; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" to ");
          return TO;
 }

repeat { simbolo = simb_repeat; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" repeat ");
          return REPEAT;
 }

until { simbolo = simb_until; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" until ");
          return UNTIL;
 }

if { simbolo = simb_until; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" if ");
          return IF;
 }

then { simbolo = simb_then; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" then ");
          return THEN;
 }

else { simbolo = simb_else; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" else ");
          return ELSE;
 }

procedure { simbolo = simb_proc; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" procedure ");
          return PROC;
 }

function { simbolo = simb_func; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" function ");
          return FUNC;
 }

{ident} { simbolo = simb_identificador; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" ident  ");
          return IDENT;
 }
        
{numero} { simbolo = simb_numero; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" numero ");
          return NUMERO;
 }


%%
void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}

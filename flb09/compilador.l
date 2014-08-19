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

comentario "(*".*"*)"

%%

%{
/* Palavras Reservadas */
%}
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
 
while { simbolo = simb_while;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("while  ");
          return WHILE;
}

do { simbolo = simb_do;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" do ");
          return DO;
}

if { simbolo = simb_if;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("if ");
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

repeat { simbolo = simb_repeat;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("repeat ");
          return REPEAT;
}

until { simbolo = simb_until;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("until ");
          return UNTIL;
}

for { simbolo = simb_for;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("for ");
          return FOR;
}

to { simbolo = simb_to;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" to ");
          return TO;
}

downto { simbolo = simb_downto;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" downto ");
          return DOWNTO;
} 

procedure { simbolo = simb_proc;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" procedure ");
          return PROCEDURE;
} 

function { simbolo = simb_func;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" function ");
          return FUNCTION;
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
    
goto { simbolo = simb_goto;
         strncpy (token, yytext, TAM_TOKEN);
         IMPRIME(" goto ");
         return GOTO;
}     
label { simbolo = simb_label;
         strncpy (token, yytext, TAM_TOKEN);
         IMPRIME(" label ");
         return LABEL;
}   

%{
/* Simbolos */
%}

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
 
\+ { simbolo = simb_soma;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" soma  ");
          return SOMA; 
    }


\- { simbolo = simb_subtrai;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" subtrai  ");
          return SUBTRAI; 
    }
    

\div { simbolo = simb_divide;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" divide  ");
          return DIVIDE; 
    }


\* { simbolo = simb_multiplica;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" multiplica  ");
          return MULTIPLICA; 
    }
    
%{
/* Operadores Logicos */
%} 
\< { simbolo = simb_menor;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" menor  ");
          return MENOR; 
    }
    
\<= { simbolo = simb_menor_igual;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" menor_igual  ");
          return MENOR_IGUAL; 
    }


\> { simbolo = simb_maior;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" maior  ");
          return MAIOR; 
    }
    
\>= { simbolo = simb_maior_igual;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" maior_igual  ");
          return MAIOR_IGUAL; 
    }    
 
\= { simbolo = simb_igual;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" igual  ");
          return IGUAL; 
    }
    
    
\<> { simbolo = simb_diferente;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" diferente  ");
          return DIFERENTE; 
    }
        
or { simbolo = simb_ou;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" ou  ");
          return OU; 
    }
    

and { simbolo = simb_e;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" e  ");
          return E; 
    }
    
true { simbolo = simb_true;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" true  ");
          return TRUE; 
    }
    
false { simbolo = simb_false;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" false  ");
          return FALSE; 
    }
    
%{
/* Números & Identificadores */
%}    
    
{numero}	{
			simbolo = simb_numero;
			strncpy (token, yytext, TAM_TOKEN);
          	IMPRIME(" numero  ");
          	return NUMERO; 
		}

    
{ident} { simbolo = simb_identificador; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" ident  ");
          return IDENT;
        }


%%

void yyerror (char *s) {
    fprintf (stderr, "%s\n", s);
}


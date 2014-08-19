%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"

char str[str_max];
char id[str_max];
int num_vars = 0;
int proc_num_vars = 0;
int num_param = 0;
int tipo;
int nivel = 0;
int deslocamento = 0;
int categoria = 0;
int num_tipo = 0;
int rot = 0;
int aux;
int auxDes;
int auxNiv;
int eh_ref = 0;
int proc_func = 0;
int params_cont = 0;
int tipo_func;
ETdS *elemento = NULL;
ETdS *elemento2 = NULL;
TdS *ts;
PdT *pt;
PdR *pr; 
PdF *pfunc;
%}

%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES 
%token VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO
%token T_BEGIN T_END VAR IDENT NUMERO ATRIBUICAO LABEL GOTO
%token MAIOR MENOR IGUAL DIFERENTE MAIOR_OU_IGUAL MENOR_OU_IGUAL 
%token NOT OR AND FALSE TRUE
%token MAIS MENOS DIV MUL DIV2
%token T_INTEGER T_REAL T_STR T_CHR T_BOOL
%token WRITE READ FUNC PROC
%token WHILE DO FOR TO DOWNTO REPEAT UNTIL IF THEN ELSE

%nonassoc LOWER_THAN_ELSE /* dummy token */
%nonassoc ELSE

%%

programa: { 
		ts = inicializa();
		pt = inicializaPdT();
		pr = inicializaPdR();
		pfunc = inicializaPdF();
		geraCodigo (NULL, "INPP"); 
	} PROGRAM IDENT prog_params {
		num_param = 0;
	}PONTO_E_VIRGULA bloco PONTO {
		int i = 0;
		do {
        	elemento = desempilha(ts);
        } while( elemento != NULL && (elemento->categoria == Proc || elemento->categoria == Func)  && elemento->nivel > nivel);
        ts->topo++;
		while( elemento != NULL){
        	elemento = desempilha(ts);
        	num_vars++;
        }
        sprintf(str,"DMEM %d", num_vars);
		geraCodigo (NULL, str);
		geraCodigo (NULL, "PARA");
    }
;

bloco: 
	declara_rotulos
	parte_declara_vars
	procs
	{ 
	}
	comando_composto 
;

declara_rotulos: LABEL rotulos PONTO_E_VIRGULA
				| 
;

rotulos: rotulos VIRGULA IDENT { 
			if( !empilha(ts, RT, nivel, -1, token) ){
				sprintf(str,"%s ja existe", token);
            	imprimeErro(str);
            }
		}
		| rotulos VIRGULA NUMERO{
			if( !empilha(ts, RT, nivel, -1, token) ){
				sprintf(str,"%s ja existe", token);
            	imprimeErro(str);
            }
		}
		| IDENT {
			if( !empilha(ts, RT, nivel, -1, token) ){
				sprintf(str,"%s ja existe", token);
            	imprimeErro(str);
            }
		}
		| NUMERO {
			if( !empilha(ts, RT, nivel, -1, token) ){
				sprintf(str,"%s ja existe", token);
            	imprimeErro(str);
            }
		}
;

parte_declara_vars:  var  { 
				sprintf(str, "AMEM %d", num_vars);
				geraCodigo(NULL, str);
                num_vars = 0;
              }
;

var         : { } VAR declara_vars
            |
;

declara_vars: declara_vars declara_var 
            | declara_var 
;

declara_var : { } 
              lista_id_var DOIS_PONTOS 
              tipo {num_tipo = 0;}
              PONTO_E_VIRGULA
;

tipo        : T_INTEGER {insereTipo(ts, Int, num_tipo);}
			  | T_REAL {insereTipo(ts, Real, num_tipo);}
			  | T_STR {insereTipo(ts, Str, num_tipo);}
			  | T_CHR {insereTipo(ts, Chr, num_tipo);}
			  | T_BOOL {insereTipo(ts, Bool, num_tipo);}
;

lista_id_var: lista_id_var VIRGULA IDENT {
              	/* insere última vars na tabela de símbolos */
              	if( !empilha(ts, VS, nivel, num_vars, token) ){
					sprintf(str,"%s ja existe", token);
              		imprimeErro(str);
              	}
              	num_tipo++;
              	num_vars++;
            }
            | IDENT { 
    			/* insere última vars na tabela de símbolos */
    			/* insere vars na tabela de símbolos */
		      	if( !empilha(ts, VS, nivel, num_vars, token) ){
					sprintf(str,"%s ja existe", token);
		      		imprimeErro(str);
		      	}
		      	num_tipo++;
    			num_vars++; 
    		}
;

prog_params: ABRE_PARENTESES lista_idents FECHA_PARENTESES
	|
;

lista_idents: lista_idents VIRGULA IDENT 
            | IDENT
;

/*========================================procedimento========================================*/

params_dec: ABRE_PARENTESES lista_params FECHA_PARENTESES {params_cont = 0;num_tipo = 0;}
	|
;

referencia: VAR {eh_ref = 1;}
			| 
;

lista_params: lista_params PONTO_E_VIRGULA referencia lista_id_params DOIS_PONTOS tipo_param {eh_ref = 0; num_tipo = 0;}
			| referencia lista_id_params DOIS_PONTOS tipo_param {eh_ref = 0; num_tipo = 0;}
;

lista_id_params:  lista_id_params VIRGULA IDENT {
				if( !empilha(ts, PF, nivel, - 4 - num_param, token) ){
					sprintf(str,"%s ja existe", token);
		      		imprimeErro(str);
		      	}				
				num_param++;
				num_tipo++;
			}  
            | IDENT {
				if( !empilha(ts, PF, nivel, - 4 - num_param, token) ){
					sprintf(str,"%s ja existe", token);
		      		imprimeErro(str);
		      	}				
				num_param++;
				num_tipo++;
			}		
;

tipo_param:  T_INTEGER {
				if (eh_ref)
					insereTipoParam(ts, RefInt, num_tipo);
				else
					insereTipoParam(ts, Int, num_tipo); 
			}
			| T_REAL {
				if (eh_ref)
					insereTipoParam(ts, RefReal, num_tipo);
				else
					insereTipoParam(ts, Real, num_tipo); 
			}
			| T_STR  {
				if (eh_ref)
					insereTipoParam(ts, RefStr, num_tipo);
				else
					insereTipoParam(ts, Str, num_tipo); 
			}
			| T_CHR  {
				if (eh_ref)
					insereTipoParam(ts, RefChr, num_tipo);
				else
					insereTipoParam(ts, Chr, num_tipo); 
			}
			| T_BOOL  {
				if (eh_ref)
					insereTipoParam(ts, RefBool, num_tipo);
				else
					insereTipoParam(ts, Bool, num_tipo); 
			}
;

params_passagem: params_passagem VIRGULA { pfunc->topo->ind_param++; } expr {
				int aux = desempilhaPdT(pt);
				if( aux != pfunc->topo->func->params[pfunc->topo->ind_param]) {
					sprintf(str,"Tipo de parametro-%d nao compativel com a proc/func: %s()", pfunc->topo->ind_param, pfunc->topo->func->id);
					imprimeErro(str);
				}
				
			}
			| { pfunc->topo->ind_param++; } expr {
				int aux = desempilhaPdT(pt);
				if( aux != pfunc->topo->func->params[pfunc->topo->ind_param]) {
					sprintf(str,"Tipo de parametro-%d nao compativel com a proc/func: %s()", pfunc->topo->ind_param, pfunc->topo->func->id);
					imprimeErro(str);
				}
				
			}
;

params: ABRE_PARENTESES params_passagem FECHA_PARENTESES { }
	|
;

procs: procs2
	|
;

procs2: procs2 proc_func PONTO_E_VIRGULA
	| proc_func PONTO_E_VIRGULA
;

proc_func: FUNC {proc_func = Func;} assinatura DOIS_PONTOS tipo_func corpo 
	| PROC {proc_func = Proc;} assinatura corpo
;

tipo_func   : T_INTEGER {tipo_func = Int;}
			  | T_REAL {tipo_func = Real;}
			  | T_STR {tipo_func = Str;}
			  | T_CHR {tipo_func = Chr;}
			  | T_BOOL {tipo_func = Bool;}
;

assinatura: IDENT {
        nivel++;
		sprintf(str, "DSVS ROT%d", rot);
		empilhaPdR(pr, rot); 
		geraCodigo(NULL ,str);
        rot++;
		sprintf(str, "ROT%d", rot);
		geraCodigo(str, "NADA");
		if( !empilha(ts, proc_func, nivel, rot, token) ){
			sprintf(str,"%s ja existe", token);
      		imprimeErro(str);
      	}
		rot++;
		sprintf(str, "ENPR %d", nivel);
		geraCodigo(NULL, str);
		elemento = ts->topo;
	} params_dec {
		elemento->num_param = num_param;
		if( elemento->num_param > 10)
			imprimeErro("Numero de parametros superior a 10");
		
		preencheParams(ts, elemento, proc_func);
		elemento = NULL;
		num_param = 0;
	}
;

corpo: PONTO_E_VIRGULA bloco  {
		int num_vars_aux = 0;
		do {
        	elemento = desempilha(ts);
        } while( (elemento->categoria == Proc || elemento->categoria == Func) && elemento->nivel > nivel);
        ts->topo++;
		do{
			elemento = desempilha(ts);
        	num_vars_aux++;
        }while( elemento->categoria == VS);
		ts->topo++;
		do {
			elemento = desempilha(ts);
        } while( elemento->categoria == RT);
        ts->topo++;
		do {
			elemento = desempilha(ts);
        } while( elemento->categoria == PF);
       
        if( elemento->categoria != Proc || elemento->categoria != Func )
     		ts->topo++;
        sprintf(str,"DMEM %d", --num_vars_aux);
        geraCodigo (NULL, str);
        elemento->tipo = tipo_func;
        sprintf(str,"RTPR %d, %d", nivel, elemento->num_param);
        geraCodigo (NULL, str);
		sprintf(str, "ROT%d", desempilhaPdR(pr));
		geraCodigo(str, "NADA");
		nivel--;
		elemento = NULL;
    }
;

/*========================================comandos========================================*/
comando_composto: T_BEGIN comandos T_END
;

comandos:  comandos PONTO_E_VIRGULA comando 
		| comando 
;

subcomandos: T_BEGIN comandos T_END
			| comando
;

/*pt_virgula: PONTO_E_VIRGULA
			|
;*/

comando: atribuicao_ou_CP
		| NUMERO {
			elemento = buscaTs(ts, nivel, token);
			if(elemento == NULL) {
				sprintf(str,"Simbolo não encontrado: %s", token);
				imprimeErro(str);
			}
		} goto_dest
		| io
		| WHILE { 
			empilhaPdR(pr, rot); 
			sprintf(str, "ROT%d", rot);
			geraCodigo(str, "NADA");
			rot++;
		} expr {
			if( desempilhaPdT(pt) != Bool) {
				sprintf(str, "Operacao nao permitida: tipo incompativel");
				imprimeErro(str);
			}
			sprintf(str, "DSVF ROT%d", rot);
			empilhaPdR(pr, rot); 
			geraCodigo(NULL ,str);
			rot++;			
		} DO subcomandos {
			aux = desempilhaPdR(pr);
			sprintf(str, "DSVS ROT%d", desempilhaPdR(pr));
			geraCodigo(NULL, str);
			sprintf(str, "ROT%d", aux);
			geraCodigo(str, "NADA"); 
		}
		| FOR IDENT {
			strcpy(id, token);
			elemento = buscaTs(ts, nivel, id);
			if(elemento == NULL) {
				sprintf(str,"Simbolo não encontrado: %s", id);
				imprimeErro(str);
			}
			empilhaPdF(pfunc, -1, elemento);
			empilhaPdT(pt, elemento->tipo); 
		} atribuicao { 
			sprintf(str, "CRVL %d,%d", auxNiv, auxDes); 
			geraCodigo(NULL, str);
		} to_downto {
			sprintf(str, "DSVF ROT%d", rot);
			empilhaPdR(pr, rot); 
			geraCodigo(NULL ,str);
			rot++;
		} DO subcomandos {
			aux = desempilhaPdR(pr);
			sprintf(str, "DSVS ROT%d", desempilhaPdR(pr));
			geraCodigo(NULL, str);
			sprintf(str, "ROT%d", aux);
			geraCodigo(str, "NADA"); 
		}
		| REPEAT {
			empilhaPdR(pr, rot); 
			sprintf(str, "ROT%d", rot);
			geraCodigo(str, "NADA");
			rot++;
		} comandos UNTIL expr {
			if( desempilhaPdT(pt) != Bool) {
				sprintf(str, "Operacao nao permitida: tipo incompativel");
				imprimeErro(str);
			}
			sprintf(str, "DSVF ROT%d", desempilhaPdR(pr));
			geraCodigo(NULL ,str);
		} 
		| cond_if
		| goto
		|
;

to_downto: TO {
			geraCodigo(NULL, "CRCT 1");
			geraCodigo(NULL, "SUBT");
			sprintf(str, "ARMZ %d,%d", auxNiv, auxDes); 
			geraCodigo(NULL, str);
			empilhaPdR(pr, rot); 
			sprintf(str, "ROT%d", rot);
			geraCodigo(str, "NADA");
			rot++;
			sprintf(str, "CRVL %d,%d", auxNiv, auxDes); 
			geraCodigo(NULL, str);
			geraCodigo(NULL, "CRCT 1");
			geraCodigo(NULL, "SOMA");
			sprintf(str, "ARMZ %d,%d", auxNiv, auxDes);
			geraCodigo(NULL, str);
			sprintf(str, "CRVL %d,%d", auxNiv, auxDes); 
			geraCodigo(NULL, str);
		} arit {
			geraCodigo(NULL, "CMME");	
		}
		| DOWNTO {
			geraCodigo(NULL, "CRCT 1");
			geraCodigo(NULL, "SOMA");
			sprintf(str, "ARMZ %d,%d", auxNiv, auxDes); 
			geraCodigo(NULL, str);
			empilhaPdR(pr, rot); 
			sprintf(str, "ROT%d", rot);
			geraCodigo(str, "NADA");
			rot++;
			sprintf(str, "CRVL %d,%d", auxNiv, auxDes); 
			geraCodigo(NULL, str);
			geraCodigo(NULL, "CRCT 1");
			geraCodigo(NULL, "SUBT");
			sprintf(str, "ARMZ %d,%d", auxNiv, auxDes);
			geraCodigo(NULL, str);
			sprintf(str, "CRVL %d,%d", auxNiv, auxDes); 
			geraCodigo(NULL, str);
		} arit {
			geraCodigo(NULL, "CMMA");	
		}
;


cond_if: if_then cond_else {
			sprintf(str, "ROT%d", desempilhaPdR(pr));
			geraCodigo(str, "NADA");
		}
;

if_then: IF expr {
			if( desempilhaPdT(pt) != Bool) {
				sprintf(str, "Operacao nao permitida: tipo incompativel");
				imprimeErro(str);
			}
			empilhaPdR(pr, rot);
			sprintf(str, "DSVF ROT%d", rot);
			geraCodigo(NULL ,str);
			rot++;
		} THEN subcomandos {
			int aux = desempilhaPdR(pr);
			empilhaPdR(pr, rot);
			sprintf(str, "DSVS ROT%d", rot);
			geraCodigo(NULL ,str);
			rot++;
			sprintf(str, "ROT%d", aux);
			geraCodigo(str, "NADA");
		}
;

cond_else: ELSE subcomandos
		| %prec LOWER_THAN_ELSE
;

atribuicao_ou_CP: IDENT {
			strcpy(id, token);
			elemento = buscaTs(ts, nivel, id);
			if(elemento == NULL) {
				sprintf(str,"Simbolo não encontrado: %s", id);
				imprimeErro(str);
			}
			if(elemento->categoria != Proc && elemento->categoria != RT)
				empilhaPdT(pt, elemento->tipo);
			empilhaPdF(pfunc, -1, elemento);
		 } atribuicao_ou_CP2
;

atribuicao_ou_CP2: atribuicao
			| goto_dest
			| chama_proc
;


goto: GOTO IDENT {
		elemento = buscaTs(ts, nivel, token);
		if(elemento == NULL) {
			sprintf(str,"Simbolo não encontrado: %s", token);
			imprimeErro(str);
		}
		sprintf(str,"DSVR %s, %d, %d", elemento->id, elemento->nivel, nivel);
		geraCodigo(NULL, str);
		
	}
	| GOTO NUMERO {
		elemento = buscaTs(ts, nivel, token);
		if(elemento == NULL) {
			sprintf(str,"Simbolo não encontrado: %s", token);
			imprimeErro(str);
		}
		sprintf(str,"DSVR %s, %d, %d", elemento->id, elemento->nivel, nivel);
		geraCodigo(NULL, str);
	}
;

goto_dest: DOIS_PONTOS  {
		if(pfunc->topo != pfunc->elementos - 1 ){
			desempilhaPdF(pfunc);
		}
		if(elemento->deslocamento == 0){
			sprintf(str,"rotulo ja declarado: %s", elemento->id);
			imprimeErro(str);
		}
		elemento->deslocamento++;
		sprintf(str, "ENRT %d, %d", nivel, numVar(ts, nivel));	
		geraCodigo(elemento->id, str);
		elemento = NULL;
	} comando
;

chama_proc: params {
			EPdF *func = desempilhaPdF(pfunc);
				
			if( func->func->categoria == Func || func->func->categoria == Proc ){
				if(func->func->categoria == Func) {
					geraCodigo(NULL, "AMEM 1");
				}
				sprintf(str, "CHPR ROT%d, %d", func->func->deslocamento, nivel);
				geraCodigo(NULL, str);
				
				if(func->ind_param + 1 != func->func->num_param){
				sprintf(str, "Numero de parametros invalidos para função %s", func->func->id);
				imprimeErro(str);
			}
		}
	}
;			

atribuicao: ATRIBUICAO {
				if(pfunc->topo != pfunc->elementos - 1 )
					desempilhaPdF(pfunc);
	} expr {
			int deslocamento = elemento->deslocamento;
			if( elemento->categoria == Proc ){
				sprintf(str,"Procedimento nao possui retorno");
				imprimeErro(str);
			}	
			if(elemento->categoria == Func){
				if(!verificaRetorno(ts, elemento, nivel)) {
					sprintf(str,"Não eh possivel fazer o retorno da funcao %s nesse local", elemento->id);
					imprimeErro(str);
				}
			}
			if( elemento->tipo > Bool )
				sprintf(str, "ARMI %d,%d", elemento->nivel, elemento->deslocamento);
			else if( elemento->categoria == Func ){
				sprintf(str, "ARMZ %d,%d", elemento->nivel, -4 - elemento->num_param );
				deslocamento = -4 - elemento->num_param;
			} else
		 		sprintf(str, "ARMZ %d,%d", elemento->nivel, elemento->deslocamento); 
	 		verificaTipo(pt, str);
	 		auxNiv = elemento->nivel;
			auxDes = deslocamento;
	 		elemento = NULL;		
	} 
;



/*-------------------------------expressões---------------------------------*/
expr: expr AND comp {verificaTipo(pt, "CONJ"); empilhaPdT(pt, Bool);}
	| expr OR comp {verificaTipo(pt, "DISJ"); empilhaPdT(pt, Bool);}
	| NOT comp {
		if( Bool != desempilhaPdT(pt)){
			sprintf(str,"Operação não permitida: tipos incompatíveis");
			imprimeErro(str);
		}
		geraCodigo(NULL, "NEGA");
		empilhaPdT(pt, Bool);
	}
    | comp
;

comp: comp IGUAL termo {verificaTipo(pt, "CMIG"); empilhaPdT(pt, Bool);}
	| comp DIFERENTE termo {verificaTipo(pt, "CMDG"); empilhaPdT(pt, Bool);}
	| comp MAIOR termo {verificaTipo(pt, "CMMA"); empilhaPdT(pt, Bool);}
	| comp MENOR termo {verificaTipo(pt, "CMME"); empilhaPdT(pt, Bool);}
	| comp MAIOR_OU_IGUAL termo {verificaTipo(pt, "CMAG"); empilhaPdT(pt, Bool);}
	| comp MENOR_OU_IGUAL termo {verificaTipo(pt, "CMEG"); empilhaPdT(pt, Bool);}
	| arit
;

arit: arit MAIS termo {verificaTipo(pt, "SOMA"); empilhaPdT(pt, Int);} 
    | arit MENOS termo {verificaTipo(pt, "SUBT"); empilhaPdT(pt, Int);}
    | termo
;

termo: termo MUL fator {verificaTipo(pt, "MULT"); empilhaPdT(pt, Int);}
	 | termo DIV fator {verificaTipo(pt, "DIVI"); empilhaPdT(pt, Int);}
	 | termo DIV2 fator {verificaTipo(pt, "DIVI"); empilhaPdT(pt, Int);}
     | fator
;

fator: TRUE {
		empilhaPdT (pt, Bool);
		sprintf(str, "CRCT 1");
		geraCodigo(NULL, str);
	 }
	 | FALSE {
		empilhaPdT (pt, Bool);
		sprintf(str, "CRCT 0");
		geraCodigo(NULL, str);
	 }
	 | NUMERO {
		empilhaPdT (pt, Int);
		sprintf(str, "CRCT %s", token);
		geraCodigo(NULL, str);
	 } 
	 | IDENT {
	 	int tipo;
  		elemento2 = buscaTs(ts, nivel, token);
  		if(elemento2 == NULL) {
			sprintf(str,"Simbolo nao encontrado: %s", token);
			imprimeErro(str);
		}
		
		if(elemento2->categoria == Proc){
			sprintf(str,"Procedimento nao pode fazer parte de uma expressao");
			imprimeErro(str);
		}
		if(elemento2->categoria != Func ){
			if(elemento2->tipo > Bool) {
				 if((pfunc->topo != pfunc->elementos - 1) && pfunc->topo->func->params[pfunc->topo->ind_param] > Bool){
					sprintf(str, "CRVL %d,%d", elemento2->nivel, elemento2->deslocamento);
					tipo = elemento2->tipo;
				} else {
					sprintf(str, "CRVI %d,%d", elemento2->nivel, elemento2->deslocamento);
					tipo = elemento2->tipo - 5;
				}
			} else if(pfunc->topo != pfunc->elementos - 1 && pfunc->topo->func->params[pfunc->topo->ind_param] > Bool){
					sprintf(str, "CREN %d,%d", elemento2->nivel, elemento2->deslocamento);
					tipo = elemento2->tipo + 5;
				} else {
					sprintf(str, "CRVL %d,%d", elemento2->nivel, elemento2->deslocamento);
					tipo = elemento2->tipo;
				}
	
			geraCodigo(NULL, str);
		}else {
			tipo = elemento2->tipo;
		}
		empilhaPdF(pfunc, -1, elemento2);
		empilhaPdT (pt, tipo);
	 } chama_proc
	 | ABRE_PARENTESES expr FECHA_PARENTESES   
;

/*--------------------------------io---------------------------------*/
io: WRITE ABRE_PARENTESES write_vars FECHA_PARENTESES 
	| READ ABRE_PARENTESES read_var FECHA_PARENTESES  
;

write_vars: write_vars VIRGULA expr {geraCodigo(NULL, "IMPR");}
	| expr {geraCodigo(NULL, "IMPR");}
;

read_var: read_var VIRGULA IDENT {
		strcpy(id,token);
		elemento = buscaTs(ts, nivel, id);
		if(elemento == NULL) {
			sprintf(str,"Simbolo não encontrado: %s", id);
			imprimeErro(str);
		}
		geraCodigo(NULL, "LEIT");
		sprintf(str, "ARMZ %d,%d", elemento->nivel, elemento->deslocamento);
		geraCodigo(NULL, str);
		elemento = NULL;
	}
	| IDENT {
		strcpy(id,token);
		elemento = buscaTs(ts, nivel, id);
		if(elemento == NULL) {
			sprintf(str,"Simbolo não encontrado: %s", id);
			imprimeErro(str);
		}
		geraCodigo(NULL, "LEIT");
		sprintf(str, "ARMZ %d,%d", elemento->nivel, elemento->deslocamento);
		geraCodigo(NULL, str);
		elemento = NULL;
	}
;
/*========================================comandos========================================*/
%%

main (int argc, char** argv) {
   FILE* fp;
   extern FILE* yyin;

   if (argc<2 || argc>2) {
         printf("usage compilador <arq>a %d\n", argc);
         return(-1);
      }

   fp=fopen (argv[1], "r");
   if (fp == NULL) {
      printf("usage compilador <arq>b\n");
      return(-1);
   }

/* -------------------------------------------------------------------
 *  Inicia a Tabela de Símbolos
 * ------------------------------------------------------------------- */

   yyin=fp;
   yyparse();

   return 0;
}

/* 
f->terminar for
c->tipos
c->goto
f->procedures
--------------------
fm->funçoes -> func como param
obs: func inside func its a problem
Testar se funciona corretamente o empilhamento de parâmetros
passados por valor ou por referência.
*/

%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"

int num_vars,i,aux;
tabS tbs;	
elementoTS e,pc;
elementoTS *e2,*p,*proc_atual;
p_rot rotulos;
p_tipo ptipos;
p_proc_func p_pf;
char id_var[256];
char id_atr[256];
char id_atr_for[256];
char str[15], str2[15];
char imp_goto[15];
char num[15];
char inicio[15];
int i,pos,rot1=-1,rot2;
int tipo = 0;
char nome_tipo[10];
int nl_atual = 0;
int nvars = 0;
int proc = 0;
int func = 0;
int ref = 0;
int insideFunction = 0;

%}
%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES 
%token PROCEDURE FUNCTION
%token VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO
%token T_BEGIN T_END VAR IDENT ATRIBUICAO
%token SOMA SUBTRAI DIVIDE MULTIPLICA
%token MENOR MENOR_IGUAL MAIOR MAIOR_IGUAL IGUAL DIFERENTE OU E
%token WHILE DO
%token REPEAT UNTIL
%token FOR TO DOWNTO
%token IF THEN ELSE
%token NUMERO
%token TRUE FALSE 
%token WRITE READ
%token GOTO LABEL

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

programa:
		{ 
			geraCodigo (NULL, "INPP"); 
			//e.nivel = 0;
			iniciaTS(&tbs,8);
			iniciaPilhaDeRotulos(&rotulos,&rot1,&rot2,10);
			iniciaPilhaDeTipos(&ptipos,10);
			iniciaPilhaDeProcFunc(&p_pf, 10);
			
		}
		PROGRAM IDENT 
		{
			/* Insere Programa Principal na Tabela de Simbolos */
			strcpy(e.id,token);
			e.cat = programaPrincipal; 
			e.nivel = nl_atual;
			e.nparam = 0;

			if(empilhaElementoTS(&tbs,e) == -1){
				printf("Error: redeclaration of %s\n",e.id);
				exit(1);
			}	
		}
		ABRE_PARENTESES lista_idents_ini FECHA_PARENTESES PONTO_E_VIRGULA
		bloco PONTO 
		{
			/* DMEM */
			num_vars = desempilhaPPTS(&tbs);
			if(num_vars){
				sprintf(str,"DMEM %d",num_vars);
				geraCodigo(NULL,str);
			}
			free(tbs.eTS); 		
			geraCodigo (NULL, "PARA"); 
		}
;

bloco: 
		{
		//imprimeTS(tbs);
		}
		declara_labels
		parte_declara_vars
		{ 
			geraProxRotulo(&rot1);
			if(rot1 < 10)
				sprintf(str,"DSVS R0%d",rot1);
			else	
				sprintf(str,"DSVS R%d",rot1);
			geraCodigo(NULL,str);
			empilhaRotulo(&rotulos,rot1);
		} 
		declara_proc_func
		{
			/* Rótulo Main R0 */
			i = desempilhaRotulo(&rotulos);
			if(i < 10)
				sprintf(str,"R0%d",i);
			else	
				sprintf(str,"R%d",i);
			geraCodigo(str,"NADA");
		}
		T_BEGIN 
		comandos
		T_END
		//comando_composto
;


declara_labels : declara_label
                     |
;
declara_label : declara_label label
                    | label
;

label   : LABEL lista_id_labels PONTO_E_VIRGULA
;

lista_id_labels  : lista_id_labels VIRGULA id_label
                 | id_label
;

id_label: IDENT insere_label | NUMERO insere_label 
	
;

insere_label:
			{
				geraProxRotulo(&rot1);
				strcpy(e.id,token);
				e.cat = rotulo;
				e.nivel = nl_atual;
				e.rotulo = rot1;
				printf("GOTO %s:::ROTULO: %d ",e.id,e.rotulo);
				empilhaElementoTS(&tbs,e);
			}
;

declara_proc_func: 
	procs_funcs |
;

procs_funcs: 
	procs_funcs pc_fc | pc_fc 

;

pc_fc:
	funcao | procedimento
;

funcao: 
		FUNCTION IDENT
		{
			func = insideFunction = 1;
			aux = 0;
			nl_atual++;
			geraProxRotulo(&rot1);
			if(rot1 < 10)
				sprintf(str,"R0%d",rot1);
			else	
				sprintf(str,"R%d",rot1);
			sprintf(str2,"ENPR %d",nl_atual);
			geraCodigo(str,str2);
			empilhaRotulo(&rotulos,rot1);
			
			strcpy(pc.id,token);
			pc.cat = funcao;
			pc.nivel = nl_atual;
			pc.rotulo = rot1;
			
			for(i = 0; i < 10; i++)
				pc.vParam[i].tipo = e.vParam[i].pfTipoPassagem = 0;
			pc.nparam = 0;

			if(empilhaElementoTS(&tbs,pc) == -1){
				printf("Error: redeclaration of %s\n",e.id);
				exit(1);
			}	
			
		} 
		parametros
		DOIS_PONTOS retorno_func 
		{
			
			num_vars = func = 0;
			tbs.eTS[buscaID_TS(tbs,pc.id)].tipo = pc.tipo;
			empilhaProcFunc(&p_pf,pc);
			imprimeTS(tbs);
		
		}
		PONTO_E_VIRGULA bloco PONTO_E_VIRGULA 
		{ 
			desempilhaProcFunc(&p_pf);
			/* Desempilha váriaveis simples da Tabela de Símbolos & DMEM */
			aux = desempilhaProcFuncTS(&tbs,nl_atual);
			if(aux){
				sprintf(str,"DMEM %d",aux);
				geraCodigo(NULL,str);
			}
			
			sprintf(str,"RTPR %d,%d",nl_atual,buscaParamProcFunc(tbs,desempilhaRotulo(&rotulos)));
			geraCodigo(NULL,str);
			nl_atual--;
			insideFunction = 0;
			
		} 
;

retorno_func: 
		IDENT
		{
			strcpy(nome_tipo,token);
		    	
			if(!strcmp(nome_tipo,"integer"))
				pc.tipo = Inteiro;
			else if(!strcmp(nome_tipo,"boolean"))
				pc.tipo = Boolean;
			else if(!strcmp(nome_tipo,"real"))
				pc.tipo = Real;
		}	
	
;

procedimento: 
		PROCEDURE IDENT
		{
			proc = 1;
			aux = 0;
			nl_atual++;
			geraProxRotulo(&rot1);
			if(rot1 < 10)
				sprintf(str,"R0%d",rot1);
			else	
				sprintf(str,"R%d",rot1);
			sprintf(str2,"ENPR %d",nl_atual);
			geraCodigo(str,str2);
			empilhaRotulo(&rotulos,rot1);
			
			strcpy(pc.id,token);
			pc.cat = procedimento; // Procedimento
			pc.nivel = nl_atual;
			pc.rotulo = rot1;
			
			
			for(i = 0; i < 10; i++)
				pc.vParam[i].tipo = e.vParam[i].pfTipoPassagem = 0;
			pc.nparam = 0;

			if(empilhaElementoTS(&tbs,pc) == -1){
				printf("Error: redeclaration of %s\n",e.id);
				exit(1);
			}	
			
		} 
		parametros
		{
			num_vars = proc = 0;
			empilhaProcFunc(&p_pf,pc);
		}
		PONTO_E_VIRGULA bloco PONTO_E_VIRGULA 
		{ 
			desempilhaProcFunc(&p_pf);
			/* Desempilha váriaveis simples da Tabela de Símbolos & DMEM */
			aux = desempilhaProcFuncTS(&tbs, nl_atual);
			if(aux){
				sprintf(str,"DMEM %d",aux);
				geraCodigo(NULL,str);
			}
			
			sprintf(str,"RTPR %d,%d",nl_atual,buscaParamProcFunc(tbs,desempilhaRotulo(&rotulos)));
			geraCodigo(NULL,str);
			nl_atual--;
			
		} 
;

parametros: 
		ABRE_PARENTESES lista_idents_param_prot FECHA_PARENTESES 
		{ 
			/*Atribui deslocamento na variaveis do Proc ou Func na TS*/
			atribuiDTS(&tbs);
		} | 
;

lista_idents_param_prot: 
		lista_idents_param_prot PONTO_E_VIRGULA lista_idents_param_v_r | 				
		lista_idents_param_v_r
;

lista_idents_param_v_r: 
		{
			ref=0;
		}
		lista_idents_param_v | 
		VAR 
		{
			ref=1;// flag para saber se é por referencia;
		} lista_idents_param_v
;

lista_idents_param_v: 
		{
			aux = num_vars;
		}
		lista_idents 
		{
			
			/*Atualiza Procedimento com a informação de Parametros correta*/
			insereParametrosTS(&tbs,num_vars); 
		}
		DOIS_PONTOS tipo 
		{
			/* Atribui  modo(v/f) & deslocamento nos paramentros do Procedimento */
			printf("IMPRIME PF\n");
			imprimeTS(tbs);
		}| 
; 

lista_idents: 
		lista_idents VIRGULA 
		IDENT
		{
			/* Empilha variável como parametro formal */
			strcpy(e.id,token);
			e.cat = parametroFormal;
			e.nivel = nl_atual;
			empilhaElementoTS(&tbs,e);
			num_vars++;	
		}  
		| IDENT 
		{
			strcpy(e.id,token);
			e.cat = parametroFormal;
			e.nivel = nl_atual;
			empilhaElementoTS(&tbs,e);
			num_vars++;	
		}
;

lista_idents_param: 
		lista_idents_p | 
; 

lista_idents_p: 
		lista_idents_p VIRGULA 
		expr_logica
		{
			printf("2-C\n");
			i=desempilhaTipo(&ptipos);
			printf("2-COMP TIPOS: i %d,(*proc_atual).vParam[nvars].tipo %d, nvars %d\n",i,(*proc_atual).vParam[nvars].tipo,nvars);
			if(i != (*proc_atual).vParam[nvars].tipo){
				imprimeErro("Incorrect types.");
			}
			nvars++;
		 
		}  
		| expr_logica
		{
			printf("3-C\n");
			printf("1-COMP TIPOS: i %d,(*proc_atual).vParam[nvars].tipo %d, nvars %d\n",i,(*proc_atual).vParam[nvars].tipo,nvars);
			if(desempilhaTipo(&ptipos) != (*proc_atual).vParam[nvars].tipo){
				imprimeErro("Incorrect types.");
			}
			nvars++;
		} 

;

lista_idents_ini: 
		lista_ident_ini | 
; 

lista_ident_ini: 
		lista_ident_ini VIRGULA 
		IDENT | IDENT 
;



parte_declara_vars:
		var 
;


var: 
		VAR declara_vars {num_vars = 0;}
		|
;

declara_vars:
		declara_vars declara_var 
		| declara_var 
;

declara_var:
		{
			aux = num_vars;
			ref = 0;
		} 
		lista_id_var DOIS_PONTOS
		tipo
		{ 
			sprintf(str,"AMEM %d",num_vars - aux);
			geraCodigo(NULL,str);
			
		}
		PONTO_E_VIRGULA
;

tipo:
		IDENT
		{
		    strcpy(nome_tipo,token);
				
			if(!strcmp(nome_tipo,"integer"))
				tipo = Inteiro;
			else if(!strcmp(nome_tipo,"boolean"))
				tipo = Boolean;
			else if(!strcmp(nome_tipo,"real"))
				tipo = Real;
			
			
			if(proc || func){
				insereTipo(&tbs,tipo,ref,num_vars-aux);
				proc_atual = (elementoTS *) atualizaProc(&tbs,num_vars-aux,tipo,ref);
				printf("------ %s (*proc_atual)vParam[0].pfTipoPasssagem %d\n",(*proc_atual).id,(*proc_atual).vParam[0].pfTipoPassagem);
			} else 
				insereTipo(&tbs,tipo,valor,num_vars-aux);
				
		}
;

lista_id_var	:
				lista_id_var VIRGULA 
				IDENT 
				{ 
					
					strcpy(e.id,token);
					e.cat = variavelSimples;
					e.nivel = nl_atual;
					e.deslocamento = num_vars;        
					num_vars++;
					if(empilhaElementoTS(&tbs,e) == -1){
						printf("Error: redeclaration of %s\n",e.id);
						exit(1);
					}
				}
				| IDENT 
				{ 
					
					
					strcpy(e.id,token);
					e.cat = variavelSimples;
					e.nivel = nl_atual;
					e.deslocamento = num_vars;
					num_vars++;

					if(empilhaElementoTS(&tbs,e) == -1){
						printf("Error: redeclaration of %s\n",e.id);
						exit(1);
					}	
				}
;


comando_composto:
		T_BEGIN comandos T_END | comando
;

comando:
		id_atribuicao_proc | comando_while | comando_if | comando_repeat 
		| 	comando_for | write | read | goto | rot |
;


comandos:
		comandos PONTO_E_VIRGULA comando
		| comando
;


expressao: 
		expr_logica
;
expr_logica:
		expr_logica E termo_logico 
		{
			if((desempilhaTipo(&ptipos) != Boolean) || (desempilhaTipo(&ptipos) != Boolean)){
				imprimeErro("Incompatible types.");
			}
			empilhaTipo(&ptipos,Boolean);
			
			geraCodigo(NULL,"CONJ");
		} 
		| expr_logica OU termo_logico
		{
			if((desempilhaTipo(&ptipos) != Boolean) || (desempilhaTipo(&ptipos) != Boolean)){
				imprimeErro("Incompatible types.");
			}
			empilhaTipo(&ptipos,Boolean);
			
			geraCodigo(NULL,"DISJ");
		}
		| termo_logico
;

termo_logico:
		termo_logico IGUAL expr 
		{
			if((desempilhaTipo(&ptipos) != Inteiro) || (desempilhaTipo(&ptipos) != Inteiro)){
				imprimeErro("Incompatible types.");
			}
			empilhaTipo(&ptipos,Boolean);
			
			geraCodigo(NULL,"CMIG");
		}
		| termo_logico DIFERENTE expr 
		{
			if((desempilhaTipo(&ptipos) != Inteiro) || (desempilhaTipo(&ptipos) != Inteiro)){
				imprimeErro("Incompatible types.");
			}
			empilhaTipo(&ptipos,Boolean);
			
			geraCodigo(NULL,"CMDG");
		}	
		| termo_logico MENOR_IGUAL expr 
		{
			if((desempilhaTipo(&ptipos) != Inteiro) || (desempilhaTipo(&ptipos) != Inteiro)){
				imprimeErro("Incompatible types.");
			}
			empilhaTipo(&ptipos,Boolean);
			
			geraCodigo(NULL,"CMEG");
		} 
		| termo_logico MAIOR_IGUAL expr 
		{
			if((desempilhaTipo(&ptipos) != Inteiro) || (desempilhaTipo(&ptipos) != Inteiro)){
				imprimeErro("Incompatible types.");
			}
			empilhaTipo(&ptipos,Boolean);
			
			geraCodigo(NULL,"CMAG");
		}
		| termo_logico MENOR expr 
		{
			if((desempilhaTipo(&ptipos) != Inteiro) || (desempilhaTipo(&ptipos) != Inteiro)){
				imprimeErro("Incompatible types.");
			}
			empilhaTipo(&ptipos,Boolean);
			
			geraCodigo(NULL,"CMME");
		}
		| termo_logico MAIOR expr 
		{
			if((desempilhaTipo(&ptipos) != Inteiro) || (desempilhaTipo(&ptipos) != Inteiro)){
				imprimeErro("Incompatible types.");
			}
			empilhaTipo(&ptipos,Boolean);
			
			geraCodigo(NULL,"CMMA");
		}
		| expr
;


expr:	
		expr SOMA termo 
		{
			if((desempilhaTipo(&ptipos) != Inteiro) || (desempilhaTipo(&ptipos) != Inteiro)){
				imprimeErro("Incompatible types.");
			}
			empilhaTipo(&ptipos,Inteiro);
			
			geraCodigo(NULL,"SOMA");
		}
		| expr SUBTRAI termo 
	 	{
	 		if((desempilhaTipo(&ptipos) != Inteiro) || (desempilhaTipo(&ptipos) != Inteiro)){
				imprimeErro("Incompatible types.");
			}
			empilhaTipo(&ptipos,Inteiro);
			
	 		geraCodigo(NULL,"SUBT");
	 	}
	 	| termo
;

termo:
		termo MULTIPLICA fator 
		{
			if((desempilhaTipo(&ptipos) != Inteiro) || (desempilhaTipo(&ptipos) != Inteiro)){
				imprimeErro("Incompatible types.");
			}
			empilhaTipo(&ptipos,Inteiro);
			
			geraCodigo(NULL,"MULT");
			
			
		} 
		| termo DIVIDE fator 
		{
			if((desempilhaTipo(&ptipos) != Inteiro) || (desempilhaTipo(&ptipos) != Inteiro)){
				imprimeErro("Incompatible types.");
			}
			empilhaTipo(&ptipos,Inteiro);
			
			geraCodigo(NULL,"DIVI");
		}
		| fator
;

fator:
		IDENT	
		{	
			pos = buscaID_TS(tbs, token);
			imprimeTS(tbs);
			if(pos == -1){
			 	printf("undeclared identifier %s' (first use in this routine)\n",token);
			 	exit(1);
			}
			
		} func_var
		| NUMERO 
		{
			strcpy(id_var,"CRCT ");
			strcat(id_var,token);
			geraCodigo(NULL,id_var);
			empilhaTipo(&ptipos,Inteiro);
		} 
		|  ABRE_PARENTESES expressao FECHA_PARENTESES | 
		TRUE 
		{
			geraCodigo(NULL,"CRCT 1");
			empilhaTipo(&ptipos,Boolean);
		}| 
		FALSE
		{
			geraCodigo(NULL,"CRCT 0");
			empilhaTipo(&ptipos,Boolean);
		}
		
		
		
;

func_var:  
	ABRE_PARENTESES 
	{
		geraCodigo(NULL,"AMEM 1");
		empilhaTipo(&ptipos,tbs.eTS[pos].tipo);
		if(tbs.eTS[pos].cat != funcao)
			imprimeErro("Illegal expression.");
	}
	inicio_param_proc 
	lista_idents_param 
	fim_param_proc 
	FECHA_PARENTESES 

	|{
		if(tbs.eTS[pos].cat == procedimento)
				imprimeErro("Ilegal expression.");

				
			//printf("&&&&&&&###############: %s (*proc_atual).vParam[%d].pfTipoPassagem %d, referencia %d, tbs.eTS[pos].id %s,tbs.eTS[pos].pfTipoPassagem %d, tbs.eTS[pos].nivel %d,tbs.eTS[pos].deslocamento %d \n",(*proc_atual).id,nvars,(*proc_atual).vParam[nvars].pfTipoPassagem,referencia, tbs.eTS[pos].id,tbs.eTS[pos].pfTipoPassagem,tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);

			
			sprintf(id_var,"CRVL %d, %d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);	
			/*if função ...*/
			
			if(tbs.eTS[pos].cat == variavelSimples){
				 /*Se está em uma chamada de procedimento */
				if(ref)
					if((*proc_atual).vParam[nvars].pfTipoPassagem == referencia)
						sprintf(id_var,"CREN %d,%d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
						
					
				
			}
			else if(tbs.eTS[pos].cat == parametroFormal){
				if(ref){
					/* Verifica como parametro é chamado no Proc*/
					if((*proc_atual).vParam[nvars].pfTipoPassagem == referencia){
						/* Verifica se dentro deste Proc, variavel foi passada por valor*/
						if(tbs.eTS[pos].pfTipoPassagem == valor)
							sprintf(id_var,"CREN %d,%d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
						} else if (tbs.eTS[pos].pfTipoPassagem == referencia)
							sprintf(id_var,"CRVI %d,%d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);	
					} else if(tbs.eTS[pos].pfTipoPassagem == referencia)
							sprintf(id_var,"CRVI %d,%d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
								
			}	
			geraCodigo(NULL,id_var);
			empilhaTipo(&ptipos,tbs.eTS[pos].tipo);
	}
;

write: WRITE ABRE_PARENTESES in_write FECHA_PARENTESES
;

in_write: in_write VIRGULA impr
          | impr
;

impr: expr_logica
		{
			desempilhaTipo(&ptipos);
			geraCodigo(NULL, "IMPR");
		}
; 

read: READ ABRE_PARENTESES in_read FECHA_PARENTESES
;

in_read: in_read VIRGULA leit
          | leit
;

leit: IDENT
      {
        geraCodigo(NULL, "LEIT");
        if(buscaID_TS(tbs, token) == -1)
        {
	     	printf("undeclared identifier '%s' (first use in this routine)\n",token);
	     	exit(1);
        }
        if (tbs.eTS[buscaID_TS(tbs, token)].tipo == Boolean){ 
            printf("Can't read variables of this type \n");
            exit(1);
        }
         sprintf(str, "ARMZ %d, %d", tbs.eTS[buscaID_TS(tbs, token)].nivel, tbs.eTS[buscaID_TS(tbs, token)].deslocamento);
         geraCodigo(NULL, str);
      }
; 

goto: 
		GOTO IDENT vai_para
		| GOTO NUMERO vai_para
;

vai_para:
		{
			if(tbs.eTS[buscaID_TS(tbs, token)].cat != rotulo)
				imprimeErro("Illegal expression \n");
			if(tbs.eTS[buscaID_TS(tbs, token)].rotulo < 10)
				sprintf(imp_goto, "DSVR R0%d, %d, %d", tbs.eTS[buscaID_TS(tbs, token)].rotulo, tbs.eTS[buscaID_TS(tbs, token)].nivel, nl_atual);
			else
				sprintf(imp_goto, "DSVR R%d, %d, %d", tbs.eTS[buscaID_TS(tbs, token)].rotulo, tbs.eTS[buscaID_TS(tbs, token)].nivel, nl_atual);
			geraCodigo(NULL, imp_goto);

		}
;
rot : 
		NUMERO
		{       
		strcpy(id_atr, token); 
		}
		goto_dest
;

goto_dest   : 
		DOIS_PONTOS
		{
			e2 = &tbs.eTS[buscaID_TS(tbs, id_atr)];
			if ((*e2).nivel != nl_atual)
				imprimeErro("Illegal expression");
			sprintf(inicio, "R%d", (*e2).rotulo);
			if((*e2).rotulo < 10)
					sprintf(inicio, "R0%d",(*e2).rotulo);
			else		
				sprintf(inicio, "R%d",(*e2).rotulo);
			printf("Go ENRT\n");	
			imprimeTS(tbs);
			sprintf(str, "ENRT %d, %d", (*e2).nivel, numVarLocais(tbs,nl_atual));
			geraCodigo(inicio, str);
		}
		comando
;
//termina goto
id_atribuicao_proc: 
		IDENT 
		{
			strcpy(id_atr,token);
			printf(" atr: %s\n",id_atr);
			
			/*Verifica se Procedimento/Variável existe na TS*/
			e2 = (elementoTS *) buscaElementoTS(tbs,id_atr);
			if((*e2).cat == programaPrincipal){
				imprimeErro ( "undeclared variable" );
			}		
		}	
		atribuicao_proc
;

atribuicao_proc:	
		ATRIBUICAO expressao	
		{	
			pos = buscaID_TS(tbs, id_atr);
			if(pos == -1){
			 	printf("undeclared identifier %s' (first use in this routine)\n",id_atr);
			 	exit(1);
			}
			
			if(desempilhaTipo(&ptipos) != tbs.eTS[pos].tipo){
				imprimeErro("Incompatible types.");
			}
			printf("ATR: %s n: %d, d:%d\n",tbs.eTS[pos].id,tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
			sprintf(id_var,"ARMZ %d, %d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
			if(tbs.eTS[pos].cat == parametroFormal){
				if(tbs.eTS[pos].pfTipoPassagem == referencia)
					sprintf(id_var,"ARMI %d, %d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
			
			} else if(tbs.eTS[pos].cat == funcao) {
				//Se está dentro da função
				printf("Var Func %s t: %d,ref %d\n",tbs.eTS[pos].id,tbs.eTS[pos].tipo, ref);
				if(insideFunction){
					if(!verificaAtrFunc(tbs,tbs.eTS[pos].id, nl_atual))
						imprimeErro("->Illegal expression");
				}else 
					imprimeErro("-+Illegal expression");  
				
			}else if((tbs.eTS[pos].cat == procedimento) || (tbs.eTS[pos].cat == rotulo) )
					imprimeErro("Illegal expression");
			
			geraCodigo(NULL,id_var);
			//imprimeTS(tbs,i); 
		} 
		| inicio_param_proc  /* Procedimento */ 
		{
			/* Procedimento sem Parametro, verifica se existe na TS, verifica 				se qtd de parametros é 0, e chpr */
			
			if((*e2).nparam){
				printf("incorrect parameters\n");
			 	exit(1);
			}		
		} fim_param_proc
		|  ABRE_PARENTESES 
		inicio_param_proc 
		lista_idents_param 
		fim_param_proc 
		FECHA_PARENTESES 
		| goto_dest
;

inicio_param_proc: 
		{	
			//p = (elementoTS *) desempilhaProcFunc(&p_pf);
			ref++;
		}

;

fim_param_proc: 
		{
			printf("_+_+_+_+_+_+_=--=-=-=-=-=CHAMNDO PROC/FUNC %s\n",(*proc_atual).id);
			if((*proc_atual).rotulo < 10)
				sprintf(str,"CHPR R0%d,%d",(*proc_atual).rotulo, nl_atual);
			else
				sprintf(str,"CHPR R1%d,%d",(*proc_atual).rotulo, nl_atual);
			geraCodigo(NULL,str);
			ref--;
			
			nvars = 0;
		}

;

comando_while:
		WHILE 
		{ 
			geraProxRotulo(&rot1);
			if(rot1 < 10)
				sprintf(str,"R0%d",rot1);
			else
				sprintf(str,"R%d",rot1);
			geraCodigo(str,"NADA");
			empilhaRotulo(&rotulos,rot1);
		} 
		expr_logica 
		{
			geraProxRotulo(&rot2);
			if(rot2 < 10)
				sprintf(str,"DSVF R0%d",rot2);
			else
				sprintf(str,"DSVF R%d",rot2);
			geraCodigo(NULL,str);
			empilhaRotulo(&rotulos,rot2);
		}
		DO comando_composto
		{
			rot2 = desempilhaRotulo(&rotulos);
			rot1 = desempilhaRotulo(&rotulos);
			if(rot1 < 10)
				sprintf(str,"DSVS R0%d",rot1);
			else
				sprintf(str,"DSVS R%d",rot1);
			geraCodigo(NULL,str);
			
			
			if(rot2 < 10)
				sprintf(str,"R0%d",rot2);
			else
				sprintf(str,"R%d",rot2);
			geraCodigo(str,"NADA");
		} 
;

comando_repeat:
		REPEAT
		{
			geraProxRotulo(&rot1);
			if(rot1 < 10)
				sprintf(str,"R0%d",rot1);
			else
				sprintf(str,"R%d",rot1);
			geraCodigo(str,"NADA");
			empilhaRotulo(&rotulos,rot1);
		}
		comandos UNTIL expr_logica
		{
			i = desempilhaRotulo(&rotulos);
			if(i < 10)
				sprintf(str,"DSVF R0%d",i);
			else
				sprintf(str,"DSVF R%d",i);
			geraCodigo(NULL,str);
		}
;

comando_for:
		FOR atribuicao_for to_downto_expr 
		{
			geraCodigo(NULL,id_atr_for);
			
			geraProxRotulo(&rot1);
			empilhaRotulo(&rotulos,rot1);
			
			if(rot1 < 10)
				sprintf(str,"DSVF R0%d",rot1);
			else
				sprintf(str,"DSVF R%d",rot1);
			geraCodigo(NULL,str);
		}
		DO comando_composto
		{
			/* A váriavel de atribuição não pode receber outra atribuição dentro do comando_composto
			   -> solução: criar uma atribuição só para o for, e uma váriavel para salvar a "varaivel de atribuição" do for, assim a cada atribuição "normal" verificar se é diferente da "varaivel 					de atribuição" do for
			sprintf(str,"DSVF R%d",desempilhaRotulo(&rotulos));
			geraCodigo(NULL,str);*/
			
			i = desempilhaRotulo(&rotulos);
			if(i < 10)
				sprintf(str2, "R0%d", i);
			else
				sprintf(str2, "R%d", i);
			
			i = desempilhaRotulo(&rotulos);
			if(i < 10)
				sprintf(str, "DSVS R0%d", i);
			else	
            	sprintf(str, "DSVS R%d", i);
            geraCodigo(NULL, str);
            geraCodigo(str2, "NADA");
		}
;

atribuicao_for:
		IDENT
		{
			strcpy(id_atr_for,token);
		}		
		ATRIBUICAO expr	
		{
			pos = buscaID_TS(tbs, id_atr_for);
			if(pos == -1){
			 	printf("undeclared identifier %s' (first use in this routine)\n",id_atr_for);
			 	exit(1);
			}
			sprintf(id_var,"ARMZ %d, %d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
			geraCodigo(NULL,id_var);
			
			// Novo valor fica no topo da pilha M
			sprintf(id_var,"CRVL %d, %d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
			geraCodigo(NULL,id_var);
			//imprimeTS(tbs,i); 
		}
;

to_downto_expr: 
		TO 
		{
			/* Para o incremento do FOR ocorrer no inicio do laço, e não no fim, facilitando a implementeção. */
			geraCodigo(NULL,"CRCT 1");
			geraCodigo(NULL,"SUBT");
			sprintf(id_var,"ARMZ %d, %d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
			geraCodigo(NULL,id_var);
			
			/*Rótulo FOR*/	
			geraProxRotulo(&rot1);
			if(rot1 < 10)
				sprintf(str,"R0%d",rot1);
			else
				sprintf(str,"R%d",rot1);
			geraCodigo(str,"NADA");
			empilhaRotulo(&rotulos,rot1);	
			
			/*Incremento do FOR*/
			sprintf(id_var,"CRVL %d, %d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
			geraCodigo(NULL,id_var);
			geraCodigo(NULL,"CRCT 1");
			geraCodigo(NULL,"SOMA");
			sprintf(id_var,"ARMZ %d, %d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
			geraCodigo(NULL,id_var);
			// Novo valor calculado fica no topo da pilha M
			sprintf(id_var,"CRVL %d, %d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
			geraCodigo(NULL,id_var);
		} 
		expr 
		{
			sprintf(id_atr_for,"CMEG");
		}
		| DOWNTO
		{
			/* Para o incremento do FOR ocorrer no inicio do laço, e não no fim, facilitando a implementeção. */
			geraCodigo(NULL,"CRCT 1");
			geraCodigo(NULL,"SOMA");
			sprintf(id_var,"ARMZ %d, %d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
			geraCodigo(NULL,id_var);
			
			/*Rótulo FOR*/	
			geraProxRotulo(&rot1);
			if(rot1 < 10)
				sprintf(str,"R0%d",rot1);
			else
				sprintf(str,"R%d",rot1);
			geraCodigo(str,"NADA");
			empilhaRotulo(&rotulos,rot1);	
			
			/*Incremento do FOR*/
			sprintf(id_var,"CRVL %d, %d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
			geraCodigo(NULL,id_var);
			geraCodigo(NULL,"CRCT 1");
			geraCodigo(NULL,"SUBT");
			sprintf(id_var,"ARMZ %d, %d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
			geraCodigo(NULL,id_var);
			// Novo valor calculado fica no topo da pilha M
			sprintf(id_var,"CRVL %d, %d",tbs.eTS[pos].nivel,tbs.eTS[pos].deslocamento);
			geraCodigo(NULL,id_var);
			
		} 
		expr
		{
			sprintf(id_atr_for,"CMAG");
		}
;

if_then:
		IF expr_logica 
		{	
		        if(desempilhaTipo(&ptipos) != Boolean)
		        {
		            printf("Expressao invalida \n");
		            exit(1);
		        }
				/* Gera desvio para sair do if, ou para entrar no else */
				geraProxRotulo(&rot1);
				if(rot1 < 10)
					sprintf(str,"DSVF R0%d",rot1);
				else
					sprintf(str,"DSVF R%d",rot1);
				geraCodigo(NULL,str);
				empilhaRotulo(&rotulos,rot1);
		} 
		THEN comando_composto
;

cond_else:
		ELSE 
		{
			/* Desvio após a execução do(s) comando(s) do if */
			geraProxRotulo(&rot2);
			if(rot2 < 10)
				sprintf(str,"DSVS R0%d",rot2);
			else
				sprintf(str,"DSVS R%d",rot2);
			geraCodigo(NULL,str);
			
			i = desempilhaRotulo(&rotulos);
			if(i < 10)
				sprintf(str,"R0%d",i);
			else
				sprintf(str,"R%d",i);
			geraCodigo(str,"NADA");
	
			empilhaRotulo(&rotulos,rot2);

		} 
		comando_composto 
		| %prec LOWER_THAN_ELSE
;

comando_if: 
		if_then cond_else
		{
			/* Rótulo para desviar ELSE */
			i = desempilhaRotulo(&rotulos);
			if(i < 10)
				sprintf(str,"R0%d",i);
			else
				sprintf(str,"R%d",i);
			geraCodigo(str,"NADA");
		}
;

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


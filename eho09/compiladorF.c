
/* -------------------------------------------------------------------
 *            Aquivo: compilador.c
 * -------------------------------------------------------------------
 *              Autor: Bruno Muller Junior
 *               Data: 08/2007
 *      Atualizado em: [15/03/2012, 08h:22m]
 *
 * -------------------------------------------------------------------
 *
 * Funções auxiliares ao compilador
 *
 * ------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"


/* -------------------------------------------------------------------
 *  variáveis globais
 * ------------------------------------------------------------------- */

FILE* fp=NULL;
void geraCodigo (char* rot, char* comando) {

  if (fp == NULL) {
    fp = fopen ("MEPA", "w");
  }

  if ( rot == NULL ) {
    fprintf(fp, "     %s\n", comando); fflush(fp);
  } else {
    fprintf(fp, "%s: %s \n", rot, comando); fflush(fp);
  }
}

int imprimeErro ( char* erro ) {
  fprintf (stderr, "Erro na linha %d - %s\n", nl, erro);
  exit(-1);
}

/*===================================TS========================================*/
TdS *inicializa( ){
	TdS *ts = (TdS *) malloc(sizeof(TdS));
	ts->tam = Init_TS;
	ts->elementos = (ETdS *) malloc(sizeof(ETdS)*Init_TS);
	ts->topo = ts->elementos - 1;
	
	return ts;
}
/*-----------------------------------------------------------------------------*/
/*-------------------------------buscaTs---------------------------------------*/
ETdS *buscaTs(TdS *ts, int nivel, char *id){
	ETdS *aux;
	
	for( aux = ts->topo; aux != ts->elementos - 1; aux--){
		if(!strcmp(aux->id, id))
			return aux;
	}
	
	return NULL;
}
/*-----------------------------------------------------------------------------*/
/*--------------------------------empilha--------------------------------------*/
int empilha(TdS *ts, int categoria, int nivel, int deslocamento, char *id){
	int auxNivel = nivel;
	ETdS *aux = buscaTs(ts, nivel, id);
	if(categoria == Func || categoria == Proc)
		auxNivel--;
	if( aux == NULL || aux->nivel != auxNivel){
		ts->topo++;
		if(ts->topo == ts->elementos + ts->tam){
			ts->tam <<= 1;
			ts->elementos = realloc(ts->elementos, ts->tam*sizeof(ETdS));
		}
		ts->topo->categoria = categoria;
		ts->topo->nivel = nivel;
		ts->topo->deslocamento = deslocamento;
		strcpy(ts->topo->id, id);
		
		return 1;
	}
	
	return 0;
}
/*-----------------------------------------------------------------------------*/
/*---------------------------------desempilha----------------------------------*/
ETdS *desempilha(TdS *ts){
	ts->topo--;
	if(ts->topo < ts->elementos)
		return NULL;
	return ts->topo+1;
}
/*---------------------------------insereTipo----------------------------------*/
void insereTipo(TdS *ts, int tipo, int num_tipo){
    ETdS *aux = ts->topo;
    int i;
    for(i = 0; i < num_tipo; i++, aux--){
            aux->tipo = tipo;
    }
}

void insereTipoParam(TdS *ts, int tipo, int num_tipo){
    ETdS *aux = ts->topo;
    int i;
    for(i = 0; i < num_tipo; i++, aux--){
            aux->tipo = tipo;
    }
}

void preencheParams(TdS *ts, ETdS *elemento, int proc_func){
	int i;
	ETdS *aux = ts->topo;
	int deslocamento = -4;
	for(i = elemento->num_param - 1; i >= 0; i--, aux--){
		elemento->params[i] = aux->tipo;
		aux->deslocamento = deslocamento--;
	} 	
}

int verificaRetorno(TdS *ts, ETdS *elemento,  int nivel){
	ETdS *aux ;
	if(nivel >= elemento->nivel){
		
		for( aux = ts->topo; aux != ts->elementos && ( aux->nivel >= elemento->nivel); aux--);
		
		if( (++aux)->id == elemento->id){
			
			return 1;
		}
		
	}
	return 0;
}

int numVar(TdS *ts, int nivel){
	ETdS *aux;
	
	for(aux = ts->topo; aux > (ts->elementos-1) && aux->categoria != VS && aux->nivel > nivel ; aux--);
	
	if(aux > ts->elementos -1 && aux->categoria == VS)
		return aux->deslocamento + 1;
	
	return 0;
}

/*ETdS *buscaLocal(TdS *ts, char *id, int nivel){
	ETdS *aux;
	
	for(aux = ts->topo + 1; aux > (ts->elementos-1) && aux->nivel == nivel ; ){
		if()
	}
}*/

/*-----------------------------------TS----------------------------------------*/
/*===================================PT========================================*/
PdT *inicializaPdT( ){
	PdT *pt = (PdT *) malloc(sizeof(PdT));
	pt->tam = Init_TS;
	pt->elementos = (Tipo *) malloc(sizeof(Tipo)*Init_TS);
	pt->topo = pt->elementos - 1;
	
	return pt;
}
/*-----------------------------------------------------------------------------*/
/*--------------------------------empilha--------------------------------------*/
int empilhaPdT(PdT *pt, int tipo){
	
	pt->topo++;
	if(pt->topo == pt->elementos + pt->tam){
		pt->tam <<= 1;
		pt->elementos = realloc(pt->elementos, pt->tam*sizeof(Tipo));
	}
	*(pt->topo) = tipo;
	
	return 0;
}
/*-----------------------------------------------------------------------------*/
/*---------------------------------desempilha----------------------------------*/
Tipo desempilhaPdT(PdT *pt){
	pt->topo--;

	return *(pt->topo+1);
}
/*-----------------------------------------------------------------------------*/
void verificaTipo(PdT *pt, char *cmd){
	/*Modificar para param por Ref*/
	char str[str_max];
	int aux1 = desempilhaPdT(pt);
	int aux2 = desempilhaPdT(pt);
	if( aux1 != aux2  && aux1 - 5 != aux2 && aux1 != aux2 - 5 ){
		sprintf(str,"Operacao nao permitida: tipos incompativeis %d %d", aux1, aux2);
		imprimeErro(str);
	}

	geraCodigo(NULL, cmd);
}
/*-----------------------------------PT----------------------------------------*/
/*===================================PR========================================*/
PdR *inicializaPdR( ){
	PdR *pr = (PdR *) malloc(sizeof(PdR));
	pr->tam = Init_TS;
	pr->elementos = (int *) malloc(sizeof(int)*Init_TS);
	pr->topo = pr->elementos - 1;
	
	return pr;
}
/*-----------------------------------------------------------------------------*/
/*--------------------------------empilha--------------------------------------*/
int empilhaPdR(PdR *pr, int rot){
	
	pr->topo++;
	if(pr->topo == pr->elementos + pr->tam){
		pr->tam <<= 1;
		pr->elementos = realloc(pr->elementos, pr->tam*sizeof(int));
	}
	*(pr->topo) = rot;
	
	return 0;
}
/*-----------------------------------------------------------------------------*/
/*---------------------------------desempilha----------------------------------*/
int desempilhaPdR(PdR *pr){
	pr->topo--;
	
	return *(pr->topo+1);
}
/*-----------------------------------PR----------------------------------------*/

/*===================================PR========================================*/
PdF *inicializaPdF( ){
	PdF *pfunc = (PdF *) malloc(sizeof(PdF));
	pfunc->tam = Init_TS;
	pfunc->elementos = (EPdF *) malloc(sizeof(int)*Init_TS);
	pfunc->topo = pfunc->elementos - 1;
	
	return pfunc;
}
/*-----------------------------------------------------------------------------*/
/*--------------------------------empilha--------------------------------------*/
int empilhaPdF(PdF *pfunc, int ind_param, ETdS *e){
	
	pfunc->topo++;
	if(pfunc->topo == pfunc->elementos + pfunc->tam){
		pfunc->tam <<= 1;
		pfunc->elementos = realloc(pfunc->elementos, pfunc->tam*sizeof(int));
	}
	pfunc->topo->ind_param = ind_param;
	pfunc->topo->func = e;
	return 0;
}
/*-----------------------------------------------------------------------------*/
/*---------------------------------desempilha----------------------------------*/
EPdF *desempilhaPdF(PdF *pfunc){
	pfunc->topo--;
	
	return pfunc->topo+1;
}
/*-----------------------------------PR----------------------------------------*/



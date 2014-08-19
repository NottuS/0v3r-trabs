
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
int rot = 0;
int last_pos=0;
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

/* Funções da Pilha de Rótulos */
void iniciaPilhaDeRotulos(p_rot *rotulos, int *rot1, int *rot2, int tam){
	*rot1 = *rot2 = 0;
	rotulos->tam = tam; 
	rotulos->total = 0;
	rotulos->r = (int*) malloc(sizeof(int)*tam);
}


void geraProxRotulo(int *novo_rot){
	*novo_rot = rot++;
}

void empilhaRotulo(p_rot *rotulos,int rot){
	if(rotulos->total == rotulos->tam){
		rotulos->tam *= 2;
		rotulos->r = (int *) realloc(rotulos->r,sizeof(int)*rotulos->tam);
	}
	rotulos->r[rotulos->total] = rot;
	rotulos->total++;
}

int desempilhaRotulo(p_rot *rotulos){
	return(rotulos->r[--rotulos->total]);
}


/* Funções da Tabela de Símbolos */
void iniciaTS(tabS *tbs,int tam){
	tbs->total = 0 ;
	tbs->tam= tam;
 	tbs->eTS = (elementoTS*)malloc(sizeof(elementoTS)*tam); 
}

elementoTS *buscaElementoTS(tabS tbs, char *id){
	int i;
	for(i = 0; i < tbs.total; i++)
		if(!strcmp(tbs.eTS[i].id,id))
			return &tbs.eTS[i];
	
	return &tbs.eTS[0];	
	
}

int buscaID_TS(tabS tbs, char *id){
	int i;
	for(i = tbs.total-1; i >= 0; i--)
		if(!strcmp(tbs.eTS[i].id,id))
			return  i;
	
	return -1;
}

int numVarLocais(tabS tbs, int nivel){
	int nv = 0;
	int i = tbs.total-1;
	while (tbs.eTS[i].nivel == nivel+1){
        i--;
      }
    while (tbs.eTS[i].cat == variavelSimples) {
        i--;
        nv++;
    }
	return nv;

}

void atribuiDTS(tabS *tbs){
	int i = tbs->total-1, desl = -4;
	while((tbs->eTS[i].cat != procedimento) && (tbs->eTS[i].cat != funcao)){
		if(tbs->eTS[i].cat == parametroFormal){
			tbs->eTS[i].deslocamento = desl;
			desl = desl-1;
		}	
		i--;
	}
	if(tbs->eTS[i].cat == funcao)
		tbs->eTS[i].deslocamento = desl;
	
}

int empilhaElementoTS(tabS *tbs, elementoTS e){
	/* Verifica se elemento já existe na TS */	
	if((buscaID_TS(*tbs, e.id) == -1) || (e.cat == parametroFormal)){
		/* Verifica se é necessário mais elementos na TS, no caso se o tamanho ao total da tabela, o tam da tabela é dobrado */ 
		if(tbs->tam == tbs->total) { 
			tbs->tam *= 2;
			tbs->eTS = (elementoTS*)realloc(tbs->eTS,sizeof(elementoTS)*tbs->tam);
			printf("Tabela aumentada, novo tam total: %d, tam: %d\n",tbs->total, tbs->tam);
		}
		tbs->eTS[tbs->total] = e;
		tbs->total++;
		return tbs->total;
	} else {
		return -1;
	}
    last_pos++;
}

void desempilhaElementoTS(tabS *tbs,int  tam){
	tbs->total -= tam;
}


int desempilhaPPTS(tabS *tbs){
	int i = tbs->total-1, nv = 0;
	/*tbs->eTS[i].cat != programaPrincipal*/
	while(i){
		if(tbs->eTS[i].cat == variavelSimples)
			nv++;
		i--;	
		tbs->total--;
	}
	return nv;
}

int desempilhaProcFuncTS(tabS *tbs, int nivel){
	int nv = 0;
	
	/*if(tbs->eTS[i].nivel > nivel)
		tbs->total--;
	while((tbs->eTS[i].cat != procedimento) && (tbs->eTS[i].cat != funcao)){
		tbs->total--;
		if(tbs->eTS[i].cat == variavelSimples)
			nv++;
		i--;	
	}*/
	while (tbs->eTS[tbs->total-1].nivel == nivel+1){
        tbs->total--;
      }
    while (tbs->eTS[tbs->total-1].cat == variavelSimples) {
        tbs->total--;
        nv++;
    }
    while ((tbs->eTS[tbs->total-1].cat != procedimento) && (tbs->eTS[tbs->total-1].cat != funcao)){
         tbs->total--;
     }
	return nv;
}

void imprimeTS(tabS tbs){
	int i;
	for(i = 0; i < tbs.total; i++)
		printf("%d: e.id %s, e.cat %d, e.nivel %d, e.var.deslocamento %d, e.var.tipo %d, .pfTipoPassagem %d, rotulo %d\n",i,tbs.eTS[i].id,tbs.eTS[i].cat,tbs.eTS[i].nivel,tbs.eTS[i].deslocamento,tbs.eTS[i].tipo,tbs.eTS[i].pfTipoPassagem, tbs.eTS[i].rotulo);
}

/* Procedimentos & Funções */
int buscaParamProcFunc(tabS tbs, int rotulo){
	int i;
	for(i = 0; i < tbs.total; i++){
		if(((tbs.eTS[i].cat == procedimento) || (tbs.eTS[i].cat == funcao)) && (tbs.eTS[i].rotulo == rotulo)){
			return  tbs.eTS[i].nparam;
		}
	}
}

void insereParametrosTS(tabS *tbs,int num_var){
	int i = tbs->total-1;
	while((tbs->eTS[i].cat != procedimento) && (tbs->eTS[i].cat != funcao)){
		i--;	
	}
	tbs->eTS[i].nparam = num_var;
}

elementoTS *atualizaProc(tabS *tbs,int nvar, int tipo, int tipoPassagem){
	int i = tbs->total-1, j,k;
	
	while((tbs->eTS[i].cat != procedimento) && (tbs->eTS[i].cat != funcao)){
		i--;	
	}
	k = nvar;
	for(j = (tbs->eTS[i].nparam - nvar) ; k > 0; j++,k--){
		tbs->eTS[i].vParam[j].tipo = tipo;
		tbs->eTS[i].vParam[j].pfTipoPassagem = tipoPassagem;
		//printf("()()()() %s tbs->eTS[%d].vParam[%d].tipo %d, tbs->eTS[i].vParam[j].pfTipoPassagem %d\n ",tbs->eTS[i].id,i,j,tbs->eTS[i].vParam[j].tipo,tbs->eTS[i].vParam[j].pfTipoPassagem);
	}
	return &tbs->eTS[i];
}

void iniciaPilhaDeProcFunc(p_proc_func *p_pf, int tam){
	p_pf->tam = tam; 
	p_pf->total = 0; 
	p_pf->eTS = (elementoTS *) malloc(sizeof(elementoTS)*tam);
}

elementoTS *empilhaProcFunc(p_proc_func *p_pf, elementoTS eTS){
	p_pf->eTS[p_pf->total] = eTS;
	//printf("EMPILHAND PROCEDIMENTO p_pf->eTS[%d] %s , eTS %s\n",p_pf->total,p_pf->eTS[p_pf->total].id, eTS.id);
	fflush(stdout);
	p_pf->total++;
	return &p_pf->eTS[p_pf->total-1];
}

elementoTS *desempilhaProcFunc(p_proc_func *p_pf){
	//printf("DESEMPILHANDO PROCEDIMENTO p_pf->eTS[%d] %s \n",p_pf->total - 1,p_pf->eTS[p_pf->total - 1].id);
	return(&p_pf->eTS[--p_pf->total]);
}


int verificaAtrFunc(tabS tbs, char *str, int nivel) {
   int i = tbs.total-1;

    while (1) {
        while (tbs.eTS[i].cat != funcao)
            i--;
        if (tbs.eTS[i].nivel == nivel){
            if(!strcmp(str, tbs.eTS[i].id))
    		    return 1;
    		else
    			return 0;
        }	
        i--;
    }
 
}

/* Verificação de tipos */

void insereTipo(tabS *tbs,int tipo,int tipoPassagem,int deslocamento){
	//printf("S::::%s d:%d p:%d q:%d\n",nome_tipo ,deslocamento,prim,qtd);
	int i = tbs->total-1,j=0; 
	for(;j<deslocamento;j++,i--){	
		tbs->eTS[i].tipo=tipo;
		tbs->eTS[i].pfTipoPassagem=tipoPassagem;
	}
}

void iniciaPilhaDeTipos(p_tipo *ptipos, int tam){
	ptipos->tam = tam; 
	ptipos->total = 0;
	ptipos->tipo = (tipos *) malloc(sizeof(int)*tam);
}

void empilhaTipo(p_tipo *ptipos,int tipo){
	ptipos->tipo[ptipos->total] = tipo;
	ptipos->total++;
}

int desempilhaTipo(p_tipo *ptipos){
	return(ptipos->tipo[--ptipos->total]);
}


/*int insereROTTS(tabS *tbs, char *id, categorias cat, int nivel, int rot) {
    int aux;
    aux=buscaID_TS(*tbs,id);
    if ( aux == -1) {
        if (tbs->tam == (tbs->total + 1)) {
            tbs->tam *= 2;
            tbs->eTS = (elementoTS*)realloc(tbs->eTS,sizeof(elementoTS)*tbs->tam);
        }
        tbs->total++;
        strcpy(tbs->eTS[tbs->total].id, id);
        tbs->eTS[tbs->total].cat = cat;
        tbs->eTS[tbs->total].nivel = nivel;
        tbs->eTS[tbs->total].rotulo = rot;
        return 0;
    }
    return 1;
}*/



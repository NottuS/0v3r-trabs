
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
  fflush(stderr);
  exit(-1);
}

void iniciaTS(tabela *ts, int tam) {
    ts->topo = -1;
    ts->tam = tam;
    ts->elements = (elemenTS *)malloc(tam*sizeof(elemenTS));
}

int insereVSTS(tabela *ts, char *id, categorias cat, int nivel, int deslocamento) {
    if (buscaTS(ts, id) == NULL) {
        if (ts->tam == (ts->topo + 1)) {
            ts->tam *= 2;
            ts->elements = (elemenTS *)realloc(ts->elements, ts->tam*sizeof(elemenTS));
        }
        ts->topo++;
        strcpy(ts->elements[ts->topo].id, id);
        ts->elements[ts->topo].categoria = cat;
        ts->elements[ts->topo].nivel = nivel;
        ts->elements[ts->topo].deslocamento = deslocamento;
        ts->elements[ts->topo].busy = 0;
                
        return 0;
    }
    return 1;
}

void tipoTS(tabela *ts, tipos tipo, int qtde) {
    int i;
    for (i = 0; i < qtde; i++)
        ts->elements[ts->topo-i].tipo = tipo;
}

elemenTS *buscaTS(tabela *ts, char *str) {
    int i;

    for (i = ts->topo; i > -1; i--)
        if (!strcmp(str, ts->elements[i].id)) {
            if (i == 0) {
                sprintf(str, "Reserved word '%s'", ts->elements[i].id);
                imprimeErro (str);
            } else
                return &ts->elements[i];
        }
    return NULL;
}


int insereVLTS(tabela *ts, char *id, categorias cat, int nivel, int deslocamento) {
    if (buscaVLTS(ts, id) == NULL) {
        if (ts->tam == (ts->topo + 1)) {
            ts->tam *= 2;
            ts->elements = (elemenTS *)realloc(ts->elements, ts->tam*sizeof(elemenTS));
        }
        ts->topo++;
        strcpy(ts->elements[ts->topo].id, id);
        ts->elements[ts->topo].categoria = cat;
        ts->elements[ts->topo].nivel = nivel;
        ts->elements[ts->topo].deslocamento = deslocamento;
        ts->elements[ts->topo].busy = 0;
                
        return 0;
    }
    return 1;
}

elemenTS *buscaVLTS(tabela *ts, char *str) {
    int i = ts->topo;

	while ((ts->elements[i].categoria != procedimento) && (ts->elements[i].categoria != funcao)) {
        if (!strcmp(str, ts->elements[i].id)) {
            if (i == 0) {
                sprintf(str, "Reserved word '%s'", ts->elements[i].id);
                imprimeErro (str);
            } else
                return &ts->elements[i];
        }
        i--;
    }
    return NULL;
}

int inserePARAMTS(tabela *ts, char *id, categorias cat, int nivel, passagens pass) {
    if (buscaPARAMTS(ts, id) == NULL) {
        if (ts->tam == (ts->topo + 1)) {
            ts->tam *= 2;
            ts->elements = (elemenTS *)realloc(ts->elements, ts->tam*sizeof(elemenTS));
        }
        ts->topo++;
        strcpy(ts->elements[ts->topo].id, id);
        ts->elements[ts->topo].categoria = cat;
        ts->elements[ts->topo].nivel = nivel;
        ts->elements[ts->topo].passagem = pass;
        ts->elements[ts->topo].busy = 0;
                
        return 0;
    }
    return 1;
}

void deslocamentoPARAMTS(tabela *ts) {
    int i = ts->topo, j = -4;
    
    while ((ts->elements[i].categoria != procedimento) && (ts->elements[i].categoria != funcao)) {
        ts->elements[i].deslocamento = j;
        i--;
        j--;
    }
    ts->elements[i].deslocamento = j;
}

elemenTS *buscaPARAMTS(tabela *ts, char *str) {
    int i = ts->topo;

    while ((ts->elements[i].categoria != procedimento) && (ts->elements[i].categoria != funcao)) {
        if (!strcmp(str, ts->elements[i].id))
           return &ts->elements[i];
        i--;
    }
    if (ts->elements[i].categoria == funcao)
        if (!strcmp(str, ts->elements[i].id))
            return &ts->elements[i];
    
    return NULL;
}

int insereROTTS(tabela *ts, char *id, categorias cat, int nivel, int rot) {
    if (buscaTS(ts, id) == NULL) {
        if (ts->tam == (ts->topo + 1)) {
            ts->tam *= 2;
            ts->elements = (elemenTS *)realloc(ts->elements, ts->tam*sizeof(elemenTS));
        }
        ts->topo++;
        strcpy(ts->elements[ts->topo].id, id);
        ts->elements[ts->topo].categoria = cat;
        ts->elements[ts->topo].nivel = nivel;
        ts->elements[ts->topo].rotulo = rot;
        ts->elements[ts->topo].busy = 0;
                
        return 0;
    }
    return 1;
}

int qtdeVL(tabela *ts) {
	 int i = ts->topo;

    while (ts->elements[i].categoria != variavel_simples)
        i--;
    return ts->elements[i].deslocamento+1;
}

int inserePROCTS(tabela *ts, char *id, categorias cat, int nivel, int rotulo) {
    if (buscaTS(ts, id) == NULL) {
        if (ts->tam == (ts->topo + 1)) {
            ts->tam *= 2;
            ts->elements = (elemenTS *)realloc(ts->elements, ts->tam*sizeof(elemenTS));
        }
        ts->topo++;
        strcpy(ts->elements[ts->topo].id, id);
        ts->elements[ts->topo].categoria = cat;
        ts->elements[ts->topo].nivel = nivel;
        ts->elements[ts->topo].rotulo = rotulo;
        ts->elements[ts->topo].parametros[0] = 0;
        ts->elements[ts->topo].busy = 0;
                
        return 0;
    }
    return 1;
}

elemenTS *buscaPROCTS(tabela *ts, char *str) {
   int i = ts->topo, j = i;

    while (j > -1) {
        while ((ts->elements[i].categoria != procedimento) && (ts->elements[i].categoria != funcao))
            i--;
        if (!strlen(str))
            return &ts->elements[i];
        else if (!strcmp(str, ts->elements[i].id))
            return &ts->elements[i];
        i--;
        j = i;
    }
    
    return NULL;
}

int buscaFUNCTS(tabela *ts, char *str, int nivel) {
   int i = ts->topo, aux = 0;

    while (!strcmp(str, ts->elements[i].id)) {
        while (ts->elements[i].categoria != funcao)
            i--;
        if (ts->elements[i].nivel == nivel)
            aux++;
        /*else if (ts->elements[i].nivel < nivel)
            aux = 0;
        else
            return 0;*/
        i--;
    }
    /*if (aux)
        if (strcmp(str, buscaPROCTS(ts, "")->id))
            return 0;
    return 1;*/
    if(aux > 1)
    	return 0;
    return 1;
}

void liberaFOR(tabela *ts, int nivel) {
    int i = ts->topo;

    while (ts->elements[i].busy != nivel)
        i--;
    ts->elements[i].busy = 0;
}

int removeTS(tabela *ts, int nivel) {
    int aux = 0;
    
    if (ts->tam >= ((ts->topo + 1) * 4)) {
        ts->tam /= 2;
        ts->elements = (elemenTS *)realloc(ts->elements, ts->tam*sizeof(elemenTS));
    }
    while (ts->elements[ts->topo].nivel == nivel+1)
        ts->topo--;
    while ((ts->elements[ts->topo].categoria != procedimento) && (ts->elements[ts->topo].categoria != parametro_formal) && (ts->elements[ts->topo].categoria != funcao) && (ts->elements[ts->topo].categoria != rotulo)) {
        ts->topo--;
        aux++;
    }
    while ((ts->elements[ts->topo].categoria != procedimento) && (ts->elements[ts->topo].categoria != funcao))
         ts->topo--;
    
    return aux;
}

void iniciaTipo(pilhaTipo *pt, int tam) {
    pt->topo = -1;
    pt->tam = tam;
    pt->elements = (tipos *)malloc(tam*sizeof(tipos));
}

void empilhaTipo(pilhaTipo *pt, tipos tipo) {
    if (pt->tam == (pt->topo + 1)) {
        pt->tam *= 2;
        pt->elements = (tipos *)realloc(pt->elements, pt->tam*sizeof(tipos));
    }
    pt->topo++;
    pt->elements[pt->topo] = tipo;
}

tipos desempilhaTipo(pilhaTipo *pt) {
    if (pt->tam >= ((pt->topo + 1) * 4)) {
        pt->tam /= 2;
        pt->elements = (tipos *)realloc(pt->elements, pt->tam*sizeof(tipos));
    }
    pt->topo--;

    return pt->elements[pt->topo+1];
}

void iniciaRotulo(pilhaRotulo *pr, int tam) {
    pr->topo = -1;
    pr->tam = tam;
    pr->elements = (int *)malloc(tam*sizeof(int));
}

void empilhaRotulo(pilhaRotulo *pr, int num) {
    if (pr->tam == (pr->topo + 1)) {
        pr->tam *= 2;
        pr->elements = (int *)realloc(pr->elements, pr->tam*sizeof(int));
    }
    pr->topo++;
    pr->elements[pr->topo] = num;
}

int desempilhaRotulo(pilhaRotulo *pr) {
    if (pr->tam >= ((pr->topo + 1) * 4)) {
        pr->tam /= 2;
        pr->elements = (int *)realloc(pr->elements, pr->tam*sizeof(int));
    }
    pr->topo--;

    return pr->elements[pr->topo+1];
}

void iniciaAux(pilhaAux *pa, int tam) {
    pa->topo = -1;
    pa->tam = tam;
    pa->elements = (elemenTS **)malloc(tam*sizeof(elemenTS *));
    pa->num = (int *)malloc(tam*sizeof(int ));
}

void empilhaAux(pilhaAux *pa, elemenTS *element) {
    if (pa->tam == (pa->topo + 1)) {
        pa->tam *= 2;
        pa->elements = (elemenTS **)realloc(pa->elements, pa->tam*sizeof(elemenTS *));
        pa->num = (int *)realloc(pa->num, pa->tam*sizeof(int ));
    }
    pa->topo++;
    pa->elements[pa->topo] = element;
}

elemenTS *desempilhaAux(pilhaAux *pa) {
    if (pa->tam >= ((pa->topo + 1) * 4)) {
        pa->tam /= 2;
        pa->elements = (elemenTS **)realloc(pa->elements, pa->tam*sizeof(elemenTS *));
        pa->num = (int *)realloc(pa->num, pa->tam*sizeof(int ));
    }
    pa->topo--;
    if (pa->topo >= 0)
        return pa->elements[pa->topo];
    else
        return NULL;
}

elemenTS *buscaAux(pilhaAux *pa) {
    return pa->elements[pa->topo];
}

void protocoloZero(tabela *ts, pilhaTipo *pt, pilhaRotulo *pr, pilhaAux *pa) {
    free(ts->elements);
    free(pt->elements);
    free(pr->elements);
    free(pa->elements);
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "smpl.h"
#include "funcao_cis.h"

#define test 1
#define fault 2
#define repair 3 

typedef struct {
	int id;
	int* state;
    int s;
	int desatualizado;
} tnodo;

tnodo *nodo;

int id_prox(int token, int s, int j, int max_comp) {
	node_set* nodes;
    /*Verifica se há mais nodos*/
	if (j > max_comp)
		return -1;
	nodes = cis(token, s);
	return nodes->nodes[j - 1];
}

void print_state(int token, int N) {
	int i;
	printf("	state:[");
	for (i = 0; i < N; i++) {
		printf(" %d", nodo[token].state[i]);
	}
	printf(" ]\n\n");
}

void get_state(int token, int nodo_test, int s, int max_comp, int j) {
    /*Copia-se o vetor de state do nodo_test, apartir dos nodos do cluster que não foram testados pelo nodo token*/
	if (nodo[nodo_test].desatualizado == 0) {
		for (; j <= max_comp; j++) {
			nodo[token].state[id_prox(token, s, j, max_comp)] = nodo[nodo_test].state[id_prox(token, s, j, max_comp)];
		}
	}
}

main(int argc, char* argv[]) {
	static int N, token, event, r, i, fault_free, nodo_test, max_comp, j, s_max, s_ultimo, lgN, pow2, latencia, nodosVerificados = 0, num_testes = 0, nodosOk;
	static char fa_name[5];
	if (argc != 2) {
		puts("Uso correto: tarefa_tempo <num-nodos>");
		exit(1);
	}
	N = atoi(argv[1]);
    lgN = ceil(log2(N));
    pow2 = pow(2, lgN);
	smpl(0, "Um exemplo de simulação");
	reset();
	stream(1);
	nodo = (tnodo*) malloc(sizeof(tnodo) * pow2);
	for (i = 0; i < N; i++) {
		memset(fa_name, '\0', 5);
		sprintf(fa_name, "%d", i);
		nodo[i].id = facility(fa_name, 1);
		nodo[i].state = (int*) malloc(sizeof(int) * pow2);
		memset(nodo[i].state, -1, sizeof(int) * pow2);
        nodo[i].s = 1;
		nodo[i].desatualizado = 0;
	}
    /*Cria nodos falsos*/
    for(; i < pow2; i++){
        memset(fa_name, '\0', 5);
		sprintf(fa_name, "%d", i);
		nodo[i].id = facility(fa_name, 1);
        nodo[i].state = (int*) malloc(sizeof(int) * pow2);
		request(nodo[i].id, token, 0); 
		nodo[i].desatualizado = 0;
    }
    /*Faz lgN ajendamentos de testes iniciais para os N nodos*/
	for (j = 0; j < lgN; j++) {
		for (i = 0; i < N; i++)
			schedule(test, 30.0, i);
	}
	schedule(fault, 40.0, 0);
	schedule(fault, 70.0, 1);
	schedule(fault, 110.0, 2);
	schedule(fault, 140.0, 3);
	//schedule(fault, 140.0, 3);
	max_comp = 1;
	s_max = j;
    s_ultimo = 1;
	nodosOk = N;
	printf("\nTempo de execução:200\n");
	printf("\n_______________________________________________________________________________\n\n");
	while (time() < 200.0) {
		cause(&event, &token);
		/*Evita que testes sejam feitos após o tempo T*/
		if (time() >= 200.00) break;
		switch (event) {
		case test:                
			if (status(nodo[token].id) != 0){
                //nodo[token].s = nodo[token].s + 1;
                //if( nodo[token].s == s_max + 1 ){
                //     nodo[token].s = 1;           
                //}
                break;
            }
			if(nodo[token].desatualizado == 1)
				nodo[token].desatualizado = 0;
            if(nodo[token].s != s_ultimo){
				
                printf("_______________________________________________________________________________\n\n");                       
            }
            s_ultimo = nodo[token].s;
			printf("sou o nodo %d vou testar no tempo %5.1f, para s = %d, o(s) nodo(s):", token, time(), nodo[token].s);
			nodo[token].state[token] = 0;
			j = 1;
            /*Testa os nodos do cluster até encontrar um nodo sem falha*/
			while (((nodo_test = id_prox(token, nodo[token].s, j, max_comp << (nodo[token].s-1) )) != -1) && (status(nodo[nodo_test].id) != 0)) {
                /*Verifica se o nodo existe*/                   
                if( nodo_test < N ){
				    nodo[token].state[nodo_test] = 1;
				    printf(" %d", nodo_test);
					num_testes++;
                }
				j++;
			}
            /*Caso o nodo token testou um nodo sem falha, copia-se o vetor de state desse, apartir dos nodos do cluster que não foram testados*/ 
			if ((nodo_test != -1)) {
				printf(" %d  ", nodo_test);
				nodo[token].state[nodo_test] = 0;
				get_state(token, nodo_test, nodo[token].s, max_comp << (nodo[token].s-1), j+1);
			}
			printf("\n");
			print_state(token, N);
			schedule(test, 30.0, token);
            nodo[token].s = nodo[token].s + 1;
            if( nodo[token].s == s_max + 1 ){
                 nodo[token].s = 1;           
            }
			nodosVerificados++;
			if(nodosVerificados == nodosOk)
				print
			break;
		case fault:
			r = request(nodo[token].id, token, 0);
			nodosOk--;
            printf("_______________________________________________________________________________\n\n");
			nodo[token].desatualizado = 1;
			if (r != 0) {
				printf("***Não consegui falhar o nodo %d*** \n\n", token);
				exit(1);
			}
			printf("***O nodo %d falha no tempo %5.1f*** \n\n", token, time());
			
          	break;
		case repair:
            printf("_______________________________________________________________________________\n\n");
			printf("***O nodo %d recuperou no tempo %5.1f*** \n\n", token, time());
			release(nodo[token].id, token);
			nodosOk++;
			latencia = time();
			nodo[token].s = 1;
			for (j = 0; j < ceil(log2(N)); j++)
				schedule(test, 30.0, token);
          	break;
		}
	}
}

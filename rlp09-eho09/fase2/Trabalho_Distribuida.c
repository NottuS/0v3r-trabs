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
	int* timestamp;
    int s;
} tnodo;

tnodo *nodo;

int id_prox(int token, int s, int j, int max_comp) {
	node_set* nodes;
	int i;
    /*Verifica se há mais nodos*/
	if (j > max_comp)
		return -1;
	nodes = cis(token, s);
	return nodes->nodes[j - 1];
}

void print_timestamp(int token, int N) {
	int i;
	printf("	timestamp:[");
	for (i = 0; i < N; i++) {
		printf(" %d", nodo[token].timestamp[i]);
	}
	printf(" ]\n\n");
}

void get_timestamp(int token, int nodo_test, int N) {
    /*Copia-se o vetor de timestamp do nodo_test, se o do nodo_test for maior doque o do token*/
	int j;
	for (j = 0; j < N; j++) {
		if (nodo[token].timestamp[j] < nodo[nodo_test].timestamp[j])
			nodo[token].timestamp[j] = nodo[nodo_test].timestamp[j];
	}
}

main(int argc, char* argv[]) {
	static int N, token, event, r, i, fault_free, nodo_test, max_comp, j, s_max, s_ultimo, lgN, pow2, aux, found_tester, k;
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
		nodo[i].timestamp = (int*) malloc(sizeof(int) * pow2);
		memset(nodo[i].timestamp, -1, sizeof(int) * pow2);
        nodo[i].s = 1;
	}
    /*Cria nodos falsos*/
    for(; i < pow2; i++){
        memset(fa_name, '\0', 5);
		sprintf(fa_name, "%d", i);
		nodo[i].id = facility(fa_name, 1);
        nodo[i].timestamp = (int*) malloc(sizeof(int) * pow2);
		request(nodo[i].id, token, 0); 
    }
    /*Faz lgN ajendamentos de testes iniciais para os N nodos*/
	for (j = 0; j < lgN; j++) {
		for (i = 0; i < N; i++)
			schedule(test, 30.0, i);
	}
	
	max_comp = 1;
	s_max = j;
    s_ultimo = 1;
	printf("\nTempo de execução:200\n");
	printf("\n_______________________________________________________________________________\n\n");
	while (time() < 200.0) {
		cause(&event, &token);
		/*Evita que testes sejam feitos após o tempo T*/
		if (time() >= 200.00) break;
		switch (event) {
		case test:                
			if (status(nodo[token].id) != 0){
                break;
            }

            if (nodo[token].s != s_ultimo){
                printf("_______________________________________________________________________________\n\n");                       
            }
            s_ultimo = nodo[token].s;
			printf("sou o nodo %d vou testar no tempo %5.1f, para s = %d, o(s) nodo(s):", token, time(), nodo[token].s);
			
			if(nodo[token].timestamp[token] == -1)
				nodo[token].timestamp[token] = 0;
			j = 1;
            /*Testa os nodos do cluster*/
			while ((aux = id_prox(token, nodo[token].s, j, max_comp << (nodo[token].s-1))) != -1){
				k = 1;
				found_tester = 0;
                /*Verifica se o nodo aux existe*/
				if (aux < N){
                    /*Encontra quem deve testar o nodo aux*/		
					while ((found_tester != 1) && ((nodo_test = id_prox(aux, nodo[token].s, k, max_comp << (nodo[token].s-1))) != -1)){
						if ((nodo_test < N) && (status(nodo[nodo_test].id) == 0))
							found_tester = 1;
                        k++;
					}
                  
					if (nodo_test == token){
						printf(" %d", aux);
                        /*Caso o nodo token testou um nodo sem falha, copia-se o vetor de timestamp*/ 
						if (status(nodo[aux].id) == 0){
                            /*Verifica se o nodo testado mudou de estado*/
							if(nodo[token].timestamp[aux] % 2 != status(nodo[aux].id))
								nodo[token].timestamp[aux] ++;
							get_timestamp(token, aux, N);
						}else{
                             /*Verifica se o nodo foi testado pela primeira vez*/
							if(nodo[token].timestamp[aux] == -1)
								nodo[token].timestamp[aux] = 1;
							else if(nodo[token].timestamp[aux] % 2 != status(nodo[aux].id))
								nodo[token].timestamp[aux] ++;
						}
					}
				}
           		j++;
			}
        	printf("\n");
			print_timestamp(token, N);
			schedule(test, 30.0, token);
            nodo[token].s = nodo[token].s + 1;
            if( nodo[token].s == s_max + 1 ){
                 nodo[token].s = 1;           
            }
			break;
		case fault:
			r = request(nodo[token].id, token, 0);
            printf("_______________________________________________________________________________\n\n");
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
			nodo[token].s = 1;
			for (j = 0; j < ceil(log2(N)); j++)
				schedule(test, 30.0, token);
          	break;
		}
	}
}

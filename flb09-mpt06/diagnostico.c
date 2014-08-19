 #include<stdio.h>
#include<stdlib.h>
#include"smpl.h"
#include "cisj.h"

#define test 1
#define fault 2
#define repair 3
#define timeEvent 100
#define interval 30.0

typedef struct {
	int id, s, *state;/* s: guarda o cluster que deve ser diagnosticado
						 state: guarda o estado dos outros nodos.
					  */
} tnodo;

tnodo *nodo;

main(int argc, char *argv[]) {
	static int N, token, event, r, i, j, aux, ln, pow2s, barra = 0, falho, nexiste;
	static char fa_name[5];
 
	if(argc != 2) {
		puts("Uso correto: tempo <num_nodos>");
		exit(1);
	}
	N = atoi(argv[1]);
	ln = (int)ceil(log2((double)N));
	smpl(0, "Um exemplo de simulação");
	reset();
	stream(1);
	nodo = (tnodo *)malloc(sizeof(tnodo)*N);
	for(i=0; i<N; i++) {
		memset(fa_name, '\0', 5);
		sprintf(fa_name, "%d", i);
		nodo[i].id = facility(fa_name, 1);
		nodo[i].s = (N < 2) ? 0 : 1;
		nodo[i].state = (int *)malloc(sizeof(int)*N);
		for(j=0; j<N; j++)
			nodo[i].state[j] = -1;
		nodo[i].state[i] = 0;
	}
	
	//schedule(fault, 20.0, 9);	
	for(i=0; i<N; i++)
		schedule(test, interval, i);
	/*schedule(repair, 40.0, 9);
	schedule(fault, 70.0, 9);
	schedule(repair, 100.0, 9);
	schedule(fault, 130.0, 9);
	schedule(repair, 160.0, 9);*/

	
	
	while(time() < timeEvent) {
		cause(&event, &token);
		if(time() < timeEvent) // Para evitar que sejam feitos testes no tempo limite.
			switch(event) {
				case test:
					if(status(nodo[token].id) != 0) break;
					if(nodo[token].s != ln) { // Verifica se o nodo está diagnosticando o ultimo cluster
						if(!barra && (nodo[token].s == 1)) { // Auxilia na vizualiação do log
							barra++;
							printf("================================================================================\n\n");
						} else if((barra > 0) && (nodo[token].s > barra)) {
							barra++;
							printf("--------------------------------------------------------------------------------\n\n");
						}
						schedule(test,0.0,token);
					} else {
						if(barra) {
							barra = 0;
							printf("--------------------------------------------------------------------------------\n\n");
						}
						schedule(test,interval,token);
					}
				
					printf("Sou o nodo %d vou testar o cluster s=%d no tempo %5.1f",
									token, nodo[token].s, time());
					node_set* nodes = cis(token, nodo[token].s);
					pow2s = 1<<(nodo[token].s-1);
					j = 0;
					aux = nodes->nodes[j];
					if(aux >= N) { // Verifica se o nodo a ser testado existe ou está falho.
						falho = 0;
						nexiste = 1;
					}
					else if(status(nodo[aux].id) != 0) {
						nexiste = 0;
						falho = 1;
					} else {
						nexiste = 0;
						falho = 0;
					}
					while((j < pow2s) && (falho || nexiste)) { // Percorre o cluster enquando encontrar nodos falhos ou identificar nodos não existentes
						if(!nexiste) {
							nodo[token].state[aux] = 1;
							printf("\n\tTestei o nodo %d falho", aux);
						}
						j++;
						aux = nodes->nodes[j];
						if(aux >= N) {
							falho = 0;
							nexiste = 1;
						}
						else if(status(nodo[aux].id) != 0) {
							nexiste = 0;
							falho = 1;
						} else {
							nexiste = 0;
							falho = 0;
						}
					}
					if(j < pow2s) { // Se algum nodo sem falha for encontrado.
						nodo[token].state[aux] = 0;
						j++;
						i = nodes->nodes[j];
						while(j < pow2s) { // Copia o vetor state do nodo sem falha
							if((i < N) && (nodo[aux].state[i] != -1)) // Verifica se o nodo, que será copiado o estado, existe e se seu estado não é desconhecido.
								nodo[token].state[i] = nodo[aux].state[i];
							j++;
							i = nodes->nodes[j];
						}
						printf("\n\tTestei o nodo %d sem falha", aux);
					}
					printf(", state: [");
					for(i=0; i<N; i++) {
							printf(" %d", nodo[token].state[i]);
						}
						puts(" ]");
					if(ln) 
						nodo[token].s = (nodo[token].s)%ln + 1; // incrementa o cluster s 
					
					puts("");
					break;
				case fault:
					if ((N-1) < token){ 
						printf("Não consegui falhar o nodo %d\n\n", token);
						exit(1);
					}
					r = request(nodo[token].id, token, 0);
					printf("================================================================================\n\n");
					if(r != 0) {
						printf("Não consegui falhar o nodo %d\n\n", token);
						exit(1);
					}
					printf(" => => => O nodo %d falha no tempo %5.1f\n\n", token, time());
					break;
				case repair:
					printf("================================================================================\n\n");
					printf(" => => => O nodo %d recuperou no tempo %5.1f\n\n", token, time());
					release(nodo[token].id, token);
					for(i = 0; i < N; i++)
						nodo[token].state[i] = -1;
					nodo[token].state[token] = 0;
					schedule(test, interval,token);
					break;
			}
	}
	printf("================================================================================\n");
}

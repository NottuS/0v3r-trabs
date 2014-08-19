#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct Combinacao {
	int *potencia, turbinas, queda;
	float ponderada;
}Combinacao;

typedef struct Usina {
	int queda_usina, potencia_usina;
	int turbinas, potencias, *valores_potencias, quedas, *alturas_quedas;
	float *eficiencias;
}Usina;

Usina usina;

int buscaBinaria(int *vetor, int inicio, int fim, int x) {
	int indice;

	if (inicio <= fim) {
		indice = (inicio + fim )/2;
		if (vetor[indice] > x)
			return buscaBinaria(vetor, inicio, indice-1, x);
		if (vetor[indice] < x)
			return buscaBinaria(vetor, inicio+1, fim, x);
		return indice;
	}
	return -1;
}

void otimiza() {
}

void *eficienciaPonderada(void *num) {
	Combinacao *combinacao;
	int i, potencia_atual = 0, n = (int)num;

	combinacao = malloc(sizeof(Combinacao));

	combinacao->turbinas = n;
	combinacao->potencia = (int *)malloc (combinacao->turbinas * sizeof(int));
	combinacao->queda = buscaBinaria(usina.alturas_quedas, 0, usina.quedas, usina.queda_usina);
	
	for (i = 0; i < combinacao->turbinas; i++) {
		combinacao->potencia[i] = usina.potencias-1;
		potencia_atual += usina.valores_potencias[usina.potencias-1];
	}
	i = 0;
	while (potencia_atual > usina.potencia_usina) {
		potencia_atual -= usina.valores_potencias[combinacao->potencia[i]] -
			usina.valores_potencias[combinacao->potencia[i]-1];
		combinacao->potencia[i] -= 1;
		i = (i + 1) % combinacao->turbinas;
	}
	
	combinacao->ponderada = 0;
	for (i = 0; i < combinacao->turbinas; i++) {
		combinacao->ponderada += usina.valores_potencias[combinacao->potencia[i]] *
			usina.eficiencias[combinacao->potencia[i] * usina.quedas + combinacao->queda];
	}
	combinacao->ponderada /= usina.potencia_usina;

	pthread_exit(combinacao);
}

int main(int argc, char *argv[]) {
	int i, j, maior_eficiencia, aux;
	pthread_t *thread;
	Combinacao **combinacao;

	usina.queda_usina = atoi (argv[1]);
	usina.potencia_usina = atoi (argv[2]);

	/* ---------- LEITURA DE DADOS DA USINA E DAS TURBINAS ------------ */
	scanf ("%d", &usina.turbinas);
	scanf ("%d", &usina.potencias);
	usina.valores_potencias = (int *)malloc (usina.potencias * sizeof(int));
	for (i = 0; i < usina.potencias; i++)
		scanf ("%d", &usina.valores_potencias[i]);
	scanf ("%d", &usina.quedas);
	usina.alturas_quedas = (int *)malloc (usina.quedas * sizeof(int));
	for (i = 0; i < usina.quedas; i++)
		scanf ("%d", &usina.alturas_quedas[i]);
	usina.eficiencias = (float *)malloc (usina.potencias * usina.quedas * sizeof(float));
	for (i = 0; i < usina.potencias; i++)
		for (j = 0; j < usina.quedas; j++)
			scanf ("%f", &usina.eficiencias[i*usina.quedas+j]);
	/* ------------------------- FIM DA LEITURA ----------------------- */

	/* ---------- CALCULO DA MAXIMA EFICIENCIA PONDERADA ------------- */
	thread = (pthread_t *)malloc(usina.turbinas * sizeof(pthread_t));
	maior_eficiencia = usina.turbinas;
	aux = usina.turbinas;
	combinacao = malloc(usina.turbinas * sizeof(Combinacao));

	for (i = aux; i * usina.valores_potencias[usina.potencias-1] >= usina.potencia_usina; i--)
		pthread_create (&thread[i], NULL, eficienciaPonderada, (void *)i);
	
	for (i = aux; i * usina.valores_potencias[usina.potencias-1] >= usina.potencia_usina; i--)
		pthread_join (thread[i], (void *)&combinacao[i]);
	
	for (++i; i < aux; i++)
		if (combinacao[i]->ponderada > combinacao[maior_eficiencia]->ponderada)
			maior_eficiencia = i;
	/* ------------------------- FIM DO CALCULO ----------------------- */

	/* -------------------- IMPRESSAO DOS RESULTADOS ------------------ */
	j = 0;
	printf ("eficiencia ponderada = %1.5f\n", combinacao[maior_eficiencia]->ponderada);
	for (i = j; i < combinacao[maior_eficiencia]->turbinas; i = j ) {
		i++;
		for (j = i, aux = 1; j < combinacao[maior_eficiencia]->turbinas &&
			combinacao[maior_eficiencia]->potencia[j-1] == combinacao[maior_eficiencia]->potencia[j]; j++ )
			aux++;
		if (aux == 1)
			printf ("1 turbina em %d MW e eficiencia %1.4f\n",
				usina.valores_potencias[combinacao[maior_eficiencia]->potencia[j-1]],
				usina.eficiencias[combinacao[maior_eficiencia]->potencia[j-1]*usina.quedas+combinacao[maior_eficiencia]->queda]);
		else
			printf ("%d turbinas em %d MW e eficiencia %1.4f\n", aux,
				usina.valores_potencias[combinacao[maior_eficiencia]->potencia[j-1]],
				usina.eficiencias[combinacao[maior_eficiencia]->potencia[j-1]*usina.quedas+combinacao[maior_eficiencia]->queda]);
	}
	/* ------------------------- FIM DA IMPRESSAO -------------------- */

	return 0;
}

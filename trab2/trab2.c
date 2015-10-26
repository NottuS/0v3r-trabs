#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define DEAD 0
#define LIVE 1
#define BLOCKSIZE 64
#define EXTRASIZE 1
#define PROBLEM_SIZE 8192

void generateMatrix(int n, int *matrix){
	int i;

	//matrix = (int **) malloc(sizeof(int) * n * n);

	srand(time(NULL));
	for (i = 0; i < n * n; ++i)	{
		matrix[i] = rand() % 2;
	}
}

void distributeMatrix(int problemSize, int  *matrix, int *partialMatrix, rank, size){
	MPI_Datatype blocktype;
    MPI_Datatype blocktype2;
    int block[BLOCKSIZE];
    int end = 0;
    int total = 0;
    int i,j,k,l;
    MPI_Type_vector(BLOCKSIZE, BLOCKSIZE, COLS, MPI_INT, &blocktype2);
    MPI_Type_create_resized( blocktype2, 0, sizeof(int), &blocktype);
    MPI_Type_commit(&blocktype);

    i = 0;
    l= 0;
    while(!end) {
    	MPI_Scatterv(matrix, size*BLOCKSIZE*BLOCKSIZE, disps, blocktype, &block,
    		 BLOCKSIZE*BLOCKSIZE, MPI_INT, 0, MPI_COMM_WORLD);
    	for (j = l; j < l;j++)
    	{
    		for (k = i; k < (i+1); ++k)
	    	{
	    		partialMatrix[j*PROBLEM_SIZE + k] = block[(j % BLOCKSIZE) * BLOCKSIZE + (k % BLOCKSIZE)];
	    	}
    	}
    	total++;
    	i = (i + BLOCKSIZE) % PROBLEM_SIZE;
    	if(i = 0){
    		l = (l + BLOCKSIZE) % PROBLEM_SIZE;
    	}

    	if( total = ((PROBLEM_SIZE/BLOCKSIZE) * (PROBLEM_SIZE/BLOCKSIZE))/size ){
    		end = true;
    	}
    }
}

void applyRules(int *block, int stride, int size, int rank, int extra){
	int i, j, k, l;
	int deadCount, liveCount;
	for (i = 0; i < size; ++i)
	{
		for (j = 0; j < size; j++){
			deadCount = 0;
			liveCount = 0;
			//arrumar indices: add stride ou n
			for (k = -1; k < 2; k++)
			{
				for (l = -1; l < 2; ++l)
				{
					if (block[(i+extra+k) * size + (j+extra+l)] && k != 1 && l != 1) {
						liveCount++;
					} /*else {
						deadCount++;
					}*/
				}
			}
			if(block[(i+extra) * size + (j+extra)] && liveCount < 2){
				block[(i+extra) * size + (j+extra)] = DEAD;
			}
			if (block[(i+extra) * size + (j+extra)]
				 && (liveCount == 2 || liveCount == 3)) {
				block[(i+extra) * size + (j+extra)] = LIVE;
			}
			if(block[(i+extra) * size + (j+extra)] && liveCount > 3){
				block[(i+extra) * size + (j+extra)] = DEAD;	
			}
			if(!block[(i+extra) * size + (j+extra)] && liveCount == 3){
				block[(i+extra) * size + (j+extra)] = LIVE;
			}
		}
	}
}

int main(int argc, char* argv[]){
	int size, rank, i, cycles;
	MPI_Status status;
	int matrix[PROBLEM_SIZE * PROBLEM_SIZE];
	int partialMatrix[(PROBLEM_SIZE * PROBLEM_SIZE)];
	int block[BLOCKSIZE * BLOCKSIZE + 4 * BLOCKSIZE)];
	int extra[BLOCKSIZE];

	

	/* Initialize MPI environment */
	MPI_Init(&argc, &argv) ;
	MPI_Comm_size(MPI_COMM_WORLD, &size) ;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
	if (rank == 0)
	{
		generateMatrix(PROBLEM_SIZE, matrix);
	}
	distributeMatrix(PROBLEM_SIZE, matrix, partialMatrix,rank, size);
	for (int i = 0; i < cycles; ++i)
	{
		//pega o bloco da matrix
	 	//4 x recebe e envia extra
	 	// copia extra
		applyRules();
	}
	
	MPI_Send(A, 10000, MPI_FLOAT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	MPI_Recv(B, 10000, MPI_FLOAT, (rank - 1) < 0 ? size -1 : rank - 1 
		, 0, MPI_COMM_WORLD, &status);

	if (rank == 0)
	{
		//gather
	}
	MPI_Finalize() ;
	return 0;
}

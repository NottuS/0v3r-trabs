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

void applyRules(int *write, int *read, int stride, int numProc, int rank, int extra){
	int i, j, k, l;
	int deadCount, liveCount;
	for (i = 0; i < numProc; ++i)
	{
		for (j = 0; j < numProc; j++){
			deadCount = 0;
			liveCount = 0;
			//arrumar indices: add stride ou n
			for (k = -1; k < 2; k++)
			{
				for (l = -1; l < 2; ++l)
				{
					if (read[(i+extra+k) * numProc + (j+extra+l)] && k != 1 && l != 1) {
						liveCount++;
					} /*else {
						deadCount++;
					}*/
				}
			}numProc
			if(read[(i+extra) * numProc + (j+extra)] && liveCount < 2){
				write[(i+extra) * numProc + (j+extra)] = DEAD;
			}
			if (write[(i+extra) * numProc + (j+extra)]
				 && (liveCount == 2 || liveCount == 3)) {
				write[(i+extra) * numProc + (j+extra)] = LIVE;
			}
			if(write[(i+extra) * numProc + (j+extra)] && liveCount > 3){
				write[(i+extra) * numProc + (j+extra)] = DEAD;	
			numProc			if(!write[(i+extra) * numProc + (j+extra)] && liveCount == 3){
				write[(i+extra) * numProc + (j+extra)] = LIVE;
			}
		}
	}
}

int main(int argc, char* argv[]){
	int numProc, rank;
	int i, j, k, cycles;
	MPI_Status status;
	int matrix[2][PROBLEM_SIZE * PROBLEM_SIZE]
	int block[BLOCKSIZE * BLOCKSIZE + 4 * BLOCKSIZE)];
	int extra[4][BLOCKSIZE];


	/* Initialize MPI environment */
	MPI_Init(&argc, &argv) ;
	MPI_Comm_size(MPI_COMM_WORLD, &numProc) ;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
	if (rank == 0)
	{
		generateMatrix(PROBLEM_SIZE, matrix);
	}
	
	for (k = 0, i = 0, j = 0; i < cycles; ++i)
	{
		//4 x recebe e envia extra
	 	// copia extra

		applyRules(&matrix[j][0], &matrix[(j + 1) % 2][0], 0, numProc, rank, EXTRASIZE);
		j = (j + 1) % 2;
	}
	
	MPI_Send(&extra[k][0], BLOCKSIZE * EXTRASIZE, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	MPI_Recv(B, 10000, MPI_FLOAT, (rank - 1) < 0 ? size -1 : rank - 1 
		, 0, MPI_COMM_WORLD, &status);

	if (rank == 0)
	{
		//gather
	}
	MPI_Finalize() ;
	return 0;
}

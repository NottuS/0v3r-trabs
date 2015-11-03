#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define BLOCKSIZE 64
#define EXTRASIZE 1
#define PROBLEM_SIZE 16
#define UP 0
#define UPRIGHT 1
#define RIGHT 2
#define DOWNRIGHT 3
#define DOWN 4
#define DOWNLEFT 5
#define LEFT 6
#define UPLEFT 7

void generateMatrix(int n, int *matrix, int rank){
	int i;

	srand(time(NULL) + rank);
	for (i = 0; i < n * n; ++i)	{
		matrix[i] = rand() % 2;
	}
}

void applyRules(int *read, int *write){
	int i, j, k, l;
	int liveCount;
	for (i = 1; i < PROBLEM_SIZE + 1; ++i)
	{
		for (j = 1; j < PROBLEM_SIZE + 1; j++){
			liveCount = 0;
			//arrumar indices: add stride ou n
			for (k = -1; k < 2; k++)
			{
				for (l = -1; l < 2; ++l)
				{
					if (k != 0 && l != 0) {
						liveCount += read[(i+k) * (PROBLEM_SIZE + 2) + (j+l)];
					} 
				}
			}

			if(liveCount == 2){
				write[(i) * (PROBLEM_SIZE + 2) + (j)] = read[(i) * (PROBLEM_SIZE + 2) + (j)] ;
			}
			if (liveCount == 3) {
				write[(i) * (PROBLEM_SIZE + 2) + (j)] = 1;
			}
			if(liveCount < 2){
				write[(i) * (PROBLEM_SIZE + 2) + (j)] = 0;	
			}
			if(liveCount > 3){
				write[(i) * (PROBLEM_SIZE + 2) + (j)] = 0;
			}
		}
	}
}

void divideMatrix(int n, int *dim){
	int i = 3;
	int cols, rows;

	cols = n/2;
	rows = 2;
	while( i < cols){
		if(n % i == 0){
			cols = n / i;
			rows = i;
		}
		i++;
	}
	dim[0] = rows;
	dim[1] = cols;
	printf("%d %d\n", rows, cols);
}

int main(int argc, char* argv[]){
	int numProc, rank, tag;
	int i, j, k, l, n, cycles;
	MPI_Comm comm;
    int dim[2], period[2], reorder;
    int coords[2], id;
	MPI_Status status;
	MPI_Request request;
	int matrix[2][(PROBLEM_SIZE+2) * (PROBLEM_SIZE+2)];
	int extra[4][PROBLEM_SIZE];
	int extraDiag[4];
	int direction[8];
	int *result;
	int sendMe;
	FILE * file;
	char path[100];

	//TODO subistiuir (PROBLEM_SIZE+2) por uma constant
	
	/*dim[0]=sqrt(numProc); dim[1]=srqt(numProc);integer array of size 
	ndims specifying the number of processes in each dimension*/
    period[0]=1; period[1]=1;/*logical array of size ndims specifying 
    whether the grid is periodic (true) or not (false) in each dimension*/
    reorder=1;/* ranking may be reordered (true) or not (false) (logical)*/
    tag = 0;

 	int lastLine = (PROBLEM_SIZE+2-1) * (PROBLEM_SIZE+2);
 	int lastElement = (PROBLEM_SIZE+2) * (PROBLEM_SIZE+2) - 1;
 	int rank_source;
 	int rank_dest;

	/* Initialize MPI environment */
	MPI_Init(&argc, &argv) ;
	MPI_Comm_size(MPI_COMM_WORLD, &numProc) ;
	divideMatrix(numProc, dim);
	MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &comm);
	MPI_Comm_rank(comm, &rank) ;
	generateMatrix(PROBLEM_SIZE + 2, &matrix[0][0], rank);
	
	MPI_Cart_shift( comm, 0, 1, &rank_source, &rank_dest);
	direction[LEFT] = rank_source;
	direction[RIGHT] = rank_dest;
	printf("me%d LEFT%d RIGHT%d\n", rank, direction[LEFT], direction[RIGHT]);
	MPI_Cart_shift( comm, 1, 1, &rank_source, &rank_dest);
	direction[UP] = rank_source;
	direction[DOWN] = rank_dest;
	printf("me%d up%d down%d\n", rank, direction[UP], direction[DOWN]);
	MPI_Cart_coords(comm, rank, 2, coords);
	coords[0] ++;
	coords[1] ++;
	MPI_Cart_rank(comm, coords, &rank_dest);
	direction[UPRIGHT] = rank_dest;
	coords[0] -= 2;
	MPI_Cart_rank(comm, coords, &rank_dest);
	direction[DOWNRIGHT] = rank_dest;
	coords[1] -= 2;
	MPI_Cart_rank(comm, coords, &rank_dest);
	direction[DOWNLEFT] = rank_dest;
	coords[0] += 2;
	MPI_Cart_rank(comm, coords, &rank_dest);
	direction[UPLEFT] = rank_dest;

	cycles = 2;
	
	for (k = 0, i = 0, j = 0; i < cycles; ++i)
	{
		sprintf(path,"/home/eholiveira/0v3r-trabs/trab2/gol-%d-%d.out",rank, i);
	
		file = fopen(path, "w");

		MPI_Isend(&matrix[j][0], 1, 
			MPI_INT, direction[UPLEFT], tag, comm, &request);
		
		MPI_Isend(&matrix[j][(PROBLEM_SIZE+2-1)], 1, 
			MPI_INT, direction[UPRIGHT], tag, comm, &request);

		MPI_Isend(&matrix[j][lastLine], 1, 
			MPI_INT, direction[DOWNLEFT], tag, comm, &request);
		
		MPI_Isend(&matrix[j][lastElement], 1, 
			MPI_INT, direction[DOWNRIGHT], tag, comm, &request);
		
		for (n = 1; n < PROBLEM_SIZE + 1; ++n)
		{
			extra[0][n - 1] = matrix[j][n * (PROBLEM_SIZE+2)];
			extra[1][n - 1] = matrix[j][(n+1) * (PROBLEM_SIZE+2) - 1];
		}
		MPI_Isend(&extra[0][0], PROBLEM_SIZE, 
			MPI_INT, direction[LEFT], tag, comm, &request);

		MPI_Isend(&extra[1][0], PROBLEM_SIZE, 
			MPI_INT, direction[RIGHT], tag, comm, &request);

		MPI_Isend(&matrix[j][PROBLEM_SIZE + 2 + 1], PROBLEM_SIZE, 
			MPI_INT, direction[UP], tag, comm, &request);

		MPI_Isend(&matrix[j][lastLine - PROBLEM_SIZE + 1], PROBLEM_SIZE, 
			MPI_INT, direction[DOWN], tag, comm, &request);

		MPI_Recv(&matrix[j][0], 1, MPI_FLOAT, direction[UPLEFT],
			 tag, comm, &status);

		MPI_Recv(&matrix[j][(PROBLEM_SIZE+2-1)], 1, MPI_FLOAT, direction[UPRIGHT],
			 tag, comm, &status);

		MPI_Recv(&matrix[j][lastLine], 1, MPI_FLOAT, direction[DOWNLEFT],
			 tag, comm, &status);

		MPI_Recv(&matrix[j][lastElement], 1, MPI_FLOAT, direction[DOWNRIGHT],
			 tag, comm, &status);
				
		MPI_Recv(&extra[2][0], PROBLEM_SIZE, MPI_INT, direction[LEFT],
			 tag, comm, &status);

		MPI_Recv(&extra[3][0], PROBLEM_SIZE, MPI_INT, direction[RIGHT],
			 tag, comm, &status);
		
		MPI_Recv(&matrix[j][PROBLEM_SIZE + 2 + 1], PROBLEM_SIZE, MPI_INT, direction[UP],
			 tag, comm, &status);

		MPI_Recv(&matrix[j][lastLine - PROBLEM_SIZE + 1], PROBLEM_SIZE, MPI_INT, direction[DOWN],
			 tag, comm, &status);

		for (n = 1; n < PROBLEM_SIZE + 1; ++n)
		{
			matrix[j][n * (PROBLEM_SIZE+2)] = extra[2][n - 1];
			matrix[j][(n+1) * (PROBLEM_SIZE+2) - 1] = extra[3][n - 1];
		}
		applyRules(&matrix[j][0], &matrix[(j + 1) % 2][0]);
		j = (j + 1) % 2;
		for (l = 1; l < PROBLEM_SIZE + 1; ++l)
		{
			for (n = 1; n < PROBLEM_SIZE + 1; ++n)
			{
				fprintf(file, "%d", matrix[j][l * (PROBLEM_SIZE + 2) +n]);
				
			}
			fprintf(file,"\n");
		}
		
		tag++;
	}

	MPI_Comm_free( &comm );
	MPI_Finalize() ;
	return 0;
}

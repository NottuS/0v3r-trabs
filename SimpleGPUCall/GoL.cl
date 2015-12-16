#define BLOCKSIZE 192

__kernel void cl_initGoL(__global WORD *board, int seed , unsigned int size){
	int gIdx = get_global_id(0);
	if(gIdx < size){
		board[gIdx] = (gIdx * board[gIdx] * seed + gIdx) & 1;
	}
}

__kernel void cl_boarderSolver(__global WORD *iboard, __global WORD *oboard, unsigned int n, unsigned int m, unsigned int stride, unsigned int table){
	int gIdx = get_global_id(0);
	int gIdy = get_global_id(1);
	int lIdx = get_local_id(0);
	/*int i;
	__local localBoard[3][BLOCKSIZE];

	int down;
	int center;
	int up;
	leftint sum;
	int sum2;
	int table2 = table;
	right = lIdx + 1;
	left = lIdx - 1;

	localBoard[0][lIdy] = iboard[gIdx + j];
	localBoard[1][lIdy] = iboard[gIdx + j];
	localBoard[2][lIdy] = iboard[gIdx + j];

	barrier(CLK_LOCAL_MEM_FENCE);*/

	if(gIdx < n && gIdy < m){
		oboard[gIdy * m + gIdx] = iboard[(gIdy - 1) * m + gIdx * BLOCKSIZE - 1] + iboard[(gIdy - 1) * m + gIdx * BLOCKSIZE] + iboard[(gIdy - 1) * m + gIdx* BLOCKSIZE + 1]
								+ iboard[gIdy * m + gIdx * BLOCKSIZE - 1] + iboard[gIdy * m + gIdx + 1]
								+ iboard[(gIdy + 1) * m + gIdx * BLOCKSIZE - 1 ] + iboard[(gIdy + 1) * m + gIdx * BLOCKSIZE] + iboard[(gIdy + 1) * m + gIdx* BLOCKSIZE + 1];
	}
}

//TODO garantir quantidade de mem multipla do # de threads.Sempre aloca n+1 linhas
__kernel void cl_innerGoL(__global WORD *iboard, __global WORD *oboard, unsigned int n, unsigned int m, unsigned int stride, unsigned int table){
	int gIdx = get_global_id(0);
	//int gIdy = get_global_id(1);
	int lIdx = get_local_id(0);
	//int lIdy = get_local_id(1);
	int i, j;

	__local localBoard[4][BLOCKSIZE + 2];
	int down;
	int center;
	int up;
	leftint sum;
	int sum2;
	int table2 = table;
	right = lIdx + 1;
	left = lIdx - 1;

	for(j = 0; j < n; j += stride){
		localBoard[0][lIdx]	= iboard[gIdx + j];
		localBoard[1][lIdx] = iboard[m + gIdx + j];
		localBoard[3][lIdx] = iboard[(n - 1) * m + gIdx + j];
		barrier(CLK_LOCAL_MEM_FENCE);

		sum = localBoard[3][left] + localBoard[3][lIdx] + localBoard[3][right]
			+ localBoard[0][left] + localBoard[0][right] 
			+ localBoard[1][left] + localBoard[1][lIdx] + localBoard[1][right];

		table = (table | 4) & (localBoard[0][lIdx] << 2);
		oboard[m + gIdx] = (table >> sum) & 1;
		
		//verificar soma de extremidades
		//verificar com i = 3 !!!
		for(i = 2; i < m; i+=2){
			localBoard[i & 3][lIdx] = iboard[i * m + gIdx + j];
			localBoard[(i + 1) & 3][lIdx] = iboard[(i + 1) * m + gIdx + j];
			barrier(CLK_LOCAL_MEM_FENCE);

			down = i & 3;
			center = (i - 1) & 3;
			up = (i - 2) & 3;

			sum = localBoard[up][left] + localBoard[up][lIdx] + localBoard[up][right]
				+ localBoard[center][left] + localBoard[center][right]
				+ localBoard[down][left] + localBoard[down][lIdx] + localBoard[down][right];

			up = (i + 1) & 3;
			sum2 = localBoard[center][left] + localBoard[center][lIdx] + localBoard[center][right]
				+ localBoard[down][left] + localBoard[down][right] + 
				+ localBoard[up][left] + localBoard[up][lIdx] + localBoard[up][right];


			table = (table | 4) & (localBoard[center][lIdx] << 2);
			table2 = (table2 | 4) & (localBoard[down][lIdx] << 2);
			oboard[(i - 1) * m + gIdx + j] = (table >> sum) & 1;
			oboard[i * m + gIdx + j] = (table2 >> sum2) & 1;
		}

		down = i - (n & 1);
		center = down + 1;
		up = down + 2;
		localBoard[down][lIdx] = iboard[gIdx + j];
		
		barrier(CLK_LOCAL_MEM_FENCE);

		sum = localBoard[up][left] + localBoard[up][lIdx] + localBoard[up][right]
			+ localBoard[center][left] + localBoard[center][right] 
			+ localBoard[down][left] + localBoard[down][lIdx] + localBoard[down][right];

		table = (table | 4) & (localBoard[0][lIdx] << 2);
		oboard[(n - 1) * m + gIdx + j] = (table >> sum) & 1;
	}
}
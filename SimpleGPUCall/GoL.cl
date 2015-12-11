#define BLOCKSIZE 192

__kernel void cl_initGoL(__global WORD *board, int seed , unsigned int size){
	int gIdx = get_global_id(0);
	if(gIdx < size){
		board[gIdx] = (gIdx * board[gIdx] * seed + gIdx) & 1;
	}
}

__kernel void cl_boarderSolver(__global WORD *iboard, __global WORD *oboard, unsigned int n, unsigned int m){
	int gIdx = get_global_id(0);
	if(gIdx < size){
		board[gIdx] = (gIdx * board[gIdx] * seed + gIdx) & 1;
	}
}

//TODO garantir quantidade de mem multipla do # de threads, percorrendo por linha
__kernel void cl_initGoL(__global WORD *iboard, __global WORD *oboard, unsigned int n, unsigned int m, unsigned int stride, unsigned int table){
	int gIdx = get_global_id(0);
	//int gIdy = get_global_id(1);
	int lIdx = get_local_id(0);
	//int lIdy = get_local_id(1);
	int i, j;

	__local localBoard[4][BLOCKSIZE];

	for(j = 0; j < n; j += stride){
		localBoard[0][lIdx]	= iboard[gIdx + j];
		localBoard[1][lIdx] = iboard[1 * m + gIdx + j];
		localBoard[2][lIdx] = iboard[2 * m + gIdx + j];

		barrier(CLK_LOCAL_MEM_FENCE);

		int sum = 0;
		int sum2 = 0;
		int table2 = table;

		sum = localBoard[0][lIdx - 1] + localBoard[0][lIdx] + localBoard[0][lIdx + 1]
			+ localBoard[1][lIdx - 1] + localBoard[1][lIdx + 1] 
			+ localBoard[2][lIdx - 1] + localBoard[2][lIdx] + localBoard[2][lIdx + 1];

		table = (table | 4) & (localBoard[1][lIdx] << 2);
		oboard[m + gIdx] = (table >> sum) & 1;

		//verificar soma de extremidades
		for(i = 3; i < n; i++){
			localBoard[i & 3][lIdx] = iboard[i * m + gIdx + j];
			barrier(CLK_LOCAL_MEM_FENCE);

			sum = localBoard[(i - 1) & 3][lIdx - 1] + localBoard[(i - 1) & 3][lIdx] + localBoard[(i - 1) & 3][lIdx + 1]
				+ localBoard[i & 3][lIdx - 1] + localBoard[i & 3][lIdx + 1] + 
				+ localBoard[(i + 1) & 3][lIdx - 1] + localBoard[(i + 1) & 3][lIdx] + localBoard[(i + 1) & 3][lIdx + 1];

			sum2 = localBoard[i & 3] [lIdx - 1] + localBoard[i & 3][lIdx] + localBoard[i & 3][lIdx + 1]
				+ localBoard[(i + 1) & 3][lIdx - 1] + localBoard[(i + 1) & 3][lIdx + 1]
				+ localBoard[(i + 2) & 3][lIdx - 1] + localBoard[(i + 2) & 3][lIdx] + localBoard[(i + 2) & 3][lIdx + 1];

			table = (table | 4) & (localBoard[i & 3][lIdx] << 2);
			table2 = (table2 | 4) & (localBoard[(i + 1) & 3][lIdx] << 2);
			oboard[i * m + gIdx + j] = (table >> sum) & 1;
			oboard[(i + 1) * m + gIdx + j] = (table2 >> sum2) & 1;
		}
	}
}
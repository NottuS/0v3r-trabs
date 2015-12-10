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

//TODO garantir quantidade de mem multipla do # de threads, percorrendo por linha, desenvolver: percorrer colunas restantes...
__kernel void cl_initGoL(__global WORD *iboard, __global WORD *oboard, unsigned int n, unsigned int m, unsigned int stride, unsigned int table){
	int gIdx = get_global_id(0);
	//int gIdy = get_global_id(1);
	int lIdx = get_local_id(0);
	//int lIdy = get_local_id(1);
	int i ;

	__local localBoard[4][BLOCKSIZE];

	localBoard[0][lIdx]	= iboard[gIdx];
	localBoard[1][lIdx] = iboard[1 * n + gIdx];
	localBoard[2][lIdx] = iboard[2 * n + gIdx];
	localBoard[3][lIdx] = iboard[3 * n + gIdx];

	barrier(CLK_LOCAL_MEM_FENCE);

	int sum = 0;
	int sum2 = 0;

	//verificar soma de extremidades
	// fazer table da posicao especifica receber ele mesmo
	for(i = 4; i < the end; i++){
		localBoard[i & 4][lIdx] = iboard[i * n + gIdx];
		barrier(CLK_LOCAL_MEM_FENCE);

		sum = localBoard[(i + 1) & 4] [lIdx] + localBoard[(i + 1) & 4] [lIdx + 1]
			+ localBoard[i & 4] [lIdx + 1] + localBoard[(i - 1) & 4] [lIdx + 1]
			+ localBoard[(i - 1) & 4] [lIdx] + localBoard[(i - 1) & 4] [lIdx - 1]
			+ localBoard[i & 4] [lIdx - 1] + localBoard[(i - 1) & 4] [lIdx + 1];

		sum2 = localBoard[(i + 2) & 4] [lIdx] + localBoard[(i + 2) & 4] [lIdx + 1]
			+ localBoard[(i + 1) & 4] [lIdx + 1] + localBoard[i & 4] [lIdx + 1]
			+ localBoard[i & 4] [lIdx] + localBoard[i & 4] [lIdx - 1]
			+ localBoard[(i + 1) & 4] [lIdx - 1] + localBoard[i & 4] [lIdx + 1];

		oboard[(i + 1) * n + gIdx] = (table >> sum) & 1;
		oboard[(i + 2) * n + gIdx] = (table >> sum2) & 1;
	}
	

	
	if(gIdx < n && gIdy < m){
		sum = iboard[(gIdy + 1) * n + gIdx] + iboard[(gIdy + 1) * n + (gIdx + 1)] 
			+ iboard[gIdy * n + (gIdx + 1)] + iboard[(gIdy - 1) * n + (gIdx + 1)] 
			+ iboard[(gIdy - 1) * n + gIdx] + iboard[(gIdy - 1) * n + (gIdx - 1)]
			+ iboard[gIdy * n + (gIdx - 1)] + iboard[(gIdy - 1) * n + (gIdx + 1)];

		oboard[gIdy * n + gIdx] = (table >> sum) & 1;
	}
}
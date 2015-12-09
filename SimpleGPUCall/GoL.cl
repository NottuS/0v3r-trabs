

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

__kernel void cl_initGoL(__global WORD *iboard, __global WORD *oboard, unsigned int n, unsigned int m, unsigned int table){
	int gIdx = get_global_id(0);
	int gIdy = get_global_id(1);

	int sum = 0;
	if(gIdx < n && gIdy < m){
		sum = iboard[(gIdy + 1) * n + gIdx] + iboard[(gIdy + 1) * n + (gIdx + 1)] 
			+ iboard[gIdy * n + (gIdx+1)] + iboard[(gIdy - 1) * n + (gIdx + 1)] 
			+ iboard[(gIdy - 1) * n + gIdx] + iboard[(gIdy - 1) * n + (gIdx -1)]
			+ iboard[gIdy * n + (gIdx - 1)] + iboard[(gIdy - 1) * n + (gIdx + 1)];

		oboard[gIdy * n + gIdx] = (table >> sum) & 1;
	}
}
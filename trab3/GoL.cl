#define BLOCKSIZE 256

__kernel void cl_initGoL(__global int *board, int seed , unsigned int size){
	int gIdx = get_global_id(0);
	if(gIdx < size){
		board[gIdx] = (gIdx * board[gIdx] * seed + gIdx) & 1;
	}
}

__kernel void cl_boarderSolver(__global int *iboard, __global int *oboard, 
	unsigned int n, unsigned int m, unsigned int table){
	int gIdx = get_global_id(0);
	int lIdx = get_local_id(0) + 1;
	int i;
	int sum;
	int sum2;
	int table = hosTtable;
	int table2 = table;
	int left;
	int plusLeft;
	int right;
	int down = lIdx + 1;
	int up = lIdx - 1;

	__local int localBoard[BLOCKSIZE + 2][4];

	for(i = BLOCKSIZE; i < m; i += BLOCKSIZE){
 		plusLeft = i - 2;
 		left = i - 1;
 		right = i + 1;
 		localBoard[lIdx][0] = iboard[gIdx * m + plusLeft];
 		localBoard[lIdx][1] = iboard[gIdx * m + left];
 		localBoard[lIdx][2] = iboard[gIdx * m + i];
 		localBoard[lIdx][3] = iboard[gIdx * m + right];
 		barrier(CLK_LOCAL_MEM_FENCE);
		sum = localBoard[up][0] + localBoard[up][1] + localBoard[up][2]
			+ localBoard[lIdx][0] + localBoard[lIdx][2]
			+ localBoard[down][0] + localBoard[down][1] + localBoard[down][2];

		sum2 = localBoard[up][1] + localBoard[up][2 + localBoard[up][3]
			+ localBoard[lIdx][1] + localBoard[lIdx][3] + 
			+ localBoard[down][1] + localBoard[down][2] + localBoard[down][3];

		table = (table | 4) & (localBoard[lIdx][1] << 2);
		table2 = (table2 | 4) & (localBoard[lIdx][2] << 2);
		oboard[gIdx * m + left] = (table >> sum) & 1;
		oboard[gIdx * m + i] = (table2 >> sum2) & 1;
	}

	localBoard[lIdx][0] = iboard[gIdx * m + n - 2];
	localBoard[lIdx][1] = iboard[gIdx * m + n - 1];
	localBoard[lIdx][2] = iboard[gIdx * m];
	localBoard[lIdx][3] = iboard[gIdx * m + 1];
	barrier(CLK_LOCAL_MEM_FENCE);
	sum = localBoard[up][0] + localBoard[up][1] + localBoard[up][2]
		+ localBoard[lIdx][0] + localBoard[lIdx][2]
		+ localBoard[down][0] + localBoard[down][1] + localBoard[down][2];

	sum2 = localBoard[up][1] + localBoard[up][2] + localBoard[up][3]
		+ localBoard[lIdx][1] + localBoard[lIdx][3] + 
		+ localBoard[down][1] + localBoard[down][2] + localBoard[down][3];

	table = (table | 4) & (localBoard[lIdx][1] << 2);
	table2 = (table2 | 4) & (localBoard[lIdx][2] << 2);
	oboard[gIdx * m + n - 1] = (table >> sum) & 1;
	oboard[gIdx * m] = (table2 >> sum2) & 1;
}

__kernel void cl_innerGoL(__global int *iboard, __global int *oboard, 
	unsigned int n, unsigned int m, unsigned int hosTtable){
	int gIdx = get_global_id(0);
	int lIdx = get_local_id(0) + 1;
	int i;

	__local int localBoard[4][BLOCKSIZE + 2];
	int down;
	int center;
	int up;
	int left;
	int right;
	int sum;
	int sum2;
	int table = hosTtable;
	int table2 = table;
	right = lIdx + 1;
	left = lIdx - 1;

	localBoard[0][lIdx]	= iboard[gIdx];
	localBoard[1][lIdx] = iboard[m + gIdx];
	localBoard[3][lIdx] = iboard[(n - 1) * m + gIdx];
	barrier(CLK_LOCAL_MEM_FENCE);

	sum = localBoard[3][left] + localBoard[3][lIdx] + localBoard[3][right]
		+ localBoard[0][left] + localBoard[0][right] 
		+ localBoard[1][left] + localBoard[1][lIdx] + localBoard[1][right];

	table = (table | 4) & (localBoard[0][lIdx] << 2);
	oboard[m + gIdx] = (table >> sum) & 1;
	
	for(i = 2; i < n; i+=2){
		localBoard[i & 3][lIdx] = iboard[i * m + gIdx];
		localBoard[(i + 1) & 3][lIdx] = iboard[(i + 1) * m + gIdx];
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
		oboard[(i - 1) * m + gIdx] = (table >> sum) & 1;
		oboard[i * m + gIdx] = (table2 >> sum2) & 1;
	}
	down = i - (n & 1);
	center = down + 1;
	up = down + 2;
	localBoard[down][lIdx] = iboard[gIdx];
	
	barrier(CLK_LOCAL_MEM_FENCE);

	sum = localBoard[up][left] + localBoard[up][lIdx] + localBoard[up][right]
		+ localBoard[center][left] + localBoard[center][right] 
		+ localBoard[down][left] + localBoard[down][lIdx] + localBoard[down][right];

	table = (table | 4) & (localBoard[0][lIdx] << 2);
	oboard[(n - 1) * m + gIdx] = (table >> sum) & 1;
}
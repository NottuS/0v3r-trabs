#include "cl/core/clFactory.hpp"
#include "cl/core/clObject.hpp"
#include <sys/time.h>

#define BLOCKSIZE 192

void print_matrix(WORD *matrix, int n, int m);

int run_on_gpu(unsigned int n, unsigned int m, unsigned int cycles){
	clFactory::setDeviceType(RUN_ON_GPU);
	clFactory::startup();
	char deviceName[1024];
	int *board = new int[n * m];
	int table = 8;

	vector<char*> kernelNames;
	kernelNames.push_back((char*)"cl_initGoL");
	kernelNames.push_back((char*)"cl_boarderSolver");
	kernelNames.push_back((char*)"cl_innerGoL");
	startupKernels((char*)"GoL.cl", kernelNames);

	clQueue *queue = getCLQueue();
	cl_command_queue command_queue = queue->getCommandQueue();
	cl_context context = queue->getContext();
	cl_device_id_device = queue->getDevice();
	kernel_t* kernelInitGoL = getKernelInstanceByDevice((char*)"cl_initGoL"
		, device);
	kernel_t* kernelBoarderSolver = getKernelInstanceByDevice((char*)"cl_boarderSolver"
		, device);
	kernel_t* kernelInnerGoL = getKernelInstanceByDevice((char*)"cl_innerGoL"
		, device);

	clFactory::getDeviceName(queue, deviceName, 1024);
	printf("Validating Device [%s - Ptr %p]...\n", deviceName, device);

	int *cl_board[2];
	int *cl_iboard;
	int *cl_oboard;
	CREATE_BUFFER(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
		(m*n) * SIZEOF_WORD, NULL, cl_board[0]);
	CREATE_BUFFER(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
		(m*n) * SIZEOF_WORD, NULL, cl_board[1]);
	int seed = 31;
	int size = n * m;
	int threadsSize = 2880;
	cl_iboard = cl_board[0];
	CALL_KERNEL(command_queue, kernelInitGoL, size, BLOCKSIZE, 3,
		sizeof(WORD *), (void*)&cl_iboard,
		sizeof(cl_int), (void*)&seed,
		sizeof(cl_int), (void*)&size
	);
	//Wait for the kernel to finish.
	SYNC_QUEUE(command_queue);

	for (unsigned int i = 0; i < cycles; ++i) {
		//Alternar ponteiros de leitura e escrita
		cl_iboard = board[i & 1];
		cl_oboard = board[(i + 1) & 1];
		CALL_KERNEL2D(command_queue, kernelBoarderSolver, n, m, BLOCKSIZE, BLOCKSIZE, 6,
			sizeof(int *), (void*)&cl_iboard,
			sizeof(int *), (void*)&cl_oboard,
			sizeof(cl_int), (void*)&n,
			sizeof(cl_int), (void*)&m,
			sizeof(cl_int), (void*)&threadsSize,
			sizeof(cl_int), (void*)&table
		);
		//Wait for the kernel to finish.
		SYNC_QUEUE(command_queue);

		CALL_KERNEL(command_queue, kernel, size, BLOCKSIZE, 6,
			sizeof(int *), (void*)&cl_iboard,
			sizeof(int *), (void*)&cl_oboard,
			sizeof(cl_int), (void*)&n,
			sizeof(cl_int), (void*)&m,
			sizeof(cl_int), (void*)&threadsSize,
			sizeof(cl_int), (void*)&table
		);
		//Wait for the kernel to finish.
		SYNC_QUEUE(command_queue);
		clMemcpyDeviceToHost(command_queue, board, cl_oboard, (m*n) * SIZEOF_WORD);
		print_matrix(board, n, m);
	}
	
	clReleaseMemObject(cl_oboard);
	clReleaseMemObject(cl_iboard);

	//Wait for the kernel to finish.
	SYNC_QUEUE(command_queue);
	clFactory::shutdown();
}

void print_matrix(int *matrix, int n, int m){
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; i < m; ++j)
		{
			printf("%c",matrix[i*m + j] ? 'X' : ' ');
		}
		printf("\n");
	}
}

int main(int argc, char *argv[]) {
	int n, m, cycles;

	n = atoi(argv[1]);
	m = atoi(argv[2]);
	cycles = atoi(argv[3]);

	run_on_gpu(n, m, cycles);
	return 0;
}

double timestamp(){
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return (double)(tp.tv_sec +
	tp.tv_usec / 1000000.0);
}
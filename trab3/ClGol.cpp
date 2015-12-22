#include "ClGol.hpp"

bool ClGol::started = false;
ClGol::~ClGol(){
	omp_set_lock(&mutex_self);
	if(started){
		for(int i=0; i < (int)queues.size(); i++){
			clFactory::disposeQueue(queues[i]);
		}
		started = false;
	}
	omp_unset_lock(&mutex_self);
}

void ClGol::initializeCL(){
	omp_set_lock(&mutex_self);
	if(!started){
		vector<char*> kernelNames;
		kernelNames.push_back((char*)"cl_initGoL");
		kernelNames.push_back((char*)"cl_boarderSolver");
		kernelNames.push_back((char*)"cl_innerGoL");
		startupKernels((char*)"GoL.cl", kernelNames);
		started = true;
	}
	omp_unset_lock(&mutex_self);
}

void ClGol::runGolkernels(unsigned int n, unsigned int m, unsigned int cycles, int printBoard){

	char deviceName[1024];
	int *board = new int[n * m];
	int table = 8;

	clQueue *queue = getCLQueue();
	cl_command_queue command_queue = queue->getCommandQueue();
	cl_context context = queue->getContext();
	cl_device_id device = queue->getDevice();
	kernel_t* kernelInitGoL = getKernelInstanceByDevice((char*)"cl_initGoL"
		, device);
	kernel_t* kernelBoarderSolver = getKernelInstanceByDevice((char*)"cl_boarderSolver"
		, device);
	kernel_t* kernelInnerGoL = getKernelInstanceByDevice((char*)"cl_innerGoL"
		, device);

	bool contains = false;
	for(int i=0; i < (int)queues.size(); i++){
		if(queues[i] == queue){
			contains = true;
			break;
		}
	}
	if(!contains) queues.push_back(queue);

	clFactory::getDeviceName(queue, deviceName, 1024);
	printf("Validating Device [%s - Ptr %p]...\n", deviceName, device);

	cl_mem cl_board[2];
	cl_mem cl_iboard;
	cl_mem cl_oboard;
	CREATE_BUFFER(context, CL_MEM_READ_WRITE, 
		(m*n) * sizeof(int), NULL, cl_board[0]);
	CREATE_BUFFER(context, CL_MEM_READ_WRITE, 
		(m*n) * sizeof(int), NULL, cl_board[1]);
	int seed = 31;
	int size = n * m;
	int threadsSize = 2880;
	cl_iboard = cl_board[0];
	CALL_KERNEL(command_queue, kernelInitGoL, size, BLOCKSIZE, 3,
		sizeof(cl_mem), (void*)&cl_iboard,
		sizeof(cl_int), (void*)&seed,
		sizeof(cl_int), (void*)&size
	);

	printf("%d %d %d %d\n", n,m,size, printBoard);
	//Wait for the kernel to finish.
	SYNC_QUEUE(command_queue);

	double start_t = timestamp();
	for (unsigned int i = 0; i < cycles; ++i) {
		//Alternar ponteiros de leitura e escrita
		cl_iboard = cl_board[i & 1];
		cl_oboard = cl_board[(i + 1) & 1];
		/*CALL_KERNEL2D(command_queue, kernelBoarderSolver, n, m, BLOCKSIZE, BLOCKSIZE, 6,
			sizeof(cl_mem), (void*)&cl_iboard,
			sizeof(cl_mem), (void*)&cl_oboard,
			sizeof(cl_int), (void*)&n,
			sizeof(cl_int), (void*)&m,
			sizeof(cl_int), (void*)&threadsSize,
			sizeof(cl_int), (void*)&table
		);*/
		//Wait for the kernel to finish.
		//SYNC_QUEUE(command_queue);

		CALL_KERNEL(command_queue, kernelInnerGoL, size, BLOCKSIZE, 5,
			sizeof(cl_mem), (void*)&cl_iboard,
			sizeof(cl_mem), (void*)&cl_oboard,
			sizeof(cl_int), (void*)&n,
			sizeof(cl_int), (void*)&m,
			sizeof(cl_int), (void*)&table
		);
		//Wait for the kernel to finish.
		SYNC_QUEUE(command_queue);
		if(printBoard){
			clMemcpyDeviceToHost(command_queue, board, cl_oboard, (m*n) * SIZEOF_WORD);
			print_matrix(board, n, m);
		}
	}
	printf("time: %f\n", timestamp() - start_t);

	clReleaseMemObject(cl_oboard);
	clReleaseMemObject(cl_iboard);
	delete board;
	//Wait for the kernel to finish.
	SYNC_QUEUE(command_queue);
}

void ClGol::print_matrix(int *matrix, int n, int m){
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; i < m; ++j)
		{
			printf("%c",matrix[i*m + j] ? 'X' : ' ');
		}
		printf("\n");
	}
}

void ClGol::runKernel(){
	
}

double ClGol::timestamp(){
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return (double)(tp.tv_sec +
	tp.tv_usec / 1000000.0);
}
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

	int seed = 31;
	
	int threadsSize = 2880;
	int blockSzM = (ceil((m + 0.0)/BLOCKSIZE) * BLOCKSIZE);
	int blockSzN = (ceil((n + 0.0)/BLOCKSIZE) * BLOCKSIZE);
	int size = (blockSzN * blockSzM);

	int *board = new int[size];

	CREATE_BUFFER(context, CL_MEM_READ_WRITE, 
		size * sizeof(int), NULL, cl_board[0]);
	CREATE_BUFFER(context, CL_MEM_READ_WRITE, 
		size * sizeof(int), NULL, cl_board[1]);
	cl_iboard = cl_board[0];
	generateMatrix(size, board, seed);
	printf("%d %d %d %d %d %d\n", n, m,size, printBoard, blockSzM, blockSzN);
	//Wait for the kernel to finish.
	clMemcpyHostToDevice(command_queue, cl_iboard, board, size * sizeof(cl_int));
	if(printBoard){
		print_matrix(board, n, m, blockSzN, blockSzM);
	}

	double start_t = timestamp();
	for (unsigned int i = 0; i < cycles; ++i) {
		//Alternar ponteiros de leitura e escrita
		cl_iboard = cl_board[i & 1];
		cl_oboard = cl_board[(i + 1) & 1];
		CALL_KERNEL(command_queue, kernelInnerGoL, blockSzM, BLOCKSIZE, 5,
			sizeof(cl_mem), (void*)&cl_iboard,
			sizeof(cl_mem), (void*)&cl_oboard,
			sizeof(cl_int), (void*)&n,
			sizeof(cl_int), (void*)&blockSzM,
			sizeof(cl_int), (void*)&table
		);
		//Wait for the kernel to finish.
		SYNC_QUEUE(command_queue);

		CALL_KERNEL(command_queue, kernelBoarderSolver, blockSzN, BLOCKSIZE, 5,
			sizeof(cl_mem), (void*)&cl_iboard,
			sizeof(cl_mem), (void*)&cl_oboard,
			sizeof(cl_int), (void*)&n,
			sizeof(cl_int), (void*)&blockSzM,
			sizeof(cl_int), (void*)&table
		);
		//Wait for the kernel to finish.
		SYNC_QUEUE(command_queue);

		if(printBoard){
			clMemcpyDeviceToHost(command_queue, board, cl_oboard, size * sizeof(cl_int));
			print_matrix(board, n, m, blockSzN, blockSzM);
		}
	}
	printf("time: %f\n", timestamp() - start_t);

	clReleaseMemObject(cl_oboard);
	clReleaseMemObject(cl_iboard);
	delete board;
	//Wait for the kernel to finish.
	SYNC_QUEUE(command_queue);
}

void ClGol::print_matrix(int *matrix, int n, int m, int blockSzN, int blockSzM){
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			printf("%c",matrix[i*blockSzM + j]);
		}
		printf("\n");
	}
	printf("\n");
}

void ClGol::runKernel(){
	
}

double ClGol::timestamp(){
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return (double)(tp.tv_sec +
	tp.tv_usec / 1000000.0);
}

void ClGol::generateMatrix(int n, int *matrix, int rank){
	int i;

	srand(time(NULL) + rank);
	for (i = 0; i < n * n; ++i)	{
		matrix[i] = rand() % 2;
	}
}
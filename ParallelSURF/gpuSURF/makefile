SRC = $(wildcard *.cu)
OBJ = $(SRC:.cu=.o)
NVCC = /usr/local/cuda-6.5/bin/nvcc
CUDACOMP = -gencode arch=compute_20,code=compute_20 -gencode arch=compute_20,code=sm_21
#-G : Generate debug-able device code
#-g : Generate debug-able code
#-O : Generate optimized code
NVCCFLAGS = --ptxas-options=-v -G -g -O0 $(CUDACOMP) -c 
TARGET = gpuSURF.a

.PHONY: all clean
all: $(OBJ) 
	cd lib && $(NVCC) -lib $(OBJ) -lcudadevrt -o $(TARGET)
		
%.o: %.cu
	@mkdir -p lib
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	$(NVCC) $(NVCCFLAGS) $< -o lib/$@  
	@echo 'Finished building: $<'
	@echo ' '

clean:
	@rm -rf  lib *.o *~

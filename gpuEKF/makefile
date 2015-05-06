SRC = $(wildcard *.cu)
OBJ = $(SRC:.cu=.o)
NVCC = /usr/local/cuda/bin/nvcc
CUDACOMP =
LIBS = -lcublas -lcurand -lgsl -lgslcblas
INCLUDES = -I/usr/local/include/gsl
#-G : Generate debug-able device code
#-g : Generate debug-able code
#-O : Generate optimized code
NVCCFLAGS = --ptxas-options=-v -G -g -O0 $(CUDACOMP) -c 
TARGET = gpuekf

.PHONY: all clean
all: $(OBJ)
    $(NVCC) --cudart static --relocatable-device-code=false -link -o $(TARGET) $(OBJ)

%.o: %.cu
	@mkdir -p build
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	$(NVCC) $(INCLUDES) -c $< -o build/$@ $(LIBS)
	@echo 'Finished building: $<'
	@echo ' '

clean:
	@rm -rf  build *.o *~

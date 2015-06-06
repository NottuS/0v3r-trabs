BUILD = ./build
SRCS =  $(wildcard src/*.cu)
OBJS = $(addprefix $(BUILD)/,$(notdir $(SRCS:.cu=.o)))
NVCC = /usr/local/cuda/bin/nvcc
CUDACOMP = -gencode arch=compute_20,code=compute_20 -gencode arch=compute_30,code=compute_30 -gencode arch=compute_20,code=sm_21 -gencode arch=compute_30,code=sm_30
LIBS = -lcublas -lcurand -lcusolver -lgsl -lgslcblas -lm
INCLUDES = -I/usr/local/include/gsl
#-G : Generate debug-able device code
#-g : Generate debug-able code
#-O : Generate optimized code
NVCCFLAGS = -G -g -O0 $(CUDACOMP) -c 

EXEC = gpuekf
vpath %.cu ./src
vpath %.o ./build
.PHONY: all clean
all: $(BUILD) $(BUILD)/$(EXEC)

$(BUILD):
	@mkdir -p $(BUILD)    
    
$(BUILD)/$(EXEC): $(OBJS) 
	$(NVCC) --cudart static -link -o $@ $^ $(LIBS)

$(BUILD)/%.o: src/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	$(NVCC) $(INCLUDES) $(NVCCFLAGS) $< -o $@
	@echo 'Finished building: $<'
	@echo ' '

clean:
	@rm -rf  build *.o *~

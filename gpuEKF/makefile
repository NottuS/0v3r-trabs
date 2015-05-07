BUILD = ./build
SRCS =  $(wildcard src/*.cu)
OBJS = $(addprefix $(BUILD)/,$(notdir $(SRCS:.cu=.o)))
NVCC = /usr/local/cuda/bin/nvcc
CUDACOMP =
LIBS = -lcublas -lcurand -lgsl -lgslcblas -lm
INCLUDES = -I/usr/local/include/gsl
#-G : Generate debug-able device code
#-g : Generate debug-able code
#-O : Generate optimized code
NVCCFLAGS = --ptxas-options=-v -G -g -O0 $(CUDACOMP) -c 

EXEC = gpuekf
vpath %.cu ./src
vpath %.o ./build
.PHONY: all clean
all: $(BUILD) $(BUILD)/$(EXEC)

$(BUILD):
	@mkdir -p $(BUILD)    
    
$(BUILD)/$(EXEC): $(OBJS) 
	$(NVCC) --cudart static --relocatable-device-code=false -link -o $@ $^

$(BUILD)/%.o: src/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	$(NVCC) $(INCLUDES) -c $< -o $@ $(LIBS)
	@echo 'Finished building: $<'
	@echo ' '

clean:
	@rm -rf  build *.o *~

# Specify here the Home paths.
CUDA_HOME=/usr/local/cuda

# Compilation begins.
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXEC=SimpleGPUCall

.PHONY: all clean list

all: $(EXEC)

$(EXEC): $(OBJECTS)
	g++ -o $@ $^ -lm -lgomp -lOpenCL -lcl_framework -L$(CUDA_HOME)/lib64 -L/usr/lib -L$(CUDA_HOME)/samples/common/lib/linux/x86_64 -Llib

%.o: %.cpp
	g++ -c $< -o $@ -Wall -fPIC -O0 -g3 -Wall -c -fmessage-length=0 -I$(CUDA_HOME)/include/ -I/usr/local/cuda-6.5/samples/common/inc -Iocl_framework/include/ -fopenmp

clean:
	rm $(OBJECTS)

list:
	echo $(SOURCES)

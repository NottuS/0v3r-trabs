SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
EXEC = parallelsurf
GPUSURF = $(shell cd ../ && pwd)/gpuSURF
GPUSURFLIB = $(GPUSURF)/lib/*
CC = g++
CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`  -L/usr/local/cuda/lib64 -L/usr/local/cuda/lib -lcuda -lcudart -lcudadevrt -I$(GPUSURF)

.PHONY: all clean mrproper
all: $(GPUSURFLIB) $(EXEC)

$(EXEC): $(OBJ)
	 cd build && $(CC) $(CFLAGS) $(GPUSURFLIB) $^ -o $@ $(LIBS)

$(GPUSURFLIB): 
	cd $(GPUSURF) && make 

%.o: %.cpp
	mkdir -p build
	$(CC) -c $< -o build/$@

clean:
	rm -rf build *.o *~
	
mrproper:
	rm -rf $(EXEC)
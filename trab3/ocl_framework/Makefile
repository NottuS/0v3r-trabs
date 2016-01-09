# Specify here the Home paths.
CUDA_HOME=/usr/local/cuda

# Compilation begins.
SOURCES=$(wildcard *.cpp cl/core/*.cpp gl/core/*.cpp gl/components/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
OUT_LIB=libocl_framework.so

.PHONY: all clean list

all: $(OUT_LIB)

$(OUT_LIB): $(OBJECTS)
	ar rcs $(OUT_LIB) $(OBJECTS) 

%.o: %.cpp
	g++ -c $< -o $@ -Wall -fPIC -O0 -g3 -Wall -c -fmessage-length=0 -I$(CUDA_HOME)/include/ -I/usr/local/cuda-6.5/samples/common/inc -fopenmp

clean:
	rm $(OBJECTS)
	rm $(OUT_LIB)

list:
	echo $(SOURCES)

# Specify here the Home paths.
CUDA_HOME=/usr/local/cuda

# Compilation begins.
SOURCES=$(wildcard ../*.cpp core/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
OUT_LIB=libcl_framework.so

.PHONY: all clean list

all: $(OUT_LIB)

$(OUT_LIB): $(OBJECTS)
	ar rcs $(OUT_LIB) $(OBJECTS) 

%.o: %.cpp
	g++ -c $< -o $@ -Wall -fPIC -O0 -g3 -Wall -c -fmessage-length=0 -I$(CUDA_HOME)/include/ -I/usr/local/cuda-6.5/samples/common/inc -fopenmp

clean:
	rm $(OUT_LIB)
	rm $(OBJECTS)

list:
	echo $(OUT_LIB)
	echo $(SOURCES)


# Build tools
PROGRAMA = tarea2
NVCC = nvcc
CXX = g++
FLAGS = -lfreeimage


# Objectes
CPP = main.cpp
CU = cuda.cu
GPUOBJS = cuda.o 
OBJS = main.o

# make and compile
all: $(PROGRAMA)

$(PROGRAMA): cuda.o cpp.cpp
	$(NVCC) -o $(PROGRAMA) $(GPUOBJS) $(OBJS) $(FLAGS) 

cuda.o: $(CU)
	$(NVCC) -c $(CU)

cpp.cpp: $(CPP)
	$(CXX) -c $(CPP) 

clean: 
	$(RM) $(PROGRAMA) *.o


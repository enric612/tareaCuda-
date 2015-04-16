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

$(PROGRAMA): $(OBJS) $(GPUOBJS)
	$(NVCC) -o $(PROGRAMA) $(GPUOBJS) $(OBJS) $(FLAGS) 

$(GPUOBJS): $(CU)
	$(NVCC) -c $(CU)

$(OBJS): $(CPP)
	$(CXX) -c $(CPP) 

clean: 
	$(RM) $(PROGRAMA) *.o


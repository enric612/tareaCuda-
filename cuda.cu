#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <FreeImage.h>
#include <limits.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>

//Funció per a determinar errors
static void HandleError( cudaError_t err, const char *file, int line)
{
	if (err != cudaSuccess)
	{
		printf( "%s in %s at line %d\n", cudaGetErrorString( err ), file, line );
		exit( EXIT_FAILURE );
	}
}

// MACRO per utilitzar la funció anterior
#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))


__global__ void cal_cuda_grises(BYTE * c_greybits, BYTE * c_greybits_PG, BYTE * c_bits, int width, int scan_width)
{
    int i = blockIdx.x;
    int j = blockIdx.y;

    float r = (float) (c_bits[i * scan_width + j * 4 + 0]);
    float g = (float) (c_bits[i * scan_width + j * 4 + 1]);
    float b = (float) (c_bits[i * scan_width + j * 4 + 2]);
    float valor = 0.2126 * r + 0.7152 *g + 0.0722 * b;
    c_greybits[i * width + j] = (BYTE) (valor);
    c_greybits_PG[i * width + j] = c_greybits[i * width + j];              
}

extern "C" void cuda_grises(BYTE * greybits, BYTE * greybits_PG, BYTE * bits, int N, int width, int scan_width)
{
    BYTE * c_greybits;
    BYTE * c_greybits_PG;
    BYTE * c_bits;
    HANDLE_ERROR(cudaMalloc((void**)&c_greybits, N*sizeof(BYTE)));
    HANDLE_ERROR(cudaMalloc((void**)&c_greybits_PG, N*sizeof(BYTE)));
    HANDLE_ERROR(cudaMalloc((void**)&c_bits, N*sizeof(BYTE)));

    HANDLE_ERROR(cudaMemcpy(c_greybits, greybits, N*sizeof(BYTE), cudaMemcpyHostToDevice));
    HANDLE_ERROR(cudaMemcpy(c_greybits_PG, greybits_PG, N*sizeof(BYTE), cudaMemcpyHostToDevice));
    HANDLE_ERROR(cudaMemcpy(c_bits, bits, N*sizeof(BYTE), cudaMemcpyHostToDevice));
    
    cal_cuda_grises<<<N, 1>>>(c_greybits, c_greybits_PG, c_bits, width, scan_width);

    HANDLE_ERROR(cudaMemcpy(greybits, c_greybits, N*sizeof(BYTE), cudaMemcpyDeviceToHost));
    HANDLE_ERROR(cudaMemcpy(greybits_PG, c_greybits_PG, N*sizeof(BYTE), cudaMemcpyDeviceToHost));
    HANDLE_ERROR(cudaMemcpy(bits, c_bits, N*sizeof(BYTE), cudaMemcpyDeviceToHost));
    
}

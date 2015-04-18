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


__global__ void cal_cuda_grises(BYTE * c_greybits,  BYTE * c_bits, int width, int scan_width, int height)
{
    int i = blockIdx.x*blockDim.x + threadIdx.x;
    int j = blockIdx.y*blockDim.y + threadIdx.y;
    if(i<height && j < width)
    {
        float r = (float) (c_bits[i * scan_width + j * 4 + 0]);
        float g = (float) (c_bits[i * scan_width + j * 4 + 1]);
        float b = (float) (c_bits[i * scan_width + j * 4 + 2]);
        float valor = 0.2126 * r + 0.7152 *g + 0.0722 * b;
        c_greybits[i * width + j] = (BYTE) (valor);
    }
}

extern "C" void cuda_grises(BYTE * greybits, BYTE * greybits_PG, BYTE * bits, int height, int width, int scan_width)
{

    BYTE *c_greybits, *c_bits;

    HANDLE_ERROR(cudaMalloc((void**)&c_bits, height*scan_width));
    HANDLE_ERROR(cudaMalloc((void**)&c_greybits, height*width));

    HANDLE_ERROR(cudaMemcpy(c_bits, bits, height*scan_width, cudaMemcpyHostToDevice));
    

    int x = static_cast<int>(ceilf(static_cast<float>(width) / 32));
    int y = static_cast<int>(ceilf(static_cast<float>(height) / 32));
    const dim3 red(x,y,1);
    const dim3 bloque(32,32,1);
    cal_cuda_grises<<<red, bloque>>>(c_greybits, c_bits, width, scan_width, height);
  
    HANDLE_ERROR(cudaMemcpy(greybits,c_greybits,height*width,cudaMemcpyDeviceToHost));
    
    cudaFree(c_bits);
    cudaFree(c_greybits);

}

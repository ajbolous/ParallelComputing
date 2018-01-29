#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#include "../ocl/ocl_loader.hpp"

#define VECSIZE 1000000
#define WGSIZE 512
#define NUMWORKGROUPS (VECSIZE / WGSIZE + 1)

int main()
{
	OpenClLoader ocl;

	Kernel *kernel = ocl.load_kernel("pccl.cl", "mul2image");

	float vec[VECSIZE];
	float result[NUMWORKGROUPS];

	for(int i =0; i < VECSIZE; i++)
		vec[i] = 1;


	size_t globalWorkSize = VECSIZE;
	size_t localWorkSize = WGSIZE;

	
}


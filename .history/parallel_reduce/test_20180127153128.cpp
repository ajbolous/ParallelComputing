#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#include "../ocl/oclloader.hpp"

#define VECSIZE 1000000
#define WGSIZE 512
#define NUMWORKGROUPS (VECSIZE / WGSIZE + 1)

int main()
{
	OpenClLoader ocl;

	Kernel *kernel = ocl.load_kernel("psr.cl", "parallelSum");

	float vec[VECSIZE];
	float result[NUMWORKGROUPS];

	for(int i =0; i < VECSIZE; i++)
		vec[i] = 1;


	size_t globalWorkSize = VECSIZE;
	size_t localWorkSize = WGSIZE;

	kernel->setArg(0, VECSIZE * sizeof(float), vec);
	kernel->setArg(1, NUMWORKGROUPS * sizeof(float), result);
	kernel->setArg(2, NUMWORKGROUPS * sizeof(float), NULL);

	kernel->execute("ParallelReduction", 1, &globalWorkSize, &localWorkSize);

	kernel->getResult(1, NUMWORKGROUPS * sizeof(float), result);

	
	float sum = 0;
	for (int i = 0; i < NUMWORKGROUPS; i++){
		sum += result[i];
	}

	float sum2 = 0;
	for(int i =0;i<VECSIZE;i++){
		sum2 += vec[i];
	}
	
	printf("%f %f", sum, sum2);
}


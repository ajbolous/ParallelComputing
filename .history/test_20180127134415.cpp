#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "Loader.hpp"

#define VECSIZE 16
#define WGSIZE 2
#define NUMWORKGROUPS 8

int main()
{
	OpenClLoader ocl;

	Kernel *kernel = ocl.load_kernel("psr.cl", "parallelSum");

	double vec[VECSIZE];
	double result[NUMWORKGROUPS];

	for(double i =0; i < VECSIZE; i++)
		vec[(int)i] = i;


	size_t globalWorkSize = VECSIZE;
	size_t localWorkSize = WGSIZE;

	kernel->setArg(0, VECSIZE * sizeof(double), vec);
	kernel->setArg(1, NUMWORKGROUPS * sizeof(double), result);
	kernel->setArg(2, NUMWORKGROUPS * sizeof(double), NULL);

	kernel->execute("ParallelReduction", 1, &globalWorkSize, &localWorkSize);

	kernel->getResult(1, NUMWORKGROUPS * sizeof(double), result);

	double sum = 0;
	for (int i = 0; i < NUMWORKGROUPS; i++){
		sum += result[i];
	}

	double sum2 = 0;
	for(int i =0;i<VECSIZE;i++){
		sum2 += vec[i];
	}

	printf("%lf %lf", sum, sum2);
}

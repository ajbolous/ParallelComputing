#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "Loader.hpp"

#define VECSIZE 1000000
#define WGSIZE 1000
#define NUMWORKGROUPS VECSIZE/WGSIZE

int main()
{

	OpenClLoader ocl;

	Kernel *kernel = ocl.load_kernel("psr.cl", "parallelSum");



	double vec[VECSIZE];
	double result[NUMWORKGROUPS];

	for(int i =0; i < VECSIZE; i++)
		vec[i] = i;


	size_t localWorkSize = WGSIZE, globalWorkSize = VECSIZE;

	kernel->setArg(0, VECSIZE * sizeof(double), vec);
	kernel->setArg(1, NUMWORKGROUPS * sizeof(double), result);
	kernel->setArg(2, NUMWORKGROUPS * sizeof(double), NULL);

	kernel->execute("ParallelReduction", 1, &globalWorkSize, &localWorkSize);

	kernel->getResult(1, NUMWORKGROUPS * sizeof(double), result);

	double sum = 0;
	for (int i = 0; i < WGSIZE; i++){
		sum += result[i];
	}
	double sum2 = 0;
	for(int i =0;i<VECSIZE;i++){
		sum2 += vec[i];
	}
	printf("%lf %lf", sum, sum2);
}

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "Loader.hpp"

int main()
{

	OpenClLoader ocl;

	Kernel *kernel = ocl.load_kernel("psr.cl", "parallelSum");

	double vec[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	double presults[4];
	double lresults[4];

	size_t localWorkSize[1] = {1}, globalWorkSize[1] = {16};

	kernel->setArg(0, 16 * sizeof(double), vec);
	kernel->setArg(1, 4 * sizeof(double), presults);
	kernel->setArg(2, 4*sizeof(double), NULL);
	
	kernel->execute("Hello.cl/hello", 1, globalWorkSize, localWorkSize);

	kernel->getResult(1,4*sizeof(double), presults);
	double sum = 0;
	for (int i = 0; i < 4; i++)
		sum += presults[i];
	printf("%lf", sum);
}

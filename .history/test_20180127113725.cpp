#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "Loader.hpp"

int main()
{

	OpenClLoader ocl;

	Kernel *kernel = ocl.load_kernel("psr.cl", "parallelSum");

	double vec[5] = {1,2,3,4,5};
	double* result[100];

	size_t localWorkSize[2] = {16, 16}, globalWorkSize[2] = {32, 32};

	kernel->execute("Hello.cl/hello", globalWorkSize, localWorkSize, result, 0, 1, memob);

	printf("%s", result);
}

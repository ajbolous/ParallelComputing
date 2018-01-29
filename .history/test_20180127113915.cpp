#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "Loader.hpp"

int main()
{

	OpenClLoader ocl;

	Kernel *kernel = ocl.load_kernel("psr.cl", "parallelSum");

	double vec[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	double presults[4];
	double lresults[4];
	
	size_t localWorkSize[2] = {4, 0}, globalWorkSize[2] = {4, 0};

	kernel->execute("Hello.cl/hello", globalWorkSize, localWorkSize, presult, 0, 3, vec, presults, lresults);

	printf("%s", result);
}

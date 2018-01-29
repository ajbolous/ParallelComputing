#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "Loader.hpp"

int main()
{

	OpenClLoader ocl;

	Kernel *kernel = ocl.load_kernel("hello.cl", "hello");

	char memob[100] = "hello";
	char result[100];
	kernel->setArg(0, 100, memob);
	size_t localWorkSize[2] = {16,16}, size_t globalWorkSize[2]= {32,32};

	kernel->execute("Hello.cl/hello", result);
}

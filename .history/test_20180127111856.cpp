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

	size_t localWorkSize[2] = {16,16}, globalWorkSize[2]= {32,32};

	kernel->execute("Hello.cl/hello", globalWorkSize, localWorkSize, 1, 0,memob);

	printf("%s the result is ", memob);
}

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>


int main()
{

	OpenClLoader ocl;

	ocl.init();
	Kernel *kernel = ocl.load_kernel("hello.cl", "hello");

	char memob[100] = "hello";
	char result[100];
	kernel->setArg(0, 100, memob);
	kernel->execute(result);
	printf("After kernel %s\n", result);
	ocl.clean();
}

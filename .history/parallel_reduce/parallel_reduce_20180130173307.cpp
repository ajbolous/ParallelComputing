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

	float vec[VECSIZE];
	float result[NUMWORKGROUPS];

	for (int i = 0; i < VECSIZE; i++)
		vec[i] = 1;

	size_t globalWorkSize = VECSIZE;
	size_t localWorkSize = WGSIZE;

	Kernel *kernel = ocl.load_kernel("psr.cl", "parallelSum", 1);

	auto start = std::chrono::high_resolution_clock::now();

	kernel->setBufferArg(0, VECSIZE * sizeof(float), vec);
	kernel->setBufferArg(1, NUMWORKGROUPS * sizeof(float), result);
	kernel->setLocalArg(2, NUMWORKGROUPS * sizeof(float));

	kernel->execute("ParallelReduction", 1, &globalWorkSize, &localWorkSize);

	kernel->getResult(1, NUMWORKGROUPS * sizeof(float), result);

	float sum = 0;
	for (int i = 0; i < NUMWORKGROUPS; i++)
	{
		sum += result[i];
	}

	auto finish = std::chrono::high_resolution_clock::now();
	auto d = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start);
	printf("\nDone executing ParallelReduce Elapsed Time: %f second", d / 1000.0 / 1000.0 / 1000.0);

	start = std::chrono::high_resolution_clock::now();

	float sum2 = 0;
	for (int i = 0; i < VECSIZE; i++)
	{
		sum2 += vec[i];
	}

	finish = std::chrono::high_resolution_clock::now();
	d = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start);
	printf("\nDone executing Serial Sum Elapsed Time: %f second", d / 1000.0 / 1000.0 / 1000.0);

	printf("\nResults : Parallel: %f Serial: %f", sum, sum2);
}

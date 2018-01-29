#ifndef OCLKERNEL

#define OCLKERNEL

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include<string>
#include <chrono>

class Kernel
{
	cl_context _context;
	cl_kernel _kernel;
	cl_command_queue _queue;
	cl_program _program;
	cl_mem _mem[10];

  public:
	Kernel(cl_context context, cl_command_queue queue, cl_program program, cl_kernel kernel)
	{
		_queue = queue;
		_context = context;
		_program = program;
		_kernel = kernel;
	}

	void setArg(int index, int size, void *data)
	{
		int ret;
		_mem[index] = clCreateBuffer(_context, CL_MEM_READ_WRITE, size, data, &ret);

		if (ret != CL_SUCCESS)
			printf("Error allocating memory CODE: %d\n", ret);

		clSetKernelArg(_kernel, index, sizeof(cl_mem), (void *)&_mem);
	}

	void execute(void *result)
	{

          auto start = std::chrono::high_resolution_clock::now();

    // operation to be timed ...

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration_cast<std::chrono::nanoseconds> duration = (finish-start)

		int ret = clEnqueueTask(_queue, _kernel, 0, NULL, NULL);
		getResult(0, sizeof(_mem[0]), result);
        t = clock() - t;
		printf("Executed Kernel , Result code: %d, Time elapsed: %f seconds\n", ret, t/CLOCKS_PER_SEC);
	}

	void getResult(int index, int size, void *result)
	{
		clEnqueueReadBuffer(_queue, _mem[index], CL_TRUE, 0, size, result, 0, NULL, NULL);
	}

	~Kernel()
	{
		clReleaseKernel(_kernel);
		clReleaseProgram(_program);
		for (int i = 0; i < 10; i++)
			if (_mem[i] != NULL)
				clReleaseMemObject(_mem[i]);
	}
};

#endif
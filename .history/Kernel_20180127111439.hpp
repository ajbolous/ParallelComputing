#ifndef OCLKERNEL

#define OCLKERNEL

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <string>
#include <chrono>
#include <stdarg.h>

class Kernel
{
    cl_context _context;
    cl_kernel _kernel;
    cl_command_queue _queue;
    cl_program _program;
    cl_mem _mem[10];

    int outArgsIndex = NULL;

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

    void execute(char *name, size_t globalWorkSize[], size_t localWorkSize[], int argCount, int outArg, ...)
    {

        va_list Numbers;

        va_start(Numbers, argCount);
        int Sum = 0;
        for (int i = 0; i < argCount; ++i)
            Sum += va_arg(Numbers, int);
        va_end(Numbers);

        auto start = std::chrono::high_resolution_clock::now();

        // operation to be timed ...

        globalWorkSize[0] = ((globalWorkSize[0] - 1) / localWorkSize[0] + 1) * localWorkSize[0];
        globalWorkSize[1] = ((globalWorkSize[1] - 1) / localWorkSize[1] + 1) * localWorkSize[1];

        printf("# Executing Kernel %s, global: ( %d,%d ) local: ( %d, %d )\n", name, globalWorkSize[0], globalWorkSize[1], localWorkSize[0], localWorkSize[1]);

        int ret = clEnqueueNDRangeKernel(_queue, _kernel, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);

        auto finish = std::chrono::high_resolution_clock::now();
        auto d = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start);

        printf("# Executed Kernel %s, Return code: %d, Time elapsed: %d Nano Seconds\n", name, ret, d);

        getResult(0, sizeof(_mem[0]), result);
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
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

        if (data == NULL)
        {
            printf("#Setting LocalArg \n\t--index:%d\n\t--size:%d\n\t--Ptr: %d\n", index, size, data);

            ret = clSetKernelArg(_kernel, index, size, NULL);
            if (ret != CL_SUCCESS)
                printf("Error allocating memory CODE: %d\n", ret);

            return;
        }
        printf("#Setting LocalArg \n\t--index:%d\n\t--size:%d\n\t--Ptr: %d\n", index, size, data);
        _mem[index] = clCreateBuffer(_context, CL_MEM_READ_WRITE, size, NULL, &ret);
        clEnqueueWriteBuffer(_queue, _mem[index], CL_TRUE, 0, size, data, 0, NULL, NULL);

        if (ret != CL_SUCCESS)
            printf("Error allocating memory CODE: %d\n", ret);

        clSetKernelArg(_kernel, index, sizeof(cl_mem), (void *)&_mem[index]);
    }

    void execute(char *name, int dim, size_t *globalWorkSize, size_t *localWorkSize)
    {

        if (dim >= 1 && localWorkSize[0] != 0)
            globalWorkSize[0] = ((globalWorkSize[0] - 1) / localWorkSize[0] + 1) * localWorkSize[0];
        if (dim > 1 && localWorkSize[1] != 0)
            globalWorkSize[1] = ((globalWorkSize[1] - 1) / localWorkSize[1] + 1) * localWorkSize[1];

        printf("\n-----------------------------------------------\n");

        printf("# Executing Kernel %s\n", name);

        auto start = std::chrono::high_resolution_clock::now();

        int ret = clEnqueueNDRangeKernel(_queue, _kernel, dim, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);

        auto finish = std::chrono::high_resolution_clock::now();
        auto d = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start);

        printf("# Executed Kernel %s\n\t--Return code: %d \n\t--Time elapsed: %d Nano Seconds\n", name, ret, d);
        printf("-----------------------------------------------\n");
    }

    void getResult(int argIndex, int size, void *result)
    {
        int ret;
        ret = clEnqueueReadBuffer(_queue, _mem[argIndex], CL_TRUE, 0, size, result, 0, NULL, NULL);
        if (ret != CL_SUCCESS)
            printf("Error reading mem buffer Code: %d\n", ret);
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
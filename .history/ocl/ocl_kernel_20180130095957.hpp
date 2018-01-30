#ifndef OCLKERNEL

#define OCLKERNEL

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <string>
#include <chrono>

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

    cl_mem allocateMemBuffer(int size, void *data, int flags)
    {
        int ret;
        printf("#Allocating memory buffer -size:%d -Ptr: %d\n", size, data);
        if (flags == NULL)
            flags = CL_MEM_READ_WRITE;
        cl_mem mem = clCreateBuffer(_context, flags, size, NULL, &ret);
        clEnqueueWriteBuffer(_queue, mem, CL_TRUE, 0, size, data, 0, NULL, NULL);

        if (ret != CL_SUCCESS)
        {
            printf("\t-Error allocating memory CODE: %d\n", ret);
            return NULL;
        }
        return mem;
    }

    void setArg(int index, int size, void *data)
    {
        printf("# Setting Kernel Arg -index:%d -size:%d -Ptr: %d\n", index, size, data);
        clSetKernelArg(_kernel, index, sizeof(cl_mem), (void *)&_mem[index]);
    }

    void setBufferArg(int index, int size, void *data)
    {
        printf("#Setting Memory Buffer Arg -index:%d -size:%d -Ptr: %d\n", index, size, data);

        cl_mem mem = allocateMemBuffer(size, data, NULL);
        if (mem == NULL)
            return;

        _mem[index] = mem;
        setArg(index, sizeof(cl_mem), (void *)&mem);
    }

    void setLocalArg(int index, int size)
    {
        int ret;
        printf("# Setting LocalArg\n  -index:%d\n  -size:%d\n", index, size);
        ret = clSetKernelArg(_kernel, index, size, NULL);
        if (ret != CL_SUCCESS)
            printf("  -Error setting argument CODE: %d\n", ret);
    }

    void execute(char *name, int dim, size_t *globalWorkSize, size_t *localWorkSize)
    {

        if (dim >= 1 && localWorkSize[0] != 0)
            globalWorkSize[0] = ((globalWorkSize[0] - 1) / localWorkSize[0] + 1) * localWorkSize[0];
        if (dim > 1 && localWorkSize[1] != 0)
            globalWorkSize[1] = ((globalWorkSize[1] - 1) / localWorkSize[1] + 1) * localWorkSize[1];

        printf("# Executing Kernel %s\n", name);

        auto start = std::chrono::high_resolution_clock::now();

        int ret = clEnqueueNDRangeKernel(_queue, _kernel, dim, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
        if (ret != CL_SUCCESS)
            printf("  -Error executing Kernel %s Code: %d\n", name, ret);
        else
        {
            auto finish = std::chrono::high_resolution_clock::now();
            auto d = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start);

            printf("  +Done executing Kernel %s \n    -Return code: %d \n    -Time elapsed: %d Nano Seconds\n", name, ret, d);
        }
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
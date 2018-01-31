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
    int _isDebug = 0;
    int outArgsIndex = NULL;

  public:
    Kernel(cl_context context, cl_command_queue queue, cl_program program, cl_kernel kernel, int isDebug)
    {
        _queue = queue;
        _context = context;
        _program = program;
        _kernel = kernel;
        _isDebug = isDebug;
    }

    cl_mem allocateMemBuffer(int size, void *data, int flags)
    {
        int ret;
        if (_isDebug)
            printf("# Allocating memory buffer  [size: %d, Ptr: %d ]\n", size, data);
        if (flags == NULL)
            flags = CL_MEM_READ_WRITE;
        cl_mem mem = clCreateBuffer(_context, flags, size, NULL, &ret);
        clEnqueueWriteBuffer(_queue, mem, CL_TRUE, 0, size, data, 0, NULL, NULL);

        if (_isDebug && ret != CL_SUCCESS)
        {
            printf("  - Error allocating memory CODE: %d\n", ret);
            return NULL;
        }
        return mem;
    }

    void setArg(int index, int size, void *data)
    {
        if (_isDebug)

            printf("# Setting Kernel Arg  [ index: %d, size: %d, Ptr: %d ]\n", index, size, data);
        clSetKernelArg(_kernel, index, size, data);
    }

    // Sets a buffer memory and allocates the argument to the kernel
    void setBufferArg(int index, int size, void *data)
    {
        if (_isDebug)
            printf("# Setting Memory Buffer Arg  [ index: %d, size: %d, Ptr: %d ]\n", index, size, data);

        //allocate memory buffer
        cl_mem mem = allocateMemBuffer(size, data, NULL);
        if (mem == NULL)
            return;

        // Set arg from mem object
        _mem[index] = mem;
        clSetKernelArg(_kernel, index, sizeof(cl_mem), (void *)&mem);
    }

    // sets a local kernel argument, Without copying data
    void setLocalArg(int index, int size)
    {
        int ret;
        if (_isDebug)
            printf("# Setting LocalArg  [ index:%d, size:%d ]\n", index, size);
        ret = clSetKernelArg(_kernel, index, size, NULL);
        if (ret != CL_SUCCESS)
            printf("\n  - Error setting argument CODE: %d\n", ret);
    }

    // executes the kernel code
    void execute(char *name, int dim, size_t *globalWorkSize, size_t *localWorkSize)
    {

        //Round up the local and global work size
        if (dim >= 1 && localWorkSize[0] != 0)
            globalWorkSize[0] = ((globalWorkSize[0] - 1) / localWorkSize[0] + 1) * localWorkSize[0];
        if (dim > 1 && localWorkSize[1] != 0)
            globalWorkSize[1] = ((globalWorkSize[1] - 1) / localWorkSize[1] + 1) * localWorkSize[1];

        if (_isDebug)
            printf("\n# Executing Kernel %s  [ dims: %d %d %d %d %d ]", name, dim, globalWorkSize[0], globalWorkSize[1], localWorkSize[0], localWorkSize[1]);

        //Actual executiion of the kernel
        int ret = clEnqueueNDRangeKernel(_queue, _kernel, dim, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);

        if (_isDebug && ret != CL_SUCCESS)
            printf("\n  - Error executing Kernel %s Code: %d\n", name, ret);
        else if (_isDebug)
            printf("\n  + Done executing Kernel %s  [ code: %d ]\n", name, ret);
    }

    void getResult(int argIndex, int size, void *result)
    {
        int ret;
        ret = clEnqueueReadBuffer(_queue, _mem[argIndex], CL_TRUE, 0, size, result, 0, NULL, NULL);
        if (_isDebug && ret != CL_SUCCESS)
            printf("\n  -Error reading mem buffer Code: %d\n", ret);
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
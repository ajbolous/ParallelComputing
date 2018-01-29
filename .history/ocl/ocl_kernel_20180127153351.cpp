#include "ocl_kernel.hpp"

void Kernel::setArg(int index, int size, void *data)
{
    int ret;

    if (data == NULL)
    {
        printf("#Setting LocalArg -index:%d -size:%d -Ptr: %d\n", index, size, data);

        ret = clSetKernelArg(_kernel, index, size, NULL);
        if (ret != CL_SUCCESS)
            printf("Error allocating memory CODE: %d\n", ret);

        return;
    }
    printf("#Setting LocalArg -index:%d -size:%d -Ptr: %d\n", index, size, data);
    _mem[index] = clCreateBuffer(_context, CL_MEM_READ_WRITE, size, NULL, &ret);
    clEnqueueWriteBuffer(_queue, _mem[index], CL_TRUE, 0, size, data, 0, NULL, NULL);

    if (ret != CL_SUCCESS)
        printf("Error allocating memory CODE: %d\n", ret);

    clSetKernelArg(_kernel, index, sizeof(cl_mem), (void *)&_mem[index]);
}

void Kernel::execute(char *name, int dim, size_t *globalWorkSize, size_t *localWorkSize)
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

    printf("# Executed Kernel %s -Return code: %d -Time elapsed: %d Nano Seconds\n", name, ret, d);
    printf("-----------------------------------------------\n");
}

void Kernel::getResult(int argIndex, int size, void *result)
{
    int ret;
    ret = clEnqueueReadBuffer(_queue, _mem[argIndex], CL_TRUE, 0, size, result, 0, NULL, NULL);
    if (ret != CL_SUCCESS)
        printf("Error reading mem buffer Code: %d\n", ret);
}
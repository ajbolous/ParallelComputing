#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <vector>

class Kernel
{
	cl_context _context;
	cl_kernel _kernel;
	OpenClLoader *_ocl;
	cl_mem _mem[10] = {NULL};

	void setArg(int index, int size, void *data)
	{
		cl_mem mem = clCreateBuffer(_context, CL_MEM_READ_WRITE, size, data, NULL);
		_mem[index] = mem;
		clSetKernelArg(_kernel, index, sizeof(cl_mem), (void *)&mem);
	}

	void execute()
	{
		clEnqueueTask(_ocl->getQueue(), _kernel, 0, NULL, NULL);
	}
};

class OpenClLoader
{
	cl_context context = NULL;
	cl_command_queue commandQueue = NULL;
	cl_device_id deviceId = NULL;

  public:
	int init()
	{
		cl_platform_id platform_id = NULL;

		cl_uint ret_num_devices;
		cl_uint ret_num_platforms;

		clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
		clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &deviceId, &ret_num_devices);
		/* Create OpenCL context */
		context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, NULL);
		/* Create Command Queue */
		commandQueue = clCreateCommandQueue(context, deviceId, 0, NULL);
		/* Create Memory Buffer */
	}

	cl_mem allocate_memory(int size, void *data)
	{
		if (context == NULL)
			return 0;
		cl_mem mem = clCreateBuffer(context, CL_MEM_READ_WRITE, size, data, NULL);
		return mem;
	}

	cl_kernel load_kernel(char *filename)
	{
		char *source_str;
		size_t source_size;
		FILE *fp;

		/* Load the source code containing the kernel*/
		fp = fopen(filename, "r");
		if (!fp)
		{
			fprintf(stderr, "Failed to load kernel.\n");
			return 0;
		}
		source_str = (char *)malloc(10000 * sizeof(char));
		source_size = fread(source_str, 1, 10000 * sizeof(char), fp);
		fclose(fp);

		/* Create Kernel Program from the source */
		cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, NULL);
		free(source_str);
		/* Build Kernel Program */
		clBuildProgram(program, 1, &deviceId, NULL, NULL, NULL);
		/* Create OpenCL Kernel */
		kernel = clCreateKernel(program, filename, NULL);
		return kernel;

		/* Execute OpenCL Kernel */
	}
	cl_command_queue getQueue()
	{
		return commandQueue;
	}

	int read_memory(cl_mem source_memory, int size, void *result)
	{
		/* Copy results from the memory buffer */
		clEnqueueReadBuffer(commandQueue, source_memory, CL_TRUE, 0, size, result, 0, NULL, NULL);
	}

	int clean()
	{
		clFlush(commandQueue);
		clFinish(commandQueue);
		clReleaseKernel(kernel);
		clReleaseProgram(program);
		clReleaseMemObject(allocatedMemroy);
		clReleaseCommandQueue(commandQueue);
		clReleaseContext(context);
	}
};

int main()
{

	OpenClLoader ocl;

	ocl.init();
	ocl.load_kernel("hello.cl");
}
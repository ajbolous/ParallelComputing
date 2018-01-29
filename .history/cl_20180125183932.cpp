#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <vector>


class OpenClLoader
{
	cl_context context = NULL;
	cl_command_queue commandQueue = NULL;
	cl_mem allocatedMemroy = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;
	std::vector<cl_mem> allocatedMemroy;
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
		context = clCreateContext(NULL, 1, &device_id, NULL, NULL, NULL);
		/* Create Command Queue */
		commandQueue = clCreateCommandQueue(context, device_id, 0, NULL);
		/* Create Memory Buffer */
	
	}

	cl_mem allocate_memory(int size)
	{
		if (context == NULL)
			return 0;

		cl_mem mem = clCreateBuffer(context, CL_MEM_READ_WRITE, size, NULL, NULL);
		allocatedMemroy.push_back(mem);
		return mem;
	}

	int load_kernel(char *filename, deviceId)
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
		source_str = (char *)malloc(MAX_SOURCE_SIZE);
		source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
		fclose(fp);

		/* Create Kernel Program from the source */
		program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
											(const size_t *)&source_size, &ret);

		/* Build Kernel Program */
		ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

		/* Create OpenCL Kernel */
		kernel = clCreateKernel(program, "hello", &ret);

		/* Set OpenCL Kernel Parameters */
		ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&allocatedMemroy);

		/* Execute OpenCL Kernel */
		ret = clEnqueueTask(commandQueue, kernel, 0, NULL, NULL);

		char string[MEM_SIZE];

		/* Copy results from the memory buffer */
		ret = clEnqueueReadBuffer(commandQueue, memobj, CL_TRUE, 0,
								  MEM_SIZE * sizeof(char), string, 0, NULL, NULL);
		puts(string);
		free(source_str);
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
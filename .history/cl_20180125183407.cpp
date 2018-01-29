#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

class OpenClLoader
{
	cl_context context = NULL;
	cl_command_queue commandQueue = NULL;
	cl_mem allocatedMemroy = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;

  public:
	int init()
	{
		cl_platform_id platform_id = NULL;

		cl_device_id device_id = NULL;
		cl_uint ret_num_devices;
		cl_uint ret_num_platforms;

		clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
		clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

		/* Create OpenCL context */
		context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

		/* Create Command Queue */
		command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
		/* Create Memory Buffer */
	}

	int allocate_memory(int size)
	{
		if (context == NULL)
			return 0;

		allocatedMemroy = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof(char), NULL, 0);
	}
	int load_kernel(char *filename)
	{
		char *source_str;
		size_t source_size;
		FILE *fp;

		/* Load the source code containing the kernel*/
		fp = fopen(filename, "r");
		if (!fp)
		{
			fprintf(stderr, "Failed to load kernel.\n");
			exit(1);
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
		ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);

		/* Execute OpenCL Kernel */
		ret = clEnqueueTask(command_queue, kernel, 0, NULL, NULL);

		char string[MEM_SIZE];

		/* Copy results from the memory buffer */
		ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0,
								  MEM_SIZE * sizeof(char), string, 0, NULL, NULL);
		puts(string);
		free(source_str);
	}

	int clean()
	{
		ret = clFlush(command_queue);
		ret = clFinish(command_queue);
		ret = clReleaseKernel(kernel);
		ret = clReleaseProgram(program);
		ret = clReleaseMemObject(memobj);
		ret = clReleaseCommandQueue(command_queue);
		ret = clReleaseContext(context);
	}
};

int main()
{

	OpenClLoader ocl;

	ocl.init();
	ocl.load_kernel("hello.cl");
}
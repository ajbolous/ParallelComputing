#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <vector>

class Kernel{
	cl_kernel _kernel;

	cl_mem _mem[5];

	void* operator [](int index){

	}

	void operator ()(){
		 
	}
public:

	Kernel(char* filename, int params){

	}
};



class OpenClLoader
{
	cl_context context = NULL;
	cl_command_queue commandQueue = NULL;
	cl_mem allocatedMemroy = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;
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

	cl_mem allocate_memory(int size)
	{
		if (context == NULL)
			return 0;
		cl_mem mem = clCreateBuffer(context, CL_MEM_READ_WRITE, size, NULL, NULL);
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
		program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, NULL);
		free(source_str);
		/* Build Kernel Program */
		clBuildProgram(program, 1, &deviceId, NULL, NULL, NULL);
		/* Create OpenCL Kernel */
		kernel = clCreateKernel(program, filename, NULL);
		return kernel;

		/* Execute OpenCL Kernel */
	}

	int set_kernel_arg(cl_kernel kernel,int index, void* mem)
	{ /* Set OpenCL Kernel Parameters */

		clSetKernelArg(kernel, index, sizeof(cl_mem), (void *)&mem);
	}

	int read_memory(cl_mem source_memory, int size, void *result)
	{
		/* Copy results from the memory buffer */
		clEnqueueReadBuffer(commandQueue, source_memory, CL_TRUE, 0, size, result, 0, NULL, NULL);
	}

	int execute_kernel(cl_kernel kernel)
	{
		clEnqueueTask(commandQueue, kernel, 0, NULL, NULL);
	}

	int invoke_kernel(char* filename){
		cl_kernel kernel = load_kernel(filename);
		execute_kernel(kernel);
	}

	int invoke_kernel_with_args(char* filename){
		cl_kernel kernel = load_kernel(filename);

	}

	int load_memory()
	{

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
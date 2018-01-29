#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <vector>

class Kernel
{
	cl_context _context;
	cl_kernel _kernel;
	cl_command_queue _queue;
	cl_program _program;

	cl_mem _mem;

  public:
	Kernel(cl_context context, cl_command_queue queue, cl_program program, cl_kernel kernel)
	{
		_queue = queue;
		_context = context;
		_program = program;
		_kernel = kernel;
	}

	void setArg(int index, int size)
	{
		int ret;
		_mem = clCreateBuffer(_context, CL_MEM_READ_WRITE, size, NULL, &ret);

		printf("Allocated memory %d %d", ret);
		clSetKernelArg(_kernel, index, sizeof(cl_mem), (void *)&_mem);
	}

	void execute()
	{
		int ret = clEnqueueTask(_queue, _kernel, 0, NULL, NULL);
		printf("%d",ret);
	}

	void getResult(int index, int size, void *result)
	{
		clEnqueueReadBuffer(_queue, _mem, CL_TRUE, 0, size, result, 0, NULL, NULL);
	}

	~Kernel()
	{
		clReleaseKernel(_kernel);
		clReleaseProgram(_program);
		clReleaseMemObject(_mem);
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

		printf("Initialilzing OpenCL Platform\n");
		printf("Number of Platforms: %d, Number of Devices: %d\n", (int)ret_num_platforms, (int)ret_num_devices);
		char deviceName[100];
		clGetDeviceInfo(deviceId, CL_DEVICE_NAME, sizeof(deviceName), deviceName, NULL);
		printf("Device: %s\n", deviceName);
	}

	Kernel *load_kernel(char *filename)
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
		cl_kernel kernel = clCreateKernel(program, filename, NULL);
		return new Kernel(context, commandQueue, program, kernel);
	}

	int clean()
	{
		clFlush(commandQueue);
		clFinish(commandQueue);

		clReleaseCommandQueue(commandQueue);
		clReleaseContext(context);
	}
};

int main()
{

	OpenClLoader ocl;

	ocl.init();
	Kernel *kernel = ocl.load_kernel("hello.cl");

	char memob[100]="hello";
	kernel->setArg(0, 100);
	kernel->execute();
	kernel->getResult(0, 100, memob);

	printf("After kernel %s", memob);
	ocl.clean();
}
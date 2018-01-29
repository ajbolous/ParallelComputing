#ifndef OCLLOADER
#define OCLLOADER

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "Kernel.hpp"

class OpenClLoader
{
	cl_context context = NULL;
	cl_command_queue commandQueue = NULL;
	cl_device_id deviceId = NULL;

  public:
	OpenClLoader()
	{
		cl_platform_id platform_id = NULL;
		char deviceName[100];

		cl_uint ret_num_devices;
		cl_uint ret_num_platforms;

		clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
		clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &deviceId, &ret_num_devices);
		/* Create OpenCL context */
		context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, NULL);
		/* Create Command Queue */
		commandQueue = clCreateCommandQueue(context, deviceId, 0, NULL);
		clGetDeviceInfo(deviceId, CL_DEVICE_NAME, sizeof(deviceName), deviceName, NULL);

		printf("Info - Initialilzing OpenCL Platform\n");
		printf("Info - Number of Platforms: %d, Number of Devices: %d\n", (int)ret_num_platforms, (int)ret_num_devices);
		printf("Info - Using Device: %s\n", deviceName);
	}

	Kernel *load_kernel(char *fileName, char *kernelName)
	{
		int ret;
		char *source_str;
		size_t source_size;
		FILE *fp;

		/* Load the source code containing the kernel*/
		fp = fopen(fileName, "r");
		if (!fp)
		{
			fprintf(stderr, "Error[00] - Failed to load kernel.\n");
            return NULL;
		}

		source_str = (char *)malloc(10000 * sizeof(char));
		source_size = fread(source_str, 1, 10000 * sizeof(char), fp);
		fclose(fp);

		/* Create Kernel Program from the source */
		cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

		if (ret != CL_SUCCESS){
			printf("Error[%d] - Creating program from source\n", ret);
            return NULL;
        }

		/* Build Kernel Program */
		ret = clBuildProgram(program, 1, &deviceId, NULL, NULL, NULL);

		if (ret != CL_SUCCESS){
			printf("Error[%d]- Building program from source\n", ret);
            return NULL;
        }
		/* Create OpenCL Kernel */
		cl_kernel kernel = clCreateKernel(program, kernelName, &ret);

		if (ret != CL_SUCCESS){
			printf("Error[%d] - Creating kernel from source\n", ret);
            return NULL;
        }

		free(source_str);

		return new Kernel(context, commandQueue, program, kernel);
	}

    ~OpenClLoader(){
			clFlush(commandQueue);
		clFinish(commandQueue);
		clReleaseCommandQueue(commandQueue);
		clReleaseContext(context);
        printf("Info - Cleaned all OpenCL resources");
    }
};

#endif
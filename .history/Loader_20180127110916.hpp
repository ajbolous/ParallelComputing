#ifndef OCLLOADER
#define OCLLOADER

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <string>

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

		printf("--------------------------------------------------\n");
		printf("Initialilzing OpenCL Platform\n");
		printf("Number of Platforms: %d, Number of Devices: %d\n", (int)ret_num_platforms, (int)ret_num_devices);
		printf("Using Device: %s\n", deviceName);
		printf("--------------------------------------------------\n\n");
	}

	Kernel *load_kernel(char *fileName, char *kernelName)
	{
	
	}

	~OpenClLoader()
	{
		clFlush(commandQueue);
		clFinish(commandQueue);
		clReleaseCommandQueue(commandQueue);
		clReleaseContext(context);
		printf("\n\n-------------------------------");
		printf("\nCleaned all OpenCL resources\n");
		printf("-------------------------------\n");
	}
};

#endif
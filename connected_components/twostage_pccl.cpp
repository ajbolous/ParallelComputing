#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#include "../ocl/ocl_loader.hpp"

#define WIDTH 1920
#define HEIGHT 1080

void linearTwoStage(char **image, int width, int height)
{
	char counter = 10;
	char labels[255] = {0};

	for (int i = 0; i < height, i++)
		for (int j = 0; j < width; j++)
		{
			if (image[i][j] == 1)
			{
				int label = 0;

				char upn = 0;
				char ln = 0;
				if ((i - 1) > 0)
					upn = image[i - 1][j];
				if ((j - 1) > 0)
					ln = image[i][j - 1];

				if (upn == 0 && ln == 0)
				{
					counter++;
					labels[counter] = counter;
					image[i][j] = counter;
				}
				else if (upn == 0)
				{
					image[i][j] = ln;
				}
				else if (ln == 0)
					image[i][j] = upn;
			}
		}
}

int main()
{
	OpenClLoader ocl;

	Kernel *kernel = ocl.load_kernel("pccl.cl", "twoStagePccl");

	char image[HEIGHT][WIDTH];

	size_t globalWorkSize[2] = {WIDTH, HEIGHT};
	size_t localWorkSize[2] = {20, 20};

	int changed = 1;
	int width = WIDTH;
	int height = HEIGHT;

	int *changedPtr = &changed;

	kernel->setArg(0, WIDTH * HEIGHT * sizeof(char), image);
	kernel->setArg(1, sizeof(int *), &changedPtr);
	kernel->setArg(2, NUMWORKGROUPS * sizeof(float), NULL);

	while (changed)
	{
		changed = 0;
		kernel->execute();
	}
}

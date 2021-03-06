#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <opencv2/opencv.hpp>

#include "../ocl/ocl_loader.hpp"

#define WIDTH 1920
#define HEIGHT 1080

void linearTwoStage(uchar *image, int width, int height)
{
	int counter = 1;
	char labels[255] = {0};
	int index = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			index = i * width + height;

			if (image[index] >= 255)
			{
				int label = 0;

				char upn = 0;
				char ln = 0;

				if ((i - 1) > 0)
					upn = image[(i - 1) * width + j];
				if ((j - 1) > 0)
					ln = image[i * width + j - 1];

				if (upn == 0 && ln == 0)
				{
					counter++;
					labels[counter] = counter;
					image[index] = counter;
					printf("pixel at %d %d is %d\n", i, j, counter);
				}
				else if (upn == 0)
					image[index] = ln;
				else if (ln == 0)
					image[index] = upn;
				else
				{
					image[index] = upn > ln ? ln : upn;
					labels[upn] = ln;
				}
			}
		}
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			index = i * width + j;
			if (image[index] != 0)
			{
				image[index] = labels[image[index]];
			}
		}
	}
}

/*
	1. read image
	2. convert into binary and shape 1920x1080
	3. run the sequential algorithm and time it
	4. run the parallel algorithm and time it
	5. output results

	* Maybe we do a live video feed with euclid ...
*/
cv::Mat loadImage()
{
	cv::Mat src = cv::imread("test.jpg", 1); // load source image

	cv::Size size(WIDTH, HEIGHT); //the dst image size,e.g.100x100
	cv::resize(src, src, size);   //resize image

	cv::Mat gray;
	cv::cvtColor(src, gray, CV_BGR2GRAY); //perform gray scale conversion.
	cv::threshold(gray, gray, 100, 255, cv::THRESH_BINARY);

	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", gray);							// Show our image inside it.

	cv::waitKey(0);
	return gray;
}

int main()
{
	OpenClLoader ocl;

	Kernel *kernel = ocl.load_kernel("pccl.cl", "twoStagePccl");

	// char image[HEIGHT][WIDTH];

	// size_t globalWorkSize[2] = {WIDTH, HEIGHT};
	// size_t localWorkSize[2] = {20, 20};

	// int changed = 1;
	// int width = WIDTH;
	// int height = HEIGHT;

	// int *changedPtr = &changed;

	// kernel->setArg(0, WIDTH * HEIGHT * sizeof(char), image);
	// kernel->setArg(1, sizeof(int *), &changedPtr);

	// while (changed)
	// {
	// 	changed = 0;
	// 	kernel->execute("ParallelCCL", 2, globalWorkSize, localWorkSize);
	// }

	cv::Mat image = loadImage();

	linearTwoStage(image.data, WIDTH, HEIGHT);
	cv::applyColorMap(image, image, cv::COLORMAP_JET);

	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", image);						// Show our image inside it.

	cv::waitKey(0);
}

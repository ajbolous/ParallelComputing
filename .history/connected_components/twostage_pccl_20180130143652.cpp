#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <opencv2/opencv.hpp>

#include "../ocl/ocl_loader.hpp"

#define WIDTH 1920
#define HEIGHT 1080

void linearTwoStage(uchar *image, int width, int height)
{
	printf("width %d, height %d", width, height);
	int *labeledImage = (int *)malloc(sizeof(int) * width * height);
	int *labels = (int *)malloc(sizeof(int) * width * height);
	int counter = 1;

	int index = 0;
	for (int i = 0; i < width * height; i++)
		labels[i] = i;


	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			index = i * width + j;
			labeledImage[index] = (int)image[index];

			if (labeledImage[index] >= 255)
			{
				int label = 0;
				int upn = 0;
				int ln = 0;

				if ((i - 1) >= 0)
					upn = labeledImage[(i - 1) * width + j];
				if ((j - 1) >= 0)
					ln = labeledImage[i * width + j - 1];

				if (upn == 0 && ln == 0)
				{
					counter++;
					labeledImage[index] = counter;
				}
				else if (upn == 0)
					labeledImage[index] = ln;
				else if (ln == 0)
					labeledImage[index] = upn;
				else
				{
					if (ln < upn)
					{

						labeledImage[index] = ln;
						labels[upn] =labels[ln];
					}
					else
					{
						labeledImage[index] = upn;
						labels[ln] = labels[upn];
					}
				}
			}
		}
	}
	printf("height: %d, width %d", height, width);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			index = i * width + j;

			if (image[index] != 0)
			{
				image[index] = labels[labeledImage[index]]*100;
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
cv::Mat loadImage(char* filename)
{
	cv::Mat src = cv::imread("test3.jpg", 1); // load source image

	cv::Size size(WIDTH, HEIGHT);  //the dst image size,e.g.100x100
	cv::resize(src, src, size); //resize image

	cv::Mat gray;
	cv::cvtColor(src, gray, CV_BGR2GRAY); //perform gray scale conversion.
	cv::threshold(gray, gray, 127, 255, cv::THRESH_BINARY);

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

	linearTwoStage(image.data, 1920, 1080);

	cv::applyColorMap(image, image, cv::COLORMAP_JET);

	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", image);						// Show our image inside it.

	cv::waitKey(0);
}

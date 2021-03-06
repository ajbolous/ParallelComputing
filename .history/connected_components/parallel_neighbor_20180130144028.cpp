#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <opencv2/opencv.hpp>

#include "../ocl/ocl_loader.hpp"

#define WIDTH 1920
#define HEIGHT 1080

cv::Mat loadImage(char* filename)
{
	cv::Mat src = cv::imread(filename, 1); // load source image

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

void displayLabeledImage(cv::Mat image){
	cv::applyColorMap(image, image, cv::COLORMAP_JET);
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", image);						// Show our image inside it.
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

	cv::Mat image = loadImage("test.jpg");






	displayLabeledImage(image);
	cv::waitKey(0);
}

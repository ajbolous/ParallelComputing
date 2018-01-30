#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <opencv2/opencv.hpp>
#include <chrono>

#include "../ocl/ocl_loader.hpp"

#define WIDTH 1920
#define HEIGHT 1080

cv::Mat loadImage(char *filename, int display)
{
    cv::Mat src = cv::imread(filename, 1); // load source image
    if (display)
    {
        cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
        imshow("Display window", src);                          // Show our image inside it.
        cv::waitKey(0);
    }
    cv::Size size(WIDTH, HEIGHT); //the dst image size,e.g.100x100
    cv::resize(src, src, size);   //resize image

    cv::Mat gray;
    cv::cvtColor(src, gray, CV_BGR2GRAY); //perform gray scale conversion.
    cv::threshold(gray, gray, 127, 255, cv::THRESH_BINARY);

    return gray;
}

void displayLabeledImage(cv::Mat image)
{
    cv::applyColorMap(image, image, cv::COLORMAP_JET);
    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
    cv::imshow("Display window", image);                    // Show our image inside it.
    cv::waitKey(0);
}

int main()
{
    OpenClLoader ocl;

    cv::Mat image = loadImage("test3.jpg", 1);

    size_t globalWorkSize[2] = {WIDTH, HEIGHT};
    size_t localWorkSize[2] = {30, 30};

    int changed = 10000;
    int width = WIDTH;
    int height = HEIGHT;

    int *changedPtr = &changed;

    auto start = std::chrono::high_resolution_clock::now();

    Kernel *fkernel = ocl.load_kernel("pccl.cl", "firstRunPccl", 0);
    fkernel->setBufferArg(0, WIDTH * HEIGHT * sizeof(uchar), image.data);
    fkernel->setArg(1, sizeof(int), &width);
    fkernel->setArg(2, sizeof(int), &height);
    fkernel->execute("FirstKernel", 2, globalWorkSize, localWorkSize);
    fkernel->getResult(0, sizeof(uchar) * WIDTH * HEIGHT, image.data);

    Kernel *kernel = ocl.load_kernel("pccl.cl", "twoStagePccl", 0);
    kernel->setBufferArg(0, WIDTH * HEIGHT * sizeof(uchar), image.data);
    kernel->setBufferArg(1, sizeof(int), &changed);
    kernel->setArg(2, sizeof(int), &width);
    kernel->setArg(3, sizeof(int), &height);

    int i = 0;
    for (i = 0; i < 4; i++)
    {
        i++;
        changed = 0;
        kernel->setBufferArg(1, sizeof(int), &changed);
        kernel->execute("ParallelCCL", 2, globalWorkSize, localWorkSize);
        kernel->getResult(1, sizeof(int), &changed);
        // printf("Round %d : Changed cells count %d\n", i, changed);
    }

    kernel->getResult(0, sizeof(uchar) * WIDTH * HEIGHT, image.data);

    auto finish = std::chrono::high_resolution_clock::now();
    auto d = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start);
    printf("\nDone executing PCCL Elapsed Time: %d nsec ]\n", d);

    displayLabeledImage(image);
}

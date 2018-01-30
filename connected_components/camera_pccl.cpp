#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <opencv2/opencv.hpp>
#include <chrono>

#include "../ocl/ocl_loader.hpp"

int main(int argc, char **argv)
{

    cv::VideoCapture cap(0); // open the default camera
    if (!cap.isOpened())     // check if we succeeded
        return -1;

    cv::Mat image;

    cap >> image;

    OpenClLoader ocl;

    size_t globalWorkSize[2] = {1, 1};
    size_t localWorkSize[2] = {16, 12};

    globalWorkSize[0] = image.size[1];
    globalWorkSize[1] = image.size[0];

    printf("\nimage size %d %d", image.size[0], image.size[1]);
    int width = image.size[1];
    int height = image.size[0];

    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.

    while (1)
    {
        auto start = std::chrono::high_resolution_clock::now();

        cap >> image;
        int changed = 10000;
        int *changedPtr = &changed;

        cv::cvtColor(image, image, CV_BGR2GRAY); //perform gray scale conversion.
        cv::threshold(image, image, 80, 255, cv::THRESH_BINARY);

        Kernel *fkernel = ocl.load_kernel("pccl.cl", "firstRunPccl", 0);
        fkernel->setBufferArg(0, width * height * sizeof(uchar), image.data);
        fkernel->setArg(1, sizeof(int), &width);
        fkernel->setArg(2, sizeof(int), &height);
        fkernel->execute("FirstKernel", 2, globalWorkSize, localWorkSize);
        fkernel->getResult(0, sizeof(uchar) * width * height, image.data);

        Kernel *kernel = ocl.load_kernel("pccl.cl", "twoStagePccl", 0);
        kernel->setBufferArg(0, width * height * sizeof(uchar), image.data);
        kernel->setBufferArg(1, sizeof(int), &changed);
        kernel->setArg(2, sizeof(int), &width);
        kernel->setArg(3, sizeof(int), &height);

        int i = 0;

        while (changed > 0)
        {
            i++;
            changed = 0;
            kernel->setBufferArg(1, sizeof(int), &changed);
            kernel->execute("ParallelCCL", 2, globalWorkSize, localWorkSize);
            kernel->getResult(1, sizeof(int), &changed);
            //printf("Round %d : Changed cells count %d\n", i, changed);
        }

        kernel->getResult(0, sizeof(uchar) * width * height, image.data);

        cv::applyColorMap(image, image, cv::COLORMAP_JET);

        auto finish = std::chrono::high_resolution_clock::now();
        auto d = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start);
        printf("\nFrame rate %f",  d/1000.0/1000.0/1000.0);

        cv::imshow("Display window", image); // Show our image inside it.
        cv::waitKey(50);
    }
}

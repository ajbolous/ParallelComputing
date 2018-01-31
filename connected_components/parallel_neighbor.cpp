#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <opencv2/opencv.hpp>
#include <chrono>

#include "../ocl/ocl_loader.hpp"

cv::Mat loadImage(char *filename, int thresh, int scale, int display)
{
    cv::Mat src = cv::imread(filename, 1); // load source image
    if (display)
    {
        cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
        imshow("Display window", src);                          // Show our image inside it.
        cv::waitKey(0);
    }

    if (scale)
    {
        cv::Size size(1920, 1080);  //the dst image size,e.g.100x100
        cv::resize(src, src, size); //resize image
    }
    cv::Mat gray;
    cv::cvtColor(src, gray, CV_BGR2GRAY); //perform gray scale conversion.
    cv::threshold(gray, gray, thresh, 255, cv::THRESH_BINARY);

    return gray;
}

void displayLabeledImage(cv::Mat image)
{
    cv::applyColorMap(image, image, cv::COLORMAP_JET);
    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
    cv::imshow("Display window", image);                    // Show our image inside it.
    cv::waitKey(0);
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("Please specify all arguments [Image, threshold, scale]\n");
        return 0;
    }

    cv::Mat image = loadImage(argv[1], atoi(argv[2]), atoi(argv[3]), 1);

    OpenClLoader ocl;

    size_t globalWorkSize[2] = {1, 1};
    size_t localWorkSize[2] = {16, 9};

    globalWorkSize[0] = image.size[1];
    globalWorkSize[1] = image.size[0];

    printf("image size %d %d", image.size[0], image.size[1]);
    int changed = 10000;
    int width = image.size[1];
    int height = image.size[0];

    int *changedPtr = &changed;

    auto start = std::chrono::high_resolution_clock::now();

    Kernel *fkernel = ocl.load_kernel("pccl.cl", "firstRunPccl", 1);
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
        // printf("Round %d : Changed cells count %d\n", i, changed);
    }

    kernel->getResult(0, sizeof(uchar) * width * height, image.data);

    auto finish = std::chrono::high_resolution_clock::now();
    auto d = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start);
    printf("\nDone executing PCCL Elapsed Time: %f second\n", d / 1000.0 / 1000.0 / 1000.0);

    cv::imwrite("pnpout.jpg", image);
    displayLabeledImage(image);
}

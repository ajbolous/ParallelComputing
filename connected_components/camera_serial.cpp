#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <opencv2/opencv.hpp>
#include <chrono>

#include "../ocl/ocl_loader.hpp"

void linearTwoStage(uchar *image, int width, int height)
{
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
                        labels[upn] = labels[ln];
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
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            index = i * width + j;

            if (image[index] != 0)
            {
                image[index] = labels[labeledImage[index]] * 2;
            }
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("\nPlease provide threshold argument");
        return 0;
    }

    cv::VideoCapture cap(0); // open the default camera
    if (!cap.isOpened())     // check if we succeeded
        return -1;

    cv::Mat image;

    cap >> image;

    // image size is 640x480
    size_t globalWorkSize[2] = {1, 1};
    size_t localWorkSize[2] = {24, 18};

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

        cv::cvtColor(image, image, CV_BGR2GRAY); //perform gray scale conversion.
        cv::threshold(image, image, atoi(argv[1]), 255, cv::THRESH_BINARY);


        linearTwoStage(image.data, width, height);
        cv::applyColorMap(image, image, cv::COLORMAP_JET);

        auto finish = std::chrono::high_resolution_clock::now();
        auto d = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start);
        printf("\nTime elapsed %f ", d / 1000.0 / 1000.0 / 1000.0);

        cv::imshow("Display window", image); // Show our image inside it.
        cv::waitKey(20);
    }
}

#include "environment.h"

void createBorders(cv::Mat& img)
{
    for (int i = 0; i < img.rows; i++)
        img.at<uchar>(i,0) = 1;
    for (int i = 0; i < img.rows; i++)
        img.at<uchar>(i, img.cols - 1) = 1;
    for (int j = 0; j < img.cols; j++)
        img.at<uchar>(0, j) = 1;
    for (int j = 0; j < img.cols; j++)
        img.at<uchar>(img.rows - 1, j) = 1;
}

void clearEnvironment(cv::Mat& img)
{
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
            if (img.at<uchar>(i,j) > 3)
                img.at<uchar>(i,j) = 0;
}

void createEnvironment(cv::Mat& img)
{
    createBorders(img);
}

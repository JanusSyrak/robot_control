#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <opencv2/opencv.hpp>


void createBorders(cv::Mat& img);
void createEnvironment(cv::Mat& img);
void clearEnvironment(cv::Mat& img);

#endif // ENVIRONMENT_H

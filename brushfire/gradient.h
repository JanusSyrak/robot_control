#ifndef GRADIENT_H
#define GRADIENT_H
#include <opencv2/opencv.hpp>

cv::Mat calculate_gradient(cv::Mat& img);
void drawGradient(cv::Mat& img);

#endif // GRADIENT_H

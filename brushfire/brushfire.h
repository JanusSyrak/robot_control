#ifndef BRUSHFIRE_H
#define BRUSHFIRE_H
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>



bool img_cmp(cv::Mat& a, cv::Mat& b);

void threshold(cv::Mat& img, int threshold);

int find_largest_pixel(cv::Mat& img);

int find_smallest_pixel(cv::Mat& img);

void scale(cv::Mat& img, double a);



void brushfire(cv::Mat& img);

void display_image(cv::Mat& img, std::string title);

#endif // BRUSHFIRE_H

#ifndef WAVEFRONT_H
#define WAVEFRONT_H
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <ctime>
#include <chrono>
#include <map>
#include <random>
#include <cmath>
#define pb push_back

void prepareWavefront(cv::Mat& img);
int count(std::vector<int> v, int x);
void wavefront(cv::Mat& img, int goal_x, int goal_y);
void randomWavefront(cv::Mat& img);
void postWavefront(cv::Mat& img);
void drawWavefront(cv::Mat& img, int start_x, int start_y);
double distance(int start_x, int start_y, int goal_x, int goal_y);

#endif // WAVEFRONT_H

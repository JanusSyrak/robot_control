#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H
#include <opencv2/opencv.hpp>
#include <random>

cv::Mat generateMap(int rows, int cols, int goal_x, int goal_y);
cv::Mat generateRandomMap(int rows, int cols);

#endif // MAPGENERATOR_H

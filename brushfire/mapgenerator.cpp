#include "mapgenerator.h"


cv::Mat generateMap(int rows, int cols, int goal_x, int goal_y)
{
    cv::Mat img(rows, cols, CV_8UC1, cv::Scalar(0));
    img.at<uchar>(goal_y, goal_x) = 2;
    return img;
}

cv::Mat generateRandomMap(int rows, int cols)
{
    std::random_device rd;
    cv::Mat img(rows, cols, CV_8UC1, cv::Scalar(0));
    int goal_x = rd() % cols;
    int goal_y = rd() % rows;
    img.at<uchar>(goal_y, goal_x) = 2;
    return img;
}

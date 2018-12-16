#include "brushfire.h"

void brushfire(cv::Mat& img)
{
    cv::Mat new_img;
    cv::Mat img_hold = img.clone();
    do {
        img = img_hold;
        new_img = img.clone();
        for (int i = 1; i < new_img.rows - 1; i++)
        {
            for (int j = 1; j < new_img.cols - 1; j++)
            {
                int smallest_neighbor = 255;
                for (int c = i - 1; c < i + 2; c++)
                {
                    for (int d = j - 1; d < j + 2; d++)
                    {
                        if (!(c == i && d == j))
                        {
                            if (new_img.at<uchar>(c,d) < smallest_neighbor)
                            {
                                smallest_neighbor = new_img.at<uchar>(c,d);
                            }
                        }
                    }
                }
                if (smallest_neighbor < 255)
                    if (new_img.at<uchar>(i,j) > 0)
                        new_img.at<uchar>(i, j) = smallest_neighbor + 1;
            }
        }
        img_hold = new_img;
        } while (!(img_cmp(img, new_img)));
}


bool img_cmp(cv::Mat& a, cv::Mat& b)
{
    if (a.rows != b.rows || a.cols != b.cols)
        return 0;
    for (int i = 0; i < a.rows; i++)
        for (int j = 0; j < b.rows; j++)
            if (a.at<uchar>(i,j) != b.at<uchar>(i,j))
                return 0;
    return 1;
}

void threshold(cv::Mat& img, int threshold)
{
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
        {
            if (img.at<uchar>(i,j) < threshold)
                img.at<uchar>(i,j) = 0;
            if (img.at<uchar>(i,j) >= threshold)
                img.at<uchar>(i,j) = 255;
        }
}

int find_largest_pixel(cv::Mat& img)
{
    int largest = 0;
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
            if (img.at<uchar>(i,j) > largest)
                largest = img.at<uchar>(i,j);
    return largest;
}

int find_smallest_pixel(cv::Mat& img)
{
    int smallest = 0;
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
            if (img.at<uchar>(i,j) > smallest)
                smallest = img.at<uchar>(i,j);
    return smallest;
}

void scale(cv::Mat& img, double a)
{
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
            img.at<uchar>(i,j) *= a;
}

void display_image(cv::Mat &img, std::string windowTitle)
{
    cv::namedWindow(windowTitle, cv::WINDOW_AUTOSIZE);
    cv::imshow(windowTitle, img);

    cv::waitKey(0);
}

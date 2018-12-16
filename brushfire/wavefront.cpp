#include "wavefront.h"
#include "brushfire.h"

int count(std::vector<int> v, int x)
{
    int count = 0;
    for (int i = 0; i < v.size(); i++)
        if (v[i] == x)
            count++;
    return count;
}

void wavefront(cv::Mat& img, int goal_x, int goal_y)
{
    std::vector<int> L;

    img.at<uchar>(goal_x, goal_y) = 2;

    for (int i = goal_x - 1; i < goal_x + 2; i++)
        for (int j = goal_y - 1; j < goal_y + 2; j++)
            if (i < img.cols && j < img.rows && !(i == goal_x && j == goal_y) && img.at<uchar>(i,j) == 0)
                L.pb(i + j * img.cols);

    while (L.size() > 0)
    {
        int largest_neighbor = INT_MAX;
        int key = L[0];
        int x = key % img.cols;
        int y = key / img.cols;
        for (int i = x - 1; i < x + 2; i++)
        {
            for (int j = y - 1; j < y + 2; j++)
            {
                if (!(i == x && j == y) && (i < img.rows) && (j < img.cols) && (i >= 0) && (j >= 0))
                {
                    if (img.at<uchar>(i,j) > 1 && img.at<uchar>(i,j) < largest_neighbor)
                    {
                        largest_neighbor = img.at<uchar>(i,j);
                    }
                    if (img.at<uchar>(i,j) == 0 && (count(L, i + j*img.cols) == 0))
                    {
                        L.pb(i + j * img.cols);
                    }
                }
            }
        }
        img.at<uchar>(x, y) = largest_neighbor + 1;
        L.erase(L.begin());
    }
}

void wavefront(cv::Mat &img)
{
    std::vector<int> L;
    int goal_x = 0;
    int goal_y = 0;

    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
            if (img.at<uchar>(i, j) == 2)
            {
                goal_x = j;
                goal_y = i;
            }

    for (int i = goal_x - 1; i < goal_x + 2; i++)
        for (int j = goal_y - 1; j < goal_y + 2; j++)
            if (i < img.cols && j < img.rows && !(i == goal_x && j == goal_y) && img.at<uchar>(i,j) == 0)
                L.pb(i + j * img.cols);

    while (L.size() > 0)
    {
        int largest_neighbor = INT_MAX;
        int key = L[0];
        int x = key % img.cols;
        int y = key / img.cols;
        for (int i = x - 1; i < x + 2; i++)
        {
            for (int j = y - 1; j < y + 2; j++)
            {
                if (!(i == x && j == y) && (i < img.rows) && (j < img.cols) && (i >= 0) && (j >= 0))
                {
                    if (img.at<uchar>(i,j) > 1 && img.at<uchar>(i,j) < largest_neighbor)
                    {
                        largest_neighbor = img.at<uchar>(i,j);
                    }
                    if (img.at<uchar>(i,j) == 0 && (count(L, i + j*img.cols) == 0))
                    {
                        L.pb(i + j * img.cols);
                    }
                }
            }
        }
        img.at<uchar>(x, y) = largest_neighbor + 1;
        L.erase(L.begin());
    }
}

void prepareWavefront(cv::Mat& img)
{
    threshold(img, 2);
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
        {
            if (img.at<uchar>(i,j) == 0)
                img.at<uchar>(i,j) = 1;
            else if (img.at<uchar>(i,j) == 255)
                img.at<uchar>(i,j) = 0;
        }
}

void postWavefront(cv::Mat& img)
{

}

void drawWavefront(cv::Mat&img, int x, int y)
{
    int goal_x, goal_y;
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
        {
            if (img.at<uchar>(i,j) == 2)
            {
                goal_x = j;
                goal_y = i;
            }
        }
    int next_x = x;
    int next_y = y;
    std::random_device rd;
    while (img.at<uchar>(y, x) == 1)
    {
        x += rd() % 1;
        y += rd() % 1;
    }
    int value = img.at<uchar>(y,x);
    //for (int john = 0; john < 20; john++)
    while (value != 2)
    {
        int min_distance = 100000;
        for (int a = y - 1; a < y + 2; a++)
            for (int b = x - 1; b < x + 2; b++)
            {
                if (a >= 0 && b >= 0 && a < img.rows && b < img.cols && !(a == y && b == x))
                {
                    if (img.at<uchar>(a, b) + 1 == value)
                    {
                        if (distance(b, a, goal_x, goal_y) < min_distance)
                        {
                            min_distance = distance(b, a, goal_x, goal_y);
                            next_x = b;
                            next_y = a;
                        }
                    }
                }
            }
        img.at<uchar>(next_y, next_x) = 255;
        x = next_x;
        y = next_y;
        //value = img.at<uchar>(y, x);
        value--;
    }
}

double distance(int start_x, int start_y, int goal_x, int goal_y)
{
    return sqrt(pow((goal_y - start_y),2) + pow((goal_x - start_x),2));
}

/* getMarbleCenter(camera_image)
{
 for every column in image:
    for every pixel in column:
      if (saturation_level(pixel) > threshold)
          //counter++;
  return column with largest counter
}
*/

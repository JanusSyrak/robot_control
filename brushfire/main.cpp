#include "wavefront.h"
#include "brushfire.h"
#include "gradient.h"
#include "mapgenerator.h"

int main()
{
    cv::Mat img;
    //img = cv::imread("/home/syrak/Pictures/middle.png", 0);
    img = cv::imread("/home/syrak/Pictures/large_occlusion2.png", 0);
    //std::cout << img << std::endl;
    display_image(img, "Big World Map");

    cv::Mat new_img = img.clone();
    prepareWavefront(new_img);

    //std::cout << new_img << std::endl;
    wavefront(new_img, 20, 20);
    //scale(new_img, 255/find_largest_pixel(new_img));
    drawWavefront(new_img, 150, 100);
    //cv::imwrite("/home/syrak/Pictures/large_occlusion3_grent.png", new_img);

    display_image(new_img, "Wavefront");

    return 0;
}


#include <gazebo/gazebo_client.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/transport/transport.hh>

#include <opencv2/opencv.hpp>

#include "fuzzybugcontroller.h"
#include "laserscanner.h"
//#include "marbledetection.h"

#include <iostream>
#include <vector>

using namespace cv;

LaserScanner scan;

FuzzyBugController fbc(&scan);

static boost::mutex mutex;

int marbleAngle = 0;

bool isMarble(cv::Mat& img)
{
    // Used in tests to see if there was a marble in the image

   for (int i = 0; i < img.cols; i++)
   {
       for (int j = 0; j < img.rows; j++)
       {
           if ((int)img.at<cv::Vec3b>(j,i)[1] > 240)
           {
               return true;
           }
       }
   }

    return false;


}

void makeHistogram(cv::Mat& pt)
{
    // Used in tests. Histograms were generated to visualize saturation values in HSV images
    // The saturation value will appear on channel 1 with green color (g_hist)



    if (!pt.data)
        return;

    int histSize = 256;

    std::vector<cv::Mat> bgr_planes;
    split(pt, bgr_planes);

    float range[] = {0, 256};
    const float* histRange = { range };

    bool uniform = true;
    bool accumulate = false;

    cv::Mat b_hist, g_hist, r_hist;

    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize);

    cv::Mat histImage( hist_h, hist_w, CV_8UC3, cv::Scalar( 0, 0, 0));

    for (int i = 1; i < histSize; i++)
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                         Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                         Scalar( 0, 0, 255), 2, 8, 0  );
    }

    mutex.lock();
    cv::imshow("HSV histogram", histImage);  // If a HSV image would be parsed into this function, channel 1 (green) would
    mutex.unlock();                          // describe the saturation value of the image
}

void detect(cv::Mat& pt)
{
    int ptx = pt.cols/2;
    int pty = pt.rows/2;
    int maxpoint = 0;
    int counter = 0;
    int angle = 0;
    int tempangle = 0;

    cv::Mat pt2 = pt.clone();

    // *************************************************************************************************//
    // HSV histogram test (line 110)
    // *************************************************************************************************//

    // makeHistogram(pt2);


    // If a marble is not detected, angle is set to -1

    // The algorithm that finds the column with the most blue pixels
    if (isMarble(pt))
    {

        for (int i = 0; i < pt.cols; i++)
        {
            for (int j = 0; j < pt.rows; j++)
            {
                if ((int)pt.at<cv::Vec3b>(j,i)[1] > 240)
                {
                    counter++;
                }

            }
            if (counter > maxpoint)
            {
                maxpoint = counter;
                tempangle = i;
            }
            counter = 0;
        }
        angle = tempangle;
    }
    else
        angle = -1;
    // *************************************************************************************************//
    // Make a line through the center of the closest marble. This was used to test if the correct angle
    // was found (line 145 - 153)
    // *************************************************************************************************//

/*
    if (angle != -1)
    {
        for (int i = 0; i < pt.rows; i++)
        {
            cv::circle(pt2, cv::Point(angle,i), 1, cv::Scalar(0,255,255), -1, cv::LINE_8, 0);
        }
    }
*/

    // *************************************************************************************************//
    // This was used to see the highest and lowest saturation in a image. testhigh would include the value
    // of the pixel containing the highest saturation and testlow would contain the
    // value of the pixel containing the lowest saturation value not being 0.
    //  (line 161 - 175)
    // *************************************************************************************************//
/*
    int testhigh = 0;
    int testlow = 255;
    for (int i = 0; i < pt.cols; i++)
    {
        for (int j = 0; j < pt.rows; j++)
        {
            if ((int)pt.at<cv::Vec3b>(j,i)[1] > testhigh)
                testhigh = (int)pt.at<cv::Vec3b>(j,i)[1];

            if ((int)pt.at<cv::Vec3b>(j,i)[1] < testlow && (int)pt.at<cv::Vec3b>(j,i)[1] != 0)
                testlow = (int)pt.at<cv::Vec3b>(j,i)[1];
        }
    }
*/

//    std::cout << "lowest saturation value: " << testlow << " - highest saturation: " << testhigh << std::endl;

     // *************************************************************************************************//
     // This test was made to see where the low saturated pixels were located (line 183 - 194)
     // *************************************************************************************************//

/*
    for (int i = 0; i < pt.cols; i++)
    {
        for (int j = 0; j < pt.rows; j++)
        {
            if ((int)pt.at<cv::Vec3b>(j,i)[1] < 240 && (int)pt.at<cv::Vec3b>(j,i)[1] > 1)
            {
                cv::circle(pt2, cv::Point(i,j), 1, cv::Scalar(255,0,0), -1, cv::LINE_8, 0);
            }
        }
    }
*/


    mutex.lock();
    cv::imshow("Center of the marble", pt2);
    mutex.unlock();
    marbleAngle = angle;

}

void statCallback(ConstWorldStatisticsPtr &_msg) {
  (void)_msg;
  // Dump the message contents to stdout.
  //  std::cout << _msg->DebugString();
  //  std::cout << std::flush;
}

void poseCallback(ConstPosesStampedPtr &_msg) {
  // Dump the message contents to stdout.
  //  std::cout << _msg->DebugString();

  for (int i = 0; i < _msg->pose_size(); i++) {
      /*
    if (_msg->pose(i).name() == "pioneer2dx") {

      std::cout << std::setprecision(2) << std::fixed << std::setw(6)
                << _msg->pose(i).position().x() << std::setw(6)
                << _msg->pose(i).position().y() << std::setw(6)
                << _msg->pose(i).position().z() << std::setw(6)
                << _msg->pose(i).orientation().w() << std::setw(6)
                << _msg->pose(i).orientation().x() << std::setw(6)
                << _msg->pose(i).orientation().y() << std::setw(6)
                << _msg->pose(i).orientation().z() << std::endl;

    }*/
  }
}

void cameraCallback(ConstImageStampedPtr &msg) {

  std::size_t width = msg->image().width();
  std::size_t height = msg->image().height();
  const char *data = msg->image().data().c_str();
  cv::Mat im(int(height), int(width), CV_8UC3, const_cast<char *>(data));

  im = im.clone();
  cv::cvtColor(im, im, CV_BGR2RGB);

  mutex.lock();
  cv::imshow("camera", im);
  mutex.unlock();
}

void houghDetect(cv::Mat& pt)
{

    // https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/hough_circle/hough_circle.html
    // Used to test if there could be used Hough Detection

    if( !pt.data)
    {
        return;
    }
    cv::Mat pt_gray;
    // convert to gray
    cvtColor(pt, pt_gray, CV_BGR2GRAY);

    // Reduce the noise so we avoid false circle detection

    GaussianBlur( pt_gray, pt_gray, Size(9,9), 2, 2);

    std::vector<Vec3f> circles;

    // Apply the Hough Transform to find the circles. This was tested with different parameters
    HoughCircles( pt_gray, circles, CV_HOUGH_GRADIENT, 1, pt_gray.rows/16, 200, 100, 0, 0);


    // Draw the circles detected
    for (size_t i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        circle( pt, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // circle outline
        circle( pt, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }



    mutex.lock();
//        namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
        imshow( "Hough Circle Transform Demo", pt);
    mutex.unlock();




}

void cameraFunction(ConstImageStampedPtr &msg) {
  std::size_t width = msg->image().width();
  std::size_t height = msg->image().height();
  const char *data = msg->image().data().c_str();
  cv::Mat im(int(height), int(width), CV_8UC3, const_cast<char *>(data));

  cv::Mat hst;

  hst = im.clone();
//  cv::cvtColor(im, im, CV_BGR2RGB);



  cv::Mat pt;

  pt = im.clone();

  cv::cvtColor(pt,pt,CV_RGB2HSV);
  detect(pt);

 // cv::cvtColor(pt,pt, CV_RGB2BGR);
 // houghDetect(pt);
}

void lidarCallback(ConstLaserScanStampedPtr &msg) {

  scan.parseLaserScannerMessage(msg);
  //  std::cout << ">> " << msg->DebugString() << std::endl;
  float angle_min = float(msg->scan().angle_min());
  //  double angle_max = msg->scan().angle_max();
  float angle_increment = float(msg->scan().angle_step());

  float range_min = float(msg->scan().range_min());
  float range_max = float(msg->scan().range_max());

  int sec = msg->time().sec();
  int nsec = msg->time().nsec();

  int nranges = msg->scan().ranges_size();
  int nintensities = msg->scan().intensities_size();

  assert(nranges == nintensities);
  int width = 400;
  int height = 400;
  float px_per_m = 200 / range_max;

  cv::Mat im(height, width, CV_8UC3);
  im.setTo(0);
  for (int i = 0; i < nranges; i++) {
    float angle = angle_min + i * angle_increment;
    float range = std::min(float(msg->scan().ranges(i)), range_max);
    //    double intensity = msg->scan().intensities(i);
    cv::Point2f startpt(200.5f + range_min * px_per_m * std::cos(angle),
                        200.5f - range_min * px_per_m * std::sin(angle));
    cv::Point2f endpt(200.5f + range * px_per_m * std::cos(angle),
                      200.5f - range * px_per_m * std::sin(angle));
    cv::line(im, startpt * 16, endpt * 16, cv::Scalar(255, 255, 255, 255), 1,
             cv::LINE_AA, 4);

    //    std::cout << angle << " " << range << " " << intensity << std::endl;
  }



  cv::circle(im, cv::Point(200, 200), 2, cv::Scalar(0, 0, 255));
  cv::putText(im, std::to_string(sec) + ":" + std::to_string(nsec),
              cv::Point(10, 20), cv::FONT_HERSHEY_PLAIN, 1.0,
              cv::Scalar(255, 0, 0));

  mutex.lock();
  cv::imshow("lidar", im);
  mutex.unlock();
}

int main(int _argc, char **_argv) {
  // Load gazebo
  gazebo::client::setup(_argc, _argv);

  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  // Listen to Gazebo topics
  gazebo::transport::SubscriberPtr statSubscriber =
      node->Subscribe("~/world_stats", statCallback);

  gazebo::transport::SubscriberPtr poseSubscriber =
      node->Subscribe("~/pose/info", poseCallback);

  gazebo::transport::SubscriberPtr cameraSubscriber =
      node->Subscribe("~/pioneer2dx/camera/link/camera/image", cameraCallback);

  gazebo::transport::SubscriberPtr cameraSubscriber2 =
      node->Subscribe("~/pioneer2dx/camera/link/camera/image", cameraFunction);

  gazebo::transport::SubscriberPtr lidarSubscriber =
      node->Subscribe("~/pioneer2dx/hokuyo/link/laser/scan", lidarCallback);

  // Publish to the robot vel_cmd topic
  gazebo::transport::PublisherPtr movementPublisher =
      node->Advertise<gazebo::msgs::Pose>("~/pioneer2dx/vel_cmd");

  // Publish a reset of the world
  gazebo::transport::PublisherPtr worldPublisher =
      node->Advertise<gazebo::msgs::WorldControl>("~/world_control");
  gazebo::msgs::WorldControl controlMessage;
  controlMessage.mutable_reset()->set_all(true);
  worldPublisher->WaitForConnection();
  worldPublisher->Publish(controlMessage);

  const int key_left = 81;
  const int key_up = 82;
  const int key_down = 84;
  const int key_right = 83;
  const int key_esc = 27;


  FuzzyBugController marble;
  marble.buildController();
      float speed = 0.0;
      float dir = 0.0;

  // Loop
  while (true) {
    gazebo::common::Time::MSleep(10);
    marble.updateMarbleAngle(marbleAngle);
    mutex.lock();
    int key = cv::waitKey(1);
    mutex.unlock();
    speed = marble.getControlOutput().speed;
    dir = marble.getControlOutput().direction;


    if (key == key_esc)
      break;

    if ((key == key_up) && (speed <= 1.2f))
      speed += 0.05;
    else if ((key == key_down) && (speed >= -1.2f))
      speed -= 0.05;
    else if ((key == key_right) && (dir <= 0.4f))
      dir += 0.05;
    else if ((key == key_left) && (dir >= -0.4f))
      dir -= 0.05;


    // Generate a pose
    ignition::math::Pose3d pose(double(speed), 0, 0, 0, 0, double(dir));

    // Convert to a pose message
    gazebo::msgs::Pose msg;
    gazebo::msgs::Set(&msg, pose);
    movementPublisher->Publish(msg);

}
  // Make sure to shut everything down.
  gazebo::client::shutdown();
}

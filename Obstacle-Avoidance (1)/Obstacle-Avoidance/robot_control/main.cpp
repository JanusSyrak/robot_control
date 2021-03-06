#include <gazebo/gazebo_client.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/transport/transport.hh>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "laserscanner.h"
#include "fuzzybugcontroller.h"
#include <cmath>


/*
int main(int _argc, char **_argv)
{
    std::cout << atan2(-6,-12) << std::endl;
}
*/


static boost::mutex mutex;
LaserScanner ls;
FuzzyBugController fb(&ls);


void statCallback(ConstWorldStatisticsPtr &_msg) {
  (void)_msg;
  // Dump the message contents to stdout.
  //  std::cout << _msg->DebugString();
  //  std::cout << std::flush;
}



float goalX = 12;
float goalY = 0;
float goalX2 = -20;
float goalY2 = 2;



void poseCallback(ConstPosesStampedPtr &_msg) {
  // Dump the message contents to stdout.
  //  std::cout << _msg->DebugString();

  for (int i = 0; i < _msg->pose_size(); i++) {
    if (_msg->pose(i).name() == "pioneer2dx") {
/*
      std::cout << std::setprecision(2) << std::fixed << std::setw(6)
                << _msg->pose(i).position().x() << std::setw(6)
                << _msg->pose(i).position().y() << std::setw(6)
           //     << _msg->pose(i).position().z() << std::setw(6)
                << _msg->pose(i).orientation().w() << std::setw(6)
             //   << _msg->pose(i).orientation().x() << std::setw(6)
              //  << _msg->pose(i).orientation().y() << std::setw(6)
                << _msg->pose(i).orientation().z() << std::setw(6)
                << acos(_msg->pose(i).orientation().w())*2 << std::setw(6)
                << asin(_msg->pose(i).orientation().z()) *2 << std::endl;
*/

      //  std::cout << _msg->pose(i).orientation().w() << "     " << asin(_msg->pose(i).orientation().z()) << "       " << atan2(asin(_msg->pose(i).orientation().z()),_msg->pose(i).orientation().w()) << std::endl;
        float robotX = _msg->pose(i).position().x();
        float robotY = _msg->pose(i).position().y();
        float robotRW = _msg->pose(i).orientation().w();
        float robotRX = _msg->pose(i).orientation().x();
        float robotRY = _msg->pose(i).orientation().y();
        float robotRZ = _msg->pose(i).orientation().z();
  //      float angleSign = asin(_msg->pose(i).orientation().z()) *2;
        float goalAngle = atan2( goalY - robotY, goalX - robotX);
        float goalDistance = sqrt((goalX-robotX)*(goalX-robotX)+(goalY-robotY)*(goalY-robotY));
        std::cout << goalDistance << std::endl;
  //      float robotAngle = acos(_msg->pose(i).orientation().w())*2;

       float sinRobot = 2.0 * (robotRW * robotRZ + robotRX * robotRY);
       float cosRobot = 1.0 -2.0 * (robotRY * robotRY + robotRZ * robotRZ);

       float phi = atan2(sinRobot, cosRobot);

      if(goalDistance < 0.1)
       {
           goalX = goalX2;
           goalY = goalY2;
       }


       if(goalAngle-phi < - M_PI)
       {
           fb.updatePointDirection(goalAngle-phi+2*M_PI);
        //   std::cout << goalAngle-phi+2*M_PI << std::endl;
       }
       else if (goalAngle-phi < M_PI)
       {
           fb.updatePointDirection(goalAngle-phi);
         //  std::cout << goalAngle-phi << std::endl;
       }
       else
       {
           fb.updatePointDirection(goalAngle-phi-2*M_PI);
 //          std::cout << goalAngle - phi-2*M_PI << std::endl;
       }
    //   std::cout << goalAngle-phi << "      " << goalDistance << std::endl;




/*
    if(robotX < goalX)
    {
        if(angleSign >= 0)
        {
            fb.updatePointDirection(goalAngle - robotAngle);
            std::cout << (goalAngle - robotAngle) << std::endl;
        }
        else
        {
            fb.updatePointDirection(goalAngle + robotAngle);
            std::cout << goalAngle + robotAngle << std::endl;
        }
    }
    if(robotY < goalY && robotX > goalX)
    {
        if(robotAngle < 0)
        {
            fb.updatePointDirection(goalAngle + robotAngle);
              std::cout <<  goalAngle + robotAngle << std::endl;
        }
        else
        {
            fb.updatePointDirection(goalAngle - robotAngle);
            std::cout << goalAngle - robotAngle << std::endl;
        }
    }
    if(robotY > goalY && robotX > goalX)
    {
        if(angleSign < 0)
        {
            fb.updatePointDirection((2*M_PI) + robotAngle);
              std::cout << (2*M_PI)+ goalAngle + robotAngle << std::endl;
        }
        else
        {
            fb.updatePointDirection((2*M_PI) + robotAngle);
            std::cout << (2*M_PI) + goalAngle - robotAngle << std::endl;
        }

    }
*/


/*
       if(goalX > robotX)
       {
           fb.updatePointDirection(goalAngle-robotAngle);
        //   std::cout << goalAngle-robotAngle << std::endl;
       }
       else
           if(goalX < robotX && goalY > robotY && robotAngle >= 0)
           {
               fb.updatePointDirection(goalAngle-robotAngle);
              // std::cout << goalAngle-robotAngle << std::endl;
           }
           else if (goalX < robotX && goalY > robotY && robotAngle < 0)
           {
               if(goalAngle-robotAngle > M_PI)
               {
                   fb.updatePointDirection(-M_PI-robotAngle-(M_PI-goalAngle));
                 //  std::cout << -M_PI-robotAngle-(M_PI-goalAngle) << std::endl;
               }
               else
               {
                  // std::cout << goalAngle-robotAngle << std::endl;
                   fb.updatePointDirection(goalAngle - robotAngle);ObstacleDistance
               }
           }
           else if (goalX < robotX && goalY < robotY && robotAngle >= 0 )
           {
               if(robotAngle - goalAngle > M_PI)
               {
                   fb.updatePointDirection(M_PI-robotAngle+M_PI+goalAngle);
                  // std::cout << M_PI-robotAngle+M_PI+goalAngle << std::endl;
               }
               else
               {
                   fb.updatePointDirection(goalAngle-robotAngle);
                  // std::cout << goalAngle-robotAngle << std::endl;

               }
           }
           else if(goalX < robotX && goalY < robotY && robotAngle < 0)
           {
               fb.updatePointDirection(goalAngle + robotAngle);
               // std::cout << goalAngle + robotAngle << std::endl;
           }


    //    std::cout << goal << "       " << goal-(asin(_msg->pose(i).orientation().z()) *2) << "         "  << asin(_msg->pose(i).orientation().z()) *2 << std::endl;
    //   std::cout << goal-(asin(_msg->pose(i).orientation().z()) *2) << std::endl;
     //   std::cout << asin(_msg->pose(i).orientation().z()) *2 << std::endl;
*/

    }
  }
}



void cameraCallback(ConstImageStampedPtr &msg) {

  std::size_t width = msg->image().width();
  std::size_t height = msg->image().height();
  const char *data = msg->image().data().c_str();
  cv::Mat im(int(height), int(width), CV_8UC3, const_cast<char *>(data));

  im = im.clone();
  cv::cvtColor(im, im, CV_RGB2BGR);

  mutex.lock();
  cv::imshow("camera", im);
  mutex.unlock();
}





void lidarCallback(ConstLaserScanStampedPtr &msg) {

 // std::cout << ls.getClosestDirection(-1.57,1.57) << "     " << ls.getClosestDistance(-1.57,1.57) << std::endl;
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
 //   cv::Point2f startpt1(200.5f + range_min * px_per_m * std::cos(ls.getClosestDirection(-1.57,1.57)),
 //                       200.5f - range_min * px_per_m * std::sin(ls.getClosestDirection(-1.57,1.57)));
  //  cv::Point2f endpt1(200.5f + range * px_per_m * std::cos(ls.getClosestDirection(-1.57,1.57)),
   //                   200.5f - range * px_per_m * std::sin(ls.getClosestDirection(-1.57,1.57)));
   // cv::line(im, startpt1 * 16, endpt1 * 16, cv::Scalar(255, 0, 0, 255), 1,
    //         cv::LINE_AA, 4);

    //    std::cout << angle << " " << range << " " << intensity << std::endl;
  }
  cv::circle(im, cv::Point(200, 200), 2, cv::Scalar(0, 0, 255));
  cv::putText(im, std::to_string(sec) + ":" + std::to_string(nsec),
              cv::Point(10, 20), cv::FONT_HERSHEY_PLAIN, 1.0,
              cv::Scalar(255, 0, 0));

  ls.parseLaserScannerMessage(msg);

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




  fb.buildController();




  const int key_left = 81;
  const int key_up = 82;
  const int key_down = 84;
  const int key_right = 83;
  const int key_esc = 27;

  float speed = 0.0;
  float dir = 0.0;




  // Loop
  while (true) {
    gazebo::common::Time::MSleep(10);



    mutex.lock();
    int key = cv::waitKey(1);
    mutex.unlock();

     speed = fb.getControlOutput().speed;
     dir = fb.getControlOutput().direction;
 //   std::cout << dir << "          " << ls.getClosestDirection(-1.57,1.57) << std::endl;


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
    else {
      // slow down
      //      speed *= 0.1;
      //      dir *= 0.1;
    }

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


#ifndef FUZZYBUGCONTROLLER_H
#define FUZZYBUGCONTROLLER_H
#include <iostream>
/*************************************************************/
/*************************************************************/

#include "laserscanner.h"
#include <fl/Headers.h>
#include <opencv2/opencv.hpp>

struct ControlOutput
{
    float direction;
    float speed;
};

/*************************************************************/
/*************************************************************/

class FuzzyBugController
{
public:
    FuzzyBugController();
    FuzzyBugController(LaserScanner* pc_laser_scanner);
    virtual ~FuzzyBugController() = default;

    virtual void buildController();
    virtual ControlOutput getControlOutput();

    void updateMarbleAngle(int N);


protected:
    LaserScanner*        m_pcLaserScanner;

    fl::Engine*          m_pcFLEngine;
    fl::InputVariable*   m_pflObstacleDirection;
    fl::InputVariable*   m_pflObstacleDistance;
    fl::OutputVariable*  m_pflSteerDirection;
    fl::OutputVariable*  m_pflSpeed;
    fl::InputVariable*   m_pflAngle;

    float marbleangle;
};

/*************************************************************/
/*************************************************************/

#endif // FUZZYBUGCONTROLLER_H

#include "fuzzybugcontroller.h"

/*************************************************************/
/*************************************************************/

FuzzyBugController::FuzzyBugController()
{
    marbleangle = 0;
}

FuzzyBugController::FuzzyBugController(LaserScanner *pc_laser_scanner) : m_pcLaserScanner(pc_laser_scanner)
{

}

/*************************************************************/
/*************************************************************/

ControlOutput FuzzyBugController::getControlOutput()
{
 //   m_pflObstacleDistance->setValue(m_pcLaserScanner->getClosestDistance(-1.57, 1.57));
 //   m_pflObstacleDirection->setValue(m_pcLaserScanner->getClosestDirection(-1.57, 1.57));



     m_pflAngle->setValue(marbleangle);
//   std::cout << "angle: " << marbleangle << std::endl;



    // std::cout << "FL - Distance " << m_pcLaserScanner->getClosestDistance(-1.57, 1.57) << ", direction " << m_pcLaserScanner->getClosestDirection(-1.57, 1.57) << std::endl;
    m_pcFLEngine->process();


    ControlOutput out;
    out.direction = m_pflSteerDirection->getValue();
    out.speed     = m_pflSpeed->getValue();

    return out;

}

/*************************************************************/
/*************************************************************/

void FuzzyBugController::buildController()
{
    using namespace fl;
    m_pcFLEngine = FllImporter().fromFile("fuzzybugcontroller.fll");

    std::string status;
    if (not m_pcFLEngine->isReady(&status))
        throw Exception("[engine error] engine is not ready:n" + status, FL_AT);

    m_pflObstacleDirection = m_pcFLEngine->getInputVariable("ObstacleDirection");
    m_pflObstacleDistance  = m_pcFLEngine->getInputVariable("ObstacleDistance");
    m_pflSteerDirection    = m_pcFLEngine->getOutputVariable("SteerDirection");
    m_pflSpeed             = m_pcFLEngine->getOutputVariable("Speed");
    m_pflAngle             = m_pcFLEngine->getInputVariable("Angle");

}

/*************************************************************/
/*************************************************************/

void FuzzyBugController::updateMarbleAngle(int N)
{
    marbleangle = N;
//    std::cout << "marbleangle: " << marbleangle << std::endl;
}

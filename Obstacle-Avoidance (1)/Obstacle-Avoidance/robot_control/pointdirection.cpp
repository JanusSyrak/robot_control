#include "pointdirection.h"

PointDirection::PointDirection()
{

}

PointDirection::PointDirection(int g[1][1])
{
    goal[0][0] = g[0][0];
}

void PointDirection::updateAngle(float a)
{
    pointAngle = a;
}

#ifndef POINTDIRECTION_H
#define POINTDIRECTION_H


class PointDirection
{
public:
    PointDirection();
    PointDirection(int a[1][1]);
    void updateAngle(float angle);
    int goal[1][1];
    float pointAngle;
};



#endif // POINTDIRECTION_H

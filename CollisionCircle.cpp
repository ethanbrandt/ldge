#include "CollisionCircle.h"
#include<cmath>

CollisionCircle::CollisionCircle()
{
    radius = 1;
}

CollisionCircle::CollisionCircle(float newRadius)
{
    radius = newRadius;
}


float CollisionCircle::getArea()
{
    return M_PI * pow(radius,2);
}

float CollisionCircle::getRadius()
{
    return radius;
}

void CollisionCircle::setRadius(float newRadius)
{
    radius = newRadius;
}


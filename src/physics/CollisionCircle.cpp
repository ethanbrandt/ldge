#define _USE_MATH_DEFINES

#include "CollisionCircle.h"
#include <cmath>


CollisionCircle::CollisionCircle()
{
    radius = 1;
}

CollisionCircle::CollisionCircle(float newRadius)
{
    radius = newRadius;
}


float CollisionCircle::GetArea()
{
    return M_PI * pow(radius,2);
}

float CollisionCircle::GetRadius()
{
    return radius;
}

void CollisionCircle::SetRadius(float newRadius)
{
    radius = newRadius;
}


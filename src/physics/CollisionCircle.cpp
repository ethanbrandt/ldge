#define _USE_MATH_DEFINES

#include "CollisionCircle.h"
#include <cmath>


CollisionCircle::CollisionCircle()
{
    radius = 1;
    Vector2 pos(0,0);
    trigger = false;
    collisionMask = 0; 
    posOffset = pos;
}

CollisionCircle::CollisionCircle(float _Radius, Vector2 _PosOffset, bool _IsTrigger, uint16_t _CollMask)
: CollisionShape(_PosOffset, _IsTrigger, _CollMask), radius(_Radius) 
{
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


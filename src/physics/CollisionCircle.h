#pragma once
#include"CollisionShape.h"

class CollisionCircle : public CollisionShape
{
private:
    float radius;

public:
    CollisionCircle();
    CollisionCircle(float _Radius, Vector2 _PosOffset, bool _IsTrigger, uint16_t _CollMask);
    float GetArea() override;
    float GetRadius();
    void SetRadius(float _Radius);
};
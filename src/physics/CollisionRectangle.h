#pragma once
#include "CollisionShape.h"

class CollisionRectangle : public CollisionShape
{
private:
	float width;
	float length;

public:
    CollisionRectangle();
    CollisionRectangle(float _Width, float _Length, Vector2 _PosOffset, bool _IsTrigger, uint16_t _CollMask);
    float GetArea() override;
    float GetLength();
    float GetWidth();
    void SetWidth(float newWidth);
    void SetLength(float newLength);
};
#include"CollisionRectangle.h"


CollisionRectangle::CollisionRectangle()
{
    length = 1;
    width = 1;
    Vector2 pos(0,0);
    trigger = false;
    collisionMask = 0; 
    posOffset = pos;
}

CollisionRectangle::CollisionRectangle(float _Width, float _Length, Vector2 _PosOffset, bool _IsTrigger, uint16_t _CollMask)
{
    length = _Length;
    width = _Width;
    posOffset = _PosOffset;
    trigger = _IsTrigger;
    collisionMask = _CollMask;
}

float CollisionRectangle::GetArea()
{
    return width * length;
}

float CollisionRectangle::GetLength()
{
    return length;
}

float CollisionRectangle::GetWidth()
{
    return width;
}

void CollisionRectangle::SetLength(float _length)
{
    length = _length;
}

void CollisionRectangle::SetWidth(float _width)
{
    width = _width;
}
#include"CollisionRectangle.h"


CollisionRectangle::CollisionRectangle()
{
    length = 1;
    width = 1;
}

CollisionRectangle::CollisionRectangle(float newWidth, float newLength)
{
    length = newLength;
    width = newWidth;
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
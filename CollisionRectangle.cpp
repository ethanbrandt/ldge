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

float CollisionRectangle::getArea()
{
    return width * length;
}

float CollisionRectangle::getLength()
{
    return length;
}

float CollisionRectangle::getWidth()
{
    return width;
}

void CollisionRectangle::setLength(float newLength)
{
    length = newLength;
}

void CollisionRectangle::setWidth(float newWidth)
{
    width = newWidth;
}
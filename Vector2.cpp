#include"Vector2.h"
#include<cmath>


Vector2::Vector2()
{
    x = 0;
    y = 0;
}

Vector2::Vector2(float newX, float newY)
{
    x = newX;
    y = newY;
}

void Vector2::setX(float newX)
{
    x = newX;
}

void Vector2::setY(float newY)
{
    y = newY;
}

float Vector2::getX()
{
    return x;
}

float Vector2::getY()
{
    return y;
}

float Vector2::getMagnitude()
{
    return sqrt(pow(x,2) + pow(y,2));
}


Vector2 Vector2::Normalize(Vector2 a)
{
    float mag = a.getMagnitude();
    float normX = a.getX() / mag;
    float normY = a.getY() / mag;

    return  Vector2(normX, normY);
}

Vector2 Vector2::operator+(Vector2& other)
{
     
    float newX = x + other.getX();
    float newY = y + other.getY();

    return Vector2(newX, newY);
}

Vector2 Vector2::operator-(Vector2& other)
{
    float newX = x - other.getX();
    float newY = y - other.getY();

    return Vector2(newX, newY);
}

Vector2 Vector2::operator*(int scale)
{
    float newX = scale * x;
    float newY = scale * y;

    return Vector2(newX, newY);
}

Vector2 Vector2::operator*(float scale)
{
    float newX = scale * x;
    float newY = scale * y;

    return Vector2(newX, newY);
}

float Vector2::dot(Vector2 a, Vector2 b)
{
    return (a.getX() * b.getX()) + (a.getY() * b.getY());
}
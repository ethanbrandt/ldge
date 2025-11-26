#include"Vector2.h"
#include<cmath>


Vector2::Vector2()
{
    x = 0;
    y = 0;
}

Vector2::Vector2(float _x, float _y)
{
    x = _x;
    y = _y;
}

void Vector2::SetX(float _x)
{
    x = _x;
}

void Vector2::SetY(float _y)
{
    y = _y;
}

float Vector2::GetX() const
{
    return x;
}

float Vector2::GetY() const
{
    return y;
}

float Vector2::GetMagnitude()
{
    return sqrt(pow(x,2) + pow(y,2));
}


Vector2 Vector2::Normalize(Vector2 a)
{
    float mag = a.GetMagnitude();
    float normX = a.GetX() / mag;
    float normY = a.GetY() / mag;

    return  Vector2(normX, normY);
}

Vector2 Vector2::operator+(Vector2& other)
{
     
    float newX = x + other.GetX();
    float newY = y + other.GetY();

    return Vector2(newX, newY);
}

Vector2 Vector2::operator-(Vector2& other)
{
    float newX = x - other.GetX();
    float newY = y - other.GetY();

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

Vector2 operator*(int scale, const Vector2& v)
{
    return Vector2(v.GetX() * scale, v.GetY() * scale);
}

Vector2 operator*(float scale, const Vector2& v)
{
    return Vector2(v.GetX() * scale, v.GetY() * scale);
}

float Vector2::dot(Vector2 _a, Vector2 _b)
{
    return (_a.GetX() * _b.GetX()) + (_a.GetY() * _b.GetY());
}
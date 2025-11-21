#include"CollisionShape.h"

class CollisionRectangle : public CollisionShape
{
private:

float width;
float length;

public:
    CollisionRectangle();
    CollisionRectangle(float newWidth, float newLength);
    float getArea();
    float getLength();
    float getWidth();
    void setWidth(float newWidth);
    void setLength(float newLength);
};
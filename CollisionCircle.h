#include"CollisionShape.h"

class CollisionCircle : public CollisionShape
{
private:
    float radius;

public:
    CollisionCircle();
    CollisionCircle(float newRadius);
    float getArea();
    float getRadius();
    void setRadius(float newRadius);

};
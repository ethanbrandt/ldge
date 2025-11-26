#include"CollisionShape.h"

class CollisionCircle : public CollisionShape
{
private:
    float radius;

public:
    CollisionCircle();
    CollisionCircle(float newRadius);
    float GetArea() override;
    float GetRadius();
    void SetRadius(float newRadius);
};
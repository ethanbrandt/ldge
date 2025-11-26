#include"CollisionShape.h"

class CollisionRectangle : public CollisionShape
{
private:
	float width;
	float length;

public:
    CollisionRectangle();
    CollisionRectangle(float newWidth, float newLength);
    float GetArea() override;
    float GetLength();
    float GetWidth();
    void SetWidth(float newWidth);
    void SetLength(float newLength);
};
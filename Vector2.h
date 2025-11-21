class Vector2
{
private:
    float x;
    float y;

public:
    Vector2();
    Vector2(float newX, float newY);


    void setX(float x);
    void setY(float y);
    float getX();
    float getY();
    float getMagnitude();


    static Vector2 Normalize(Vector2 a);

    Vector2 operator+(Vector2& other);
    Vector2 operator-(Vector2& other);
    Vector2 operator*(int scale);
    Vector2 operator*(float scale);

    static float dot(Vector2 a, Vector2 b);
    //Everything has been implemented. I just need testing!



};


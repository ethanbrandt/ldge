class Vector2
{
private:
    float x;
    float y;

public:
    Vector2();
    Vector2(float _x, float _y);


    void SetX(float _x);
    void SetY(float _y);
    float GetX();
    float GetY();
    float GetMagnitude();


    static Vector2 Normalize(Vector2 _a);

    Vector2 operator+(Vector2& other);
    Vector2 operator-(Vector2& other);
    Vector2 operator*(int scale);
    Vector2 operator*(float scale);

    static float dot(Vector2 _a, Vector2 _b);
};


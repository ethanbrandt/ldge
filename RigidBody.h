#include"Vector2.h"
#include"CollisionShape.h"
#include"CollisionCircle.h"
#include"CollisionRectangle.h"



struct CollisionResult
{
    bool collided;
    float depth;
    float radii;
    Vector2 normal;

};

class RigidBody
{
private:
    Vector2 position;
    Vector2 velocity;
    float mass;
    CollisionShape* colShape;
    Vector2 normal;

public:
    RigidBody();
    RigidBody(Vector2 newPosition, Vector2 newVelocity, float newMass, CollisionShape* newColShape);
    Vector2 getPosition();
    Vector2 getVelocity();
    float getMass();
    CollisionShape* getColShape();
    void setPosition(Vector2 newPos);
    void setVelocity(Vector2 newVel);
    void setMass(float newMass);
    void setShape(CollisionShape* newColShape);
    //only circles have been implemented for collision detection
    CollisionResult DetectCollision(RigidBody a, RigidBody b);
    void ResolveCollision(RigidBody a, RigidBody b);
    

};
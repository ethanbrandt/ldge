#include"Vector2.h"
#include"CollisionShape.h"
#include"CollisionCircle.h"
#include"CollisionRectangle.h"



struct CollisionResult
{   
    
    bool collided;

    //used for circ-circ collisions
    float depth;
    float radii;
    Vector2 normal;

    float overlap; //used for circle-rect collisions

    //these ones down here are used for rect-rect collisions
    float overlapX;
    float overlapY;

    float overlapX1;
    float overlapX2;
    float overlapY1;
    float overlapY2;
    int type;


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
    static CollisionResult DetectCollision(RigidBody a, RigidBody b);
    static void ResolveCollision(RigidBody a, RigidBody b);
    

};

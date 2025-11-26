#pragma once
#include"../Vector2.h"
#include"CollisionShape.h"
#include"CollisionCircle.h"
#include"CollisionRectangle.h"

enum CollisionType
{
	CIRCLE_CIRCLE,
	RECTANGLE_RECTANGLE,
	CIRCLE_RECTANGLE
};

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
    CollisionType type;
};

class RigidBody
{
private:
    Vector2 position;
    Vector2 velocity;
    float mass;
    CollisionShape* colShape;

	static CollisionResult DetectCircleCircleCollision(CollisionShape* _shapeA, CollisionShape* _shapeB, RigidBody& _a, RigidBody& _b);
	static CollisionResult DetectRectangleRectangleCollision(CollisionShape *shapeA, CollisionShape *shapeB, RigidBody &_a, RigidBody &_b);
	static CollisionResult DetectCircleRectangleCollision(CollisionShape* _shapeA, CollisionShape* _shapeB, RigidBody& _a, RigidBody& _b);

	static void ResolveCircleCircleCollision(CollisionResult _res, RigidBody &_a, RigidBody &_b);
	static void ResolveRectangleRectangleCollision(CollisionResult _res, RigidBody &_a, RigidBody &_b);
	static void ResolveCircleRectangleCollision(CollisionResult _res, RigidBody &_circle, RigidBody &_rect);
	
public:
    RigidBody();
    RigidBody(Vector2 _pos, Vector2 _vel, float _mass, CollisionShape* _colShape);
    Vector2 GetPosition();
    Vector2 GetVelocity();
    float GetMass();
    CollisionShape* GetColShape();
    void SetPosition(Vector2 _pos);
    void SetVelocity(Vector2 _vel);
    void SetMass(float _mass);
    void SetShape(CollisionShape* _colShape);
	static CollisionResult DetectCollision(RigidBody _a, RigidBody _b);
	static void ResolveCollision(RigidBody _a, RigidBody _b);
};

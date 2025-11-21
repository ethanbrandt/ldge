#include"RigidBody.h"
#include<typeinfo>
#include<cmath>

RigidBody::RigidBody()
{

}

RigidBody::RigidBody(Vector2 newPosition, Vector2 newVelocity, float newMass, CollisionShape* newColShape)
{
    position = newPosition;
    velocity = newVelocity;
    mass = newMass;
    colShape = newColShape;
}

Vector2 RigidBody::getPosition()
{
    return position;
}

Vector2 RigidBody::getVelocity()
{
    return velocity;
}

float RigidBody::getMass()
{
    return mass;
}

CollisionShape* RigidBody::getColShape()
{
    return colShape;
}

void RigidBody::setPosition(Vector2 newPos)
{
    position = newPos;
}

void RigidBody::setVelocity(Vector2 newVel)
{
    velocity = newVel;
}

void RigidBody::setMass(float newMass)
{
    mass = newMass;
}

void RigidBody::setShape(CollisionShape* newColShape)
{
    colShape = newColShape;
}

CollisionResult RigidBody::DetectCollision(RigidBody a, RigidBody b)
{
    //only circles work
    CollisionResult res;
    CollisionShape* shapeA = a.getColShape();
    CollisionShape* shapeB = b.getColShape();

    if(typeid(shapeA) == typeid(CollisionCircle) && typeid(shapeA) == typeid(shapeB))
    {
        CollisionCircle* circA = static_cast<CollisionCircle*>(shapeA);
        CollisionCircle* circB = static_cast<CollisionCircle*>(shapeB);

        Vector2 offA = circA->getOffset();
        Vector2 offB = circB->getOffset();

        Vector2 centerA = a.getPosition() + offA;
        Vector2 centerB = b.getPosition() + offB;

        float dist = sqrt(pow(centerA.getX() - centerB.getX(), 2) + pow(centerA.getY() - centerB.getY(), 2)); 

        float radii = circA->getRadius() + circB->getRadius();

        float depth = radii - dist;

        Vector2 normal = Vector2::Normalize(centerB - centerA);

        res.depth = depth;
        res.radii = radii;
        res.normal = normal;

        if(dist >= radii)
            res.collided = false;
        else
            res.collided = true;

    }
    return res;
}

void RigidBody::ResolveCollision(RigidBody a, RigidBody b)
{
    //only circles work
    CollisionResult res = DetectCollision(a, b);

    if(res.collided)
    {
        Vector2 normal = res.normal;
        float depth = res.depth;
        Vector2 aPos = a.getPosition();
        Vector2 bPos = b.getPosition();
        Vector2 changeA = normal * -1 * (depth / 2);
        Vector2 changeB = normal * (depth / 2);

        a.setPosition(aPos + changeA);
        b.setPosition(bPos + changeB);
    }
}
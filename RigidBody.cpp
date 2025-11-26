#include"RigidBody.h"
#include<typeinfo>
#include<cmath>

RigidBody::RigidBody()
{
    Vector2 newPosition(0,0);
    Vector2 newVelocity(0,0);
    float newMass = 0;
    CollisionShape newColShape;

    position = newPosition;
    velocity = newVelocity;
    mass = newMass;
    colShape = newColShape;

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
        res.type = 0;

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
    else if(typeid(shapeA) == typeid(CollisionRectangle) && typeid(shapeA) == typeid(shapeB))
    {
        res.type = 1;

        CollisionRectangle* rectA = static_cast<CollisionRectangle*>(shapeA);
        CollisionRectangle* rectB = static_cast<CollisionRectangle*>(shapeB);

        Vector2 offA = rectA->getOffset();
        Vector2 offB = rectB->getOffset();

        Vector2 centerA = a.getPosition() + offA;
        Vector2 centerB = b.getPosition() + offB;

        Vector2 a1, a2, a3, a4; //vertices of rectangle A

        a1.setX(centerA.getX() - (rectA->getLength() / 2));
        a1.setY(centerA.getY() + (rectA->getWidth() / 2));

        a2.setX(centerA.getX() + (rectA->getLength() / 2));
        a2.setY(centerA.getY() + (rectA->getWidth() / 2));

        a3.setX(centerA.getX() + (rectA->getLength() / 2));
        a3.setY(centerA.getY() - (rectA->getWidth() / 2));

        a4.setX(centerA.getX() - (rectA->getLength() / 2));
        a4.setY(centerA.getY() - (rectA->getWidth() / 2));

        Vector2 b1, b2, b3, b4; // vertices of rectangle B

        b1.setX(centerB.getX() - (rectA->getLength() / 2));
        b1.setY(centerB.getY() + (rectA->getWidth() / 2));

        b2.setX(centerB.getX() + (rectA->getLength() / 2));
        b2.setY(centerB.getY() + (rectA->getWidth() / 2));

        b3.setX(centerB.getX() + (rectA->getLength() / 2));
        b3.setY(centerB.getY() - (rectA->getWidth() / 2));

        b4.setX(centerB.getX() - (rectA->getLength() / 2));
        b4.setY(centerB.getY() - (rectA->getWidth() / 2));

        if(!(a2.getX() < b1.getX() || a1.getX() > b2.getX() || a3.getY() > b2.getY() || a1.getY() < b4.getY())) //checks to see if they overlap
        {
            res.collided = true;

            float overlapX1 = b2.getX() - a1.getX(); 
            float overlapX2 = a2.getX() - b1.getX();
            float overlapX = std::min(overlapX1, overlapX2);

            float overlapY1 = b2.getY() - a4.getY();
            float overlapY2 = a1.getY() - b4.getY();
            float overlapY = std::min(overlapY1, overlapY2);

            res.overlapX = overlapX;
            res.overlapY = overlapY;
            res.overlapX1 = overlapX1;
            res.overlapX2 = overlapX2;
            res.overlapY1 = overlapY1;
            res.overlapY2 = overlapY2;

            
        }

        

        /*if(a2.getX() >= b1.getX() && a2.getY() <= b1.getY() && a2.getY() >= b4.getY() && a2.getX() <= b2.getX())
        {

        }
        else if(a3.getX() >= b1.getX() && a3.getY() <= b1.getY() && a3.getY() >= b4.getY() && a3.getX() <= b2.getX())
        {

        }
        else if(a4.getX() >= b1.getX() && a4.getY() <= b1.getY() && a4.getY() >= b4.getY() && a4.getX() <= b2.getX())
        {

        }
        else if(a1.getX() >= b1.getX() && a1.getY() <= b1.getY() && a1.getY() >= b4.getY() && a1.getX() <= b2.getX())
        {

        }*/ //this code ended up being tooo much 

        
    }
    else if(typeid(shapeA) == typeid(CollisionRectangle) && typeid(CollisionCircle) == typeid(shapeB))
        {
            CollisionRectangle* rect = static_cast<CollisionRectangle*>(shapeA);
            CollisionCircle* circ = static_cast<CollisionCircle*>(shapeB);

            Vector2 offRect = rect->getOffset();
            Vector2 offCirc = circ->getOffset();

            Vector2 centerRect = a.getPosition() + offRect;
            Vector2 centerCirc = b.getPosition() + offCirc;

            Vector2 a1, a2, a3, a4; //vertices of rectangle

            a1.setX(centerRect.getX() - (rect->getLength() / 2));
            a1.setY(centerRect.getY() + (rect->getWidth() / 2));

            a2.setX(centerRect.getX() + (rect->getLength() / 2));
            a2.setY(centerRect.getY() + (rect->getWidth() / 2));

            a3.setX(centerRect.getX() + (rect->getLength() / 2));
            a3.setY(centerRect.getY() - (rect->getWidth() / 2));

            a4.setX(centerRect.getX() - (rect->getLength() / 2));
            a4.setY(centerRect.getY() - (rect->getWidth() / 2));

            //we're going from the center of the circle to the closest possible point of contact of the rectangle.
            float closestX = std::clamp(centerCirc.getX(), a1.getX(), a2.getX());
            float closestY = std::clamp(centerCirc.getY(), a3.getY(), a2.getY());

            float dx = centerCirc.getX() - closestX;
            float dy = centerCirc.getY() - closestY;

            float squareDist = pow(dx,2) + pow(dy,2);
            float distance = sqrt(squareDist);

            if(distance <= circ->getRadius())
            {
                res.collided = true;
                res.depth = distance;
                Vector2 normal(dx,dy);
                normal = Vector2::Normalize(normal);
                res.normal = normal;
                res.overlap = circ->getRadius() - distance;
                

            }

        }
        else
            res.collided = false;
    return res;
}

void RigidBody::ResolveCollision(RigidBody a, RigidBody b)
{
    //only circles work
    CollisionResult res = DetectCollision(a, b);
    if(res.collided)
    {
        switch(res.type)
        {
            case 0:
            {
                
                Vector2 normal = res.normal;
                float depth = res.depth;
                Vector2 aPos = a.getPosition();
                Vector2 bPos = b.getPosition();
                Vector2 changeA = normal * -1 * (depth / 2);
                Vector2 changeB = normal * (depth / 2);

                if(a.getMass() == -1)
                {
                    b.setPosition(aPos + changeA + changeA);
                        
                }
                else if(b.getMass() == -1)
                {
                    a.setPosition(aPos + changeB + changeB);
                }
                else
                {
                    a.setPosition(aPos + changeA);
                    b.setPosition(bPos + changeB);
                }
                break;
            }
                
        
            case 1:
            {
                Vector2 positionA = a.getPosition();
                if(res.overlapX < res.overlapY)
                {

                    if(res.overlapX1 < res.overlapX2)
                        positionA.setX(positionA.getX() - res.overlapX1);
                    else
                        positionA.setX(positionA.getX() + res.overlapX2);

                }
                else
                {
                    if(res.overlapY1 < res.overlapY2)
                        positionA.setY(positionA.getY() - res.overlapY1);
                    else
                        positionA.setY(positionA.getY() + res.overlapY2);
                }
                a.setPosition(positionA);
                break;
            }

            case 2:
            {
                Vector2 normal = res.normal;
                Vector2 move = res.normal * res.overlap;

                Vector2 circPos = a.getPosition();

                Vector2 newPos = circPos + move;

                a.setPosition(newPos);
            }
        }
    }

}

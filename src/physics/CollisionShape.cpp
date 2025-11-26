#include"CollisionShape.h"


CollisionShape::CollisionShape()
{
    Vector2 pos(0,0);
    posOffset = pos;
    Trigger = false;
    collisionMask = 0;
}

CollisionShape::CollisionShape(Vector2 newPosOffset, bool newIsTrigger, uint16_t newCollMask)
{
    posOffset = newPosOffset;
    Trigger = newIsTrigger;
    collisionMask = newCollMask;
}

bool CollisionShape::isTrigger()
{
    return Trigger;
}

uint16_t CollisionShape::GetColMask()
{
    return collisionMask;
}

Vector2 CollisionShape::getOffset()
{
    return posOffset;
}
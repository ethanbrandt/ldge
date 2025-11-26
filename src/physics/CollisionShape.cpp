#include"CollisionShape.h"


CollisionShape::CollisionShape()
{
    Vector2 pos(0,0);
    posOffset = pos;
    trigger = false;
    collisionMask = 0;
}

CollisionShape::CollisionShape(Vector2 newPosOffset, bool newIsTrigger, uint16_t newCollMask)
{
    posOffset = newPosOffset;
    trigger = newIsTrigger;
    collisionMask = newCollMask;
}

bool CollisionShape::IsTrigger()
{
    return trigger;
}

uint16_t CollisionShape::GetColMask()
{
    return collisionMask;
}

Vector2 CollisionShape::GetOffset()
{
    return posOffset;
}
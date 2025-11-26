#include"Vector2.h"
#include <cstdint>

class CollisionShape
{
protected:
    Vector2 posOffset;
    bool Trigger;
    uint16_t collisionMask; 

public:
    CollisionShape(); //nothing is implemented
    CollisionShape(Vector2 newPosOffset, bool newIsTrigger, uint16_t newCollMask);

    virtual float GetArea() = 0; //tell him that c++ didn't allow it to be pure virtual
    bool IsTrigger(); 
    uint16_t GetColMask();
    Vector2 GetOffset();
};
#pragma once
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "../gorm/ScriptManager.h"
#include "../utilities/Vector2.h"
#include "CollisionShape.h"
#include "CollisionCircle.h"
#include "CollisionRectangle.h"
#include "RigidBody.h"

class PhysicsManager
{
private:
	static PhysicsManager* instance;
	std::unordered_map<RigidBody*, EntityId> rbToEntity;
	std::vector<RigidBody*> rbs;
public:
	PhysicsManager()
	{
		instance = this;
	}

	static PhysicsManager* GetInstance()
	{
		return instance;
	}

	void UpdatePhysics(float _deltaTime)
	{
		for (RigidBody* rb : rbs)
		{
			Vector2 currPos = rb->GetPosition();
			rb->SetPosition((_deltaTime * rb->GetVelocity()) + currPos);
		}

		std::vector<RigidBodyPair> triggered;
		std::vector<RigidBodyPair> collided;
		RigidBody::ReturnTriggersAndCollisions(rbs, triggered, collided);

		RigidBody::ResolveAllCollisions(rbs);

		for (RigidBodyPair rbPair : triggered)
		{
			ScriptManager::GetInstance()->OnTrigger(rbToEntity[rbPair.a], *rbPair.b);
			ScriptManager::GetInstance()->OnTrigger(rbToEntity[rbPair.b], *rbPair.a);
		}

		for (RigidBodyPair rbPair : collided)
		{
			ScriptManager::GetInstance()->OnCollision(rbToEntity[rbPair.a], *rbPair.b);
			ScriptManager::GetInstance()->OnCollision(rbToEntity[rbPair.b], *rbPair.a);
		}
	}

	void RegisterRigidBody(RigidBody* _rb, EntityId _id)
	{
		rbs.push_back(_rb);
		rbToEntity[_rb] = _id;
	}

	void UnregisterRigidBody(RigidBody* _rb)
	{
		rbs.erase(std::remove(rbs.begin(), rbs.end(), _rb), rbs.end());
		rbToEntity.erase(_rb);
	}
};
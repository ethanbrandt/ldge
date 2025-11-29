	#include "RigidBody.h"
	#include <typeinfo>
	#include <cmath>
	#include <algorithm>
	#include <vector>

	RigidBody::RigidBody()
	{
		Vector2 newPosition(0,0);
		Vector2 newVelocity(0,0);
		float newMass = 0;

		position = newPosition;
		velocity = newVelocity;
		mass = newMass;
		isStatic = false;
	}

	RigidBody::RigidBody(Vector2 _position, Vector2 _velocity, float _mass, CollisionShape* _colShape, bool _isStatic)
	{
		position = _position;
		velocity = _velocity;
		mass = _mass;
		colShape = _colShape;
		isStatic = _isStatic;
	}

	Vector2 RigidBody::GetPosition()
	{
		return position;
	}

	Vector2 RigidBody::GetVelocity()
	{
		return velocity;
	}

	float RigidBody::GetMass()
	{
		return mass;
	}

	CollisionShape* RigidBody::GetColShape()
	{
		return colShape;
	}

	bool RigidBody::IsStatic()
	{
		return isStatic;
	}

	void RigidBody::SetPosition(Vector2 _pos)
	{
		position = _pos;
	}

	void RigidBody::SetVelocity(Vector2 _vel)
	{
		velocity = _vel;
	}

	void RigidBody::SetMass(float _mass)
	{
		mass = _mass;
	}

	void RigidBody::SetShape(CollisionShape* _colShape)
	{
		colShape = _colShape;
	}

	float clamp(float val, float min, float max)
	{
		if(val > max)
			return max;
		else if (val < min)
			return min;
		else 
			return val;

	}

	CollisionResult RigidBody::DetectCollision(RigidBody _a, RigidBody _b)
	{
		CollisionShape* shapeA = _a.GetColShape();
		CollisionShape* shapeB = _b.GetColShape();

		if((shapeA->GetColMask() & shapeB->GetColMask()) == 0) //checks to see if they sheare a bit. If they don't then we're not moving them
		{
			CollisionResult res;
			res.collided = false;
			return res;
		}

		if(typeid(*shapeA) == typeid(CollisionCircle) && typeid(*shapeB) == typeid(CollisionCircle))
			return DetectCircleCircleCollision(shapeA, shapeB, _a, _b);
		else if(typeid(*shapeA) == typeid(CollisionRectangle) && typeid(*shapeB) == typeid(CollisionRectangle))
			return DetectRectangleRectangleCollision(shapeA, shapeB, _a, _b);
		else if(typeid(*shapeA) == typeid(CollisionRectangle) && typeid(CollisionCircle) == typeid(*shapeB))
			return DetectCircleRectangleCollision(shapeA, shapeB, _a, _b);
		else if(typeid(*shapeA) == typeid(CollisionCircle) && typeid(CollisionRectangle) == typeid(*shapeB))
			return DetectCircleRectangleCollision(shapeB, shapeA, _b, _a);
		else
		{
			CollisionResult res;
			res.collided = false;
			return res;
		}
	}

	CollisionResult RigidBody::DetectCircleRectangleCollision(CollisionShape* _shapeA, CollisionShape* _shapeB, RigidBody& _a, RigidBody& _b)
	{
		CollisionResult res;

		res.type = CIRCLE_RECTANGLE;

		CollisionRectangle* rect = static_cast<CollisionRectangle*>(_shapeA);
		CollisionCircle* circ = static_cast<CollisionCircle*>(_shapeB);

		Vector2 offRect = rect->GetOffset();
		Vector2 offCirc = circ->GetOffset();

		Vector2 centerRect = _a.GetPosition() + offRect;
		Vector2 centerCirc = _b.GetPosition() + offCirc;

		Vector2 a1, a2, a3, a4; //vertices of rectangle

		a1.SetX(centerRect.GetX() - (rect->GetLength() / 2));
		a1.SetY(centerRect.GetY() + (rect->GetWidth() / 2));

		a2.SetX(centerRect.GetX() + (rect->GetLength() / 2));
		a2.SetY(centerRect.GetY() + (rect->GetWidth() / 2));

		a3.SetX(centerRect.GetX() + (rect->GetLength() / 2));
		a3.SetY(centerRect.GetY() - (rect->GetWidth() / 2));

		a4.SetX(centerRect.GetX() - (rect->GetLength() / 2));
		a4.SetY(centerRect.GetY() - (rect->GetWidth() / 2));

		//we're going from the center of the circle to the closest possible point of contact of the rectangle.
		float closestX = clamp(centerCirc.GetX(), a1.GetX(), a2.GetX());

		float closestY = clamp(centerCirc.GetY(), a3.GetY(), a2.GetY());

		float dx = centerCirc.GetX() - closestX;
		float dy = centerCirc.GetY() - closestY;

		float squareDist = pow(dx,2) + pow(dy,2);
		float distance = sqrt(squareDist);

		if(distance <= circ->GetRadius())
		{
			res.collided = true;
			res.depth = distance;
			Vector2 normal(dx,dy);
			if(distance == 0)
			{
				normal = Vector2(0,1);
			}
			else{
				normal = Vector2::Normalize(normal);
			}
			
			res.normal = normal;
			res.overlap = circ->GetRadius() - distance;
		}
		return res;
	}

	CollisionResult RigidBody::DetectRectangleRectangleCollision(CollisionShape *shapeA, CollisionShape *shapeB, RigidBody &_a, RigidBody &_b)
	{
		CollisionResult res;

		res.type = RECTANGLE_RECTANGLE;

		CollisionRectangle *rectA = static_cast<CollisionRectangle *>(shapeA);
		CollisionRectangle *rectB = static_cast<CollisionRectangle *>(shapeB);

		Vector2 offA = rectA->GetOffset();
		Vector2 offB = rectB->GetOffset();

		Vector2 centerA = _a.GetPosition() + offA;
		Vector2 centerB = _b.GetPosition() + offB;

		Vector2 a1, a2, a3, a4; // vertices of rectangle A

		a1.SetX(centerA.GetX() - (rectA->GetLength() / 2));
		a1.SetY(centerA.GetY() + (rectA->GetWidth() / 2));

		a2.SetX(centerA.GetX() + (rectA->GetLength() / 2));
		a2.SetY(centerA.GetY() + (rectA->GetWidth() / 2));

		a3.SetX(centerA.GetX() + (rectA->GetLength() / 2));
		a3.SetY(centerA.GetY() - (rectA->GetWidth() / 2));

		a4.SetX(centerA.GetX() - (rectA->GetLength() / 2));
		a4.SetY(centerA.GetY() - (rectA->GetWidth() / 2));

		Vector2 b1, b2, b3, b4; // vertices of rectangle B

		b1.SetX(centerB.GetX() - (rectB->GetLength() / 2));
		b1.SetY(centerB.GetY() + (rectB->GetWidth() / 2));

		b2.SetX(centerB.GetX() + (rectB->GetLength() / 2));
		b2.SetY(centerB.GetY() + (rectB->GetWidth() / 2));

		b3.SetX(centerB.GetX() + (rectB->GetLength() / 2));
		b3.SetY(centerB.GetY() - (rectB->GetWidth() / 2));

		b4.SetX(centerB.GetX() - (rectB->GetLength() / 2));
		b4.SetY(centerB.GetY() - (rectB->GetWidth() / 2));

		if (!(a2.GetX() < b1.GetX() || a1.GetX() > b2.GetX() || a3.GetY() > b2.GetY() || a1.GetY() < b4.GetY())) // checks to see if they overlap
		{
			res.collided = true;

			float overlapX1 = b2.GetX() - a1.GetX();
			float overlapX2 = a2.GetX() - b1.GetX();
			float overlapX = std::min(overlapX1, overlapX2);

			float overlapY1 = b2.GetY() - a4.GetY();
			float overlapY2 = a1.GetY() - b4.GetY();
			float overlapY = std::min(overlapY1, overlapY2);

			res.overlapX = overlapX;
			res.overlapY = overlapY;
			res.overlapX1 = overlapX1;
			res.overlapX2 = overlapX2;
			res.overlapY1 = overlapY1;
			res.overlapY2 = overlapY2;
		}
		return res;
	}

	CollisionResult RigidBody::DetectCircleCircleCollision(CollisionShape *_shapeA, CollisionShape *_shapeB, RigidBody& _a, RigidBody& _b)
	{
		CollisionResult res;

		res.type = CIRCLE_CIRCLE;

		CollisionCircle *circA = static_cast<CollisionCircle *>(_shapeA);
		CollisionCircle *circB = static_cast<CollisionCircle *>(_shapeB);

		Vector2 offA = circA->GetOffset();
		Vector2 offB = circB->GetOffset();

		Vector2 centerA = _a.GetPosition() + offA;
		Vector2 centerB = _b.GetPosition() + offB;

		float dist = sqrt(pow(centerA.GetX() - centerB.GetX(), 2) + pow(centerA.GetY() - centerB.GetY(), 2));

		float radii = circA->GetRadius() + circB->GetRadius();

		float depth = radii - dist;

		Vector2 normal = Vector2::Normalize(centerB - centerA);

		res.depth = depth;
		res.radii = radii;
		res.normal = normal;
		res.collided = (dist < radii);
		
		return res;
	}

	void RigidBody::ResolveCollision(RigidBody &_a, RigidBody &_b)
	{
		CollisionResult res = DetectCollision(_a, _b);
		
		if(!res.collided)
			return;

		if (res.type == CIRCLE_CIRCLE)
			ResolveCircleCircleCollision(res, _a, _b);
		else if (res.type == RECTANGLE_RECTANGLE)
			ResolveRectangleRectangleCollision(res, _a, _b);
		else if (res.type == CIRCLE_RECTANGLE)
		{
			if(typeid(*_a.GetColShape()) == typeid(CollisionCircle))
				ResolveCircleRectangleCollision(res, _a, _b);
			else
				ResolveCircleRectangleCollision(res, _b, _a);
		}
	}

	void RigidBody::ResolveCircleCircleCollision(CollisionResult _res, RigidBody &_a, RigidBody &_b)
	{
		Vector2 normal = _res.normal;
		float depth = _res.depth;
		Vector2 aPos = _a.GetPosition();
		Vector2 bPos = _b.GetPosition();
		Vector2 changeA = normal * -1 * (depth / 2);
		Vector2 changeB = normal * (depth / 2);

		if (_a.IsStatic())
		{
			Vector2 change = normal * depth;
			Vector2 move = bPos + change;
			_b.SetPosition((move));
		}
		else if (_b.IsStatic())
		{
			Vector2 change = normal * depth * -1;
			Vector2 move = aPos + change;
			_a.SetPosition((move));
		}
		else
		{
			_a.SetPosition(aPos + changeA);
			_b.SetPosition(bPos + changeB);
		}
	}

	void RigidBody::ResolveRectangleRectangleCollision(CollisionResult _res, RigidBody &_a, RigidBody &_b)
	{
		float moveX=0;
		float moveY=0;

		if(_res.overlapX < _res.overlapY)
		{
			if(_a.GetPosition().GetX() < _b.GetPosition().GetX())
				moveX = -1 * _res.overlapX; //we'll be moving a to the left
			else
				moveX = _res.overlapX; //move a to the right
		}
		else
		{
			if(_a.GetPosition().GetY() < _b.GetPosition().GetY())
				moveY = -1 * _res.overlapY; //move a down
			else
				moveY = _res.overlapY; //move a up
		}

		Vector2 move(moveX,moveY);

		if(_a.IsStatic())
		{
			Vector2 test = _b.GetPosition() - move;
			Vector2 move = test;
			_b.SetPosition(move);
		}
		else if(_b.IsStatic())
		{
			Vector2 test = _a.GetPosition() + move;
			Vector2 move = test;
			_a.SetPosition(move);
		}
		else
		{
			Vector2 test = move * 0.5f;
			move = test;
			Vector2 moveA = _a.GetPosition() + move;

			Vector2 moveB = _b.GetPosition() - move;

			_a.SetPosition(moveA);
			_b.SetPosition(moveB);
		}

		
		
	}

	void RigidBody::ResolveCircleRectangleCollision(CollisionResult _res, RigidBody &_circle, RigidBody &_rect)
	{

		if(_circle.IsStatic())
		{
			Vector2 move = -1 * _res.normal * _res.overlap;
			Vector2 rectPos = _rect.GetPosition();
			Vector2 pos = rectPos + move;

			_rect.SetPosition(pos);
		}
		else if(_rect.IsStatic())
		{
			Vector2 move = _res.normal * _res.overlap;
			Vector2 circPos = _circle.GetPosition();
			Vector2 pos = circPos + move;

			_circle.SetPosition(pos);
		}
		else
		{
			Vector2 moveCirc = _res.normal * (_res.overlap/2);
			Vector2 moveRect = -1 * _res.normal * (_res.overlap/2);

			Vector2 rectPos = _rect.GetPosition();
			Vector2 circPos = _circle.GetPosition();

			Vector2 pos = rectPos + moveRect;

			_rect.SetPosition(pos);
			
			pos = circPos + moveCirc;

			_circle.SetPosition(pos);
		}
		
	}

void RigidBody::ResolveAllCollisions(std::vector <RigidBody>& bodies)
{
	for(int i=0; i<bodies.size(); i++)
	{
		for(int j=i+1; j<bodies.size(); j++)
		{
			ResolveCollision(bodies[i], bodies[j]);
		}
	}
}

std::vector <RigidBody> RigidBody::DetectTrigger(std::vector <RigidBody>& bodies)
{
	CollisionResult res;
	std::vector <RigidBody> BodyPair;	


	for(int i=0; i<bodies.size(); i++)
	{
		for(int j=i+1; j<bodies.size(); j++)
		{
			res = DetectCollision(bodies[i], bodies[j]);
			if(res.collided)
			{
				if(bodies[j].GetColShape()->IsTrigger())
				{
					BodyPair.push_back(bodies[i]);
					BodyPair.push_back(bodies[j]);
				}

				else if(bodies[i].GetColShape()->IsTrigger())
				{
					BodyPair.push_back(bodies[j]);
					BodyPair.push_back(bodies[i]);
				}
			}

		}
	}
	return BodyPair; //the rigid body that stepped on trigger is on evens, the trigger is on odds
}

std::vector <RigidBody> RigidBody::ReturnCollisions(std::vector <RigidBody>& bodies)
{
	CollisionResult res;
	std::vector <RigidBody> BodyPair;	


	for(int i=0; i<bodies.size(); i++)
	{
		for(int j=i+1; j<bodies.size(); j++)
		{
			res = DetectCollision(bodies[i], bodies[j]);
			if(res.collided)
			{			
				BodyPair.push_back(bodies[i]);
				BodyPair.push_back(bodies[j]);
			}

		}
	}
	return BodyPair;
}




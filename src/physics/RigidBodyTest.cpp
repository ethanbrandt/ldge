#include<iostream>
#include"RigidBody.h"

using namespace std;

int main()
{
    Vector2 bPos(1,1);
    Vector2 bVel(2,2);
    CollisionCircle* circle;
    RigidBody a;
    RigidBody b(bPos, bVel, 1, circle);
    bool test;

    if(a.GetPosition().GetX() == 0)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(a.GetPosition().GetY() == 0)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(a.GetVelocity().GetX() == 0)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(a.GetVelocity().GetY() == 0)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(a.GetMass() == 0)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }


    if(b.GetPosition().GetX() == bPos.GetX())
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(b.GetPosition().GetY() == bPos.GetY())
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(b.GetVelocity().GetX() == bVel.GetX())
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(b.GetVelocity().GetY() == bVel.GetY())
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(b.GetMass() == 1)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(typeid(b.GetColShape()) == typeid(CollisionCircle))
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    a.SetMass(3);
    Vector2 newApos(3,4);
    Vector2 newAvel(5,5);
    Vector2 off(0,0);
    CollisionRectangle* rectangle = new CollisionRectangle(2, 2, off, false, 0);
    a.SetPosition(newApos);
    a.SetVelocity(newAvel);
    a.SetShape(rectangle);


    if(a.GetPosition().GetX() == 3)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(a.GetPosition().GetY() == 4)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(a.GetVelocity().GetX() == 5)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(a.GetVelocity().GetY() == 5)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(a.GetMass() == 3)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    Vector2 positionA(2,1);
    a.SetPosition(positionA);

    //RigidBody::ResolveCollision(b,a);

    cout<<b.GetPosition().GetX()<<b.GetPosition().GetY();
    
    delete rectangle;
    return 0;
}
#include<iostream>
#include"RigidBody.h"
#include<cmath>

using namespace std;

int main()
{
    Vector2 bPos(1,1);
    Vector2 bVel(2,2);
    CollisionCircle* circle = new CollisionCircle;
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

    if(typeid(*b.GetColShape()) == typeid(CollisionCircle))
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

    CollisionCircle* shapeC = new CollisionCircle(5, Vector2(0,0), false, 0);
    CollisionCircle* shapeD = new CollisionCircle(5, Vector2(0,0), false, 0);
    RigidBody c(Vector2(0,0), Vector2(0,0), 1, shapeC);
    RigidBody d(Vector2(1,1), Vector2(0,0), 1, shapeD);

    RigidBody::ResolveCollision(c,d);

    cout<<"c was pushed to "<<c.GetPosition().GetX()<<" "<<c.GetPosition().GetY()<<"\n";
    cout<<"d was pushed to "<<d.GetPosition().GetX()<<" "<<d.GetPosition().GetY()<<"\n";
    float distance = sqrt(pow(c.GetPosition().GetX() - d.GetPosition().GetX(),2) + pow(c.GetPosition().GetY() - d.GetPosition().GetY(),2));
    
    if(distance == 10)
    {
        test = true;
        cout<<test<<" Circle-Circle collision pushed circles the appropriate distance (look above to see where they were pushed)\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    CollisionRectangle* shapeE = new CollisionRectangle(5, 5, Vector2(0,0), false, 0);
    CollisionRectangle* shapeF = new CollisionRectangle(5, 5, Vector2(0,0), false, 0);
    RigidBody e(Vector2(0,0), Vector2(0,0), 1, shapeE);
    RigidBody f(Vector2(0,1), Vector2(0,0), 1, shapeF);

    RigidBody::ResolveCollision(e,f);
    

    cout<<"e was pushed to "<<e.GetPosition().GetX()<<" "<<e.GetPosition().GetY()<<"\n";
    cout<<"f was pushed to "<<f.GetPosition().GetX()<<" "<<f.GetPosition().GetY()<<"\n";
    distance = sqrt(pow(e.GetPosition().GetX() - f.GetPosition().GetX(),2) + pow(e.GetPosition().GetY() - f.GetPosition().GetY(),2));
    
    
    if(distance == 5)
    {
        test = true;
        cout<<test<<" Rect-Rect collision pushed rectangles the appropriate distance (look above to see where they were pushed)\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    CollisionCircle* shapeG = new CollisionCircle(5, Vector2(0,0), false, 0);
    CollisionRectangle* shapeH = new CollisionRectangle(5, 5, Vector2(0,0), false, 0);
    RigidBody g(Vector2(0,0), Vector2(0,0), 1, shapeE);
    RigidBody h(Vector2(0,3), Vector2(0,0), -1, shapeF);


    RigidBody::ResolveCollision(g,h);
    

    cout<<"g was pushed to "<<e.GetPosition().GetX()<<" "<<g.GetPosition().GetY()<<"\n";
    cout<<"h was pushed to "<<h.GetPosition().GetX()<<" "<<h.GetPosition().GetY()<<"\n";
    distance = sqrt(pow(g.GetPosition().GetX() - h.GetPosition().GetX(),2) + pow(g.GetPosition().GetY() - h.GetPosition().GetY(),2));
    cout<<distance<<"\n";


    delete shapeC;
    delete shapeD;
    delete shapeE;
    delete shapeF;
    delete circle;
    delete rectangle;
    return 0;
}
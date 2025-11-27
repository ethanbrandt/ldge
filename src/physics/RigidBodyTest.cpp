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
    RigidBody b(bPos, bVel, 1, circle, true);
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

    if(a.IsStatic() == false)
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

    if(b.IsStatic() == true)
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
    RigidBody c(Vector2(0,0), Vector2(0,0), 1, shapeC, false);
    RigidBody d(Vector2(1,1), Vector2(0,0), 1, shapeD, false);

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

    
    RigidBody d_static(Vector2(1,1), Vector2(0,0), 1, shapeD, true);
    c.SetPosition(Vector2(0,0));
    RigidBody::ResolveCollision(c,d_static);
    cout<<"c was pushed to "<<c.GetPosition().GetX()<<" "<<c.GetPosition().GetY()<<"\n";
    cout<<"d_static was pushed to "<<d_static.GetPosition().GetX()<<" "<<d_static.GetPosition().GetY()<<"\n";
    distance = sqrt(pow(c.GetPosition().GetX() - d_static.GetPosition().GetX(),2) + pow(c.GetPosition().GetY() - d_static.GetPosition().GetY(),2));
    
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

    RigidBody c_static(Vector2(0,0), Vector2(0,0), 1, shapeC, true);
    d.SetPosition(Vector2(1,1));
    RigidBody::ResolveCollision(c_static,d);
    cout<<"c_static was pushed to "<<c_static.GetPosition().GetX()<<" "<<c_static.GetPosition().GetY()<<"\n";
    cout<<"d was pushed to "<<d.GetPosition().GetX()<<" "<<d.GetPosition().GetY()<<"\n";
    distance = sqrt(pow(c_static.GetPosition().GetX() - d.GetPosition().GetX(),2) + pow(c_static.GetPosition().GetY() - d.GetPosition().GetY(),2));
    
    
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
    RigidBody e(Vector2(0,0), Vector2(0,0), 1, shapeE, false);
    RigidBody f(Vector2(0,1), Vector2(0,0), 1, shapeF, false);

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


    RigidBody e_static(Vector2(0,0), Vector2(0,0), 1, shapeE, true);
    f.SetPosition(Vector2(0,1));
    RigidBody::ResolveCollision(e_static,f);

    cout<<"e_static was pushed to "<<e_static.GetPosition().GetX()<<" "<<e_static.GetPosition().GetY()<<"\n";
    cout<<"f was pushed to "<<f.GetPosition().GetX()<<" "<<f.GetPosition().GetY()<<"\n";
    distance = sqrt(pow(e_static.GetPosition().GetX() - f.GetPosition().GetX(),2) + pow(e_static.GetPosition().GetY() - f.GetPosition().GetY(),2));
    cout<<distance<<"\n";
    
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

    RigidBody f_static(Vector2(0,1), Vector2(0,0), 1, shapeF, true);
    e.SetPosition(Vector2(0,0));
    RigidBody::ResolveCollision(e,f_static);

    cout<<"e was pushed to "<<e.GetPosition().GetX()<<" "<<e.GetPosition().GetY()<<"\n";
    cout<<"f was pushed to "<<f_static.GetPosition().GetX()<<" "<<f_static.GetPosition().GetY()<<"\n";
    distance = sqrt(pow(e.GetPosition().GetX() - f_static.GetPosition().GetX(),2) + pow(e.GetPosition().GetY() - f_static.GetPosition().GetY(),2));
    cout<<distance<<"\n";
    
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
    RigidBody g(Vector2(0,0), Vector2(0,0), 1, shapeE, false);
    RigidBody h(Vector2(0,3), Vector2(0,0), -1, shapeF, false);


    RigidBody::ResolveCollision(g,h);
    

    cout<<"g was pushed to "<<e.GetPosition().GetX()<<" "<<g.GetPosition().GetY()<<"\n";
    cout<<"h was pushed to "<<h.GetPosition().GetX()<<" "<<h.GetPosition().GetY()<<"\n";
    distance = sqrt(pow(g.GetPosition().GetX() - h.GetPosition().GetX(),2) + pow(g.GetPosition().GetY() - h.GetPosition().GetY(),2));
    
    if(distance == 5)
    {
        test = true;
        cout<<test<<" Circ-Rect collision pushed rectangles the appropriate distance (look above to see where they were pushed)\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    
    RigidBody i(Vector2(0,0), Vector2(0,0), 1, shapeE, false);
    RigidBody j(Vector2(0,3), Vector2(0,0), -1, shapeF, false);


    RigidBody::ResolveCollision(j,i);
    

    cout<<"i was pushed to "<<i.GetPosition().GetX()<<" "<<i.GetPosition().GetY()<<"\n";
    cout<<"j was pushed to "<<j.GetPosition().GetX()<<" "<<j.GetPosition().GetY()<<"\n";
    distance = sqrt(pow(i.GetPosition().GetX() - j.GetPosition().GetX(),2) + pow(i.GetPosition().GetY() - j.GetPosition().GetY(),2));
    
    if(distance == 5)
    {
        test = true;
        cout<<test<<" Circ-Rect collision pushed rectangles the appropriate distance (look above to see where they were pushed)\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    RigidBody k(Vector2(0,0), Vector2(0,0), 1, shapeE, true);
    RigidBody l(Vector2(0,3), Vector2(0,0), -1, shapeF, false);


    RigidBody::ResolveCollision(k,l);
    

    cout<<"k was pushed to "<<k.GetPosition().GetX()<<" "<<k.GetPosition().GetY()<<"\n";
    cout<<"l was pushed to "<<l.GetPosition().GetX()<<" "<<l.GetPosition().GetY()<<"\n";
    distance = sqrt(pow(k.GetPosition().GetX() - l.GetPosition().GetX(),2) + pow(k.GetPosition().GetY() - l.GetPosition().GetY(),2));
    
    if(distance == 5)
    {
        test = true;
        cout<<test<<" Circ-Rect collision pushed rectangles the appropriate distance (look above to see where they were pushed)\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    RigidBody m(Vector2(0,0), Vector2(0,0), 1, shapeE, false);
    RigidBody n(Vector2(0,3), Vector2(0,0), -1, shapeF, true);


    RigidBody::ResolveCollision(m,n);
    

    cout<<"m was pushed to "<<m.GetPosition().GetX()<<" "<<m.GetPosition().GetY()<<"\n";
    cout<<"n was pushed to "<<n.GetPosition().GetX()<<" "<<n.GetPosition().GetY()<<"\n";
    distance = sqrt(pow(m.GetPosition().GetX() - n.GetPosition().GetX(),2) + pow(m.GetPosition().GetY() - n.GetPosition().GetY(),2));
    
    if(distance == 5)
    {
        test = true;
        cout<<test<<" Circ-Rect collision pushed rectangles the appropriate distance (look above to see where they were pushed)\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }


    delete shapeC;
    delete shapeD;
    delete shapeE;
    delete shapeF;
    delete circle;
    delete rectangle;
    return 0;
}
#define _USE_MATH_DEFINES

#include<iostream>
#include"CollisionShape.h"
#include"CollisionCircle.h"
#include"CollisionRectangle.h"
#include<cmath>


using namespace std;

int main()
{
    CollisionCircle circle;
    CollisionRectangle rect;

    bool test;

    if(circle.GetRadius() == 1 && circle.GetColMask() == 0 && circle.IsTrigger() == false && circle.GetOffset().GetMagnitude() == 0)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(rect.GetWidth() == 1 && rect.GetLength() == 1 && rect.GetColMask() == 0 && rect.IsTrigger() == false && rect.GetOffset().GetMagnitude() == 0)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }
    Vector2 posOff(4,3);
    CollisionCircle circleA(2, posOff, true, 1);
    CollisionRectangle rectA(2,4, posOff, false, 2);

    if(circleA.GetRadius() == 2)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(circleA.GetColMask() == 1)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(circleA.IsTrigger() == true)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(circleA.GetOffset().GetMagnitude() == 5)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(circleA.GetColMask() == 1)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }
    

    if(rectA.GetWidth() == 2 && rectA.GetLength() == 4)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(rectA.IsTrigger() == false)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(rectA.GetOffset().GetMagnitude() == 5)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(rectA.GetColMask() == 2)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    circleA.SetRadius(3);

    if(ceil(circleA.GetArea()) == ceil((9 * M_PI)))
    {
        test = true;
        cout<<test<<"\n";
        
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    rectA.SetLength(6);
    rectA.SetWidth(3);

    if(rectA.GetArea() == 18)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }


    return 0;
}
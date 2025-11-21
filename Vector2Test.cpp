#include<iostream>
#include"Vector2.h"

using namespace std;


int main()
{
    Vector2 a;
    Vector2 b(2,1);
    bool test;

    if(a.getX() == 0 && a.getY() == 0)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }
    
    if(b.getX() == 2 && b.getY() == 1)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    a.setX(4);
    a.setY(3);

    if(a.getX() == 4 && a.getY() == 3)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(a.getMagnitude() == 5)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    Vector2 c = Vector2::Normalize(a);

    if(c.getMagnitude() == 1)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    c = a + b;

    if(c.getX() == 6 && c.getY() == 4)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    c = a - b;

    if(c.getX() == 2 && c.getY() == 2)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    float d = Vector2::dot(a,b);

    if(d == 11)
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
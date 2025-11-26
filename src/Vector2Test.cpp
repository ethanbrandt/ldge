#include<iostream>
#include"Vector2.h"

using namespace std;


int main()
{
    Vector2 a;
    Vector2 b(2,1);
    bool test;

    if(a.GetX() == 0 && a.GetY() == 0)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }
    
    if(b.GetX() == 2 && b.GetY() == 1)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    a.SetX(4);
    a.SetY(3);

    if(a.GetX() == 4 && a.GetY() == 3)
    {
        test = true;
        cout<<test<<"\n";
    }
    else
    {
        test = false;
        cout<<test<<"\n";
    }

    if(a.GetMagnitude() == 5)
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

    if(c.GetMagnitude() == 1)
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

    if(c.GetX() == 6 && c.GetY() == 4)
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

    if(c.GetX() == 2 && c.GetY() == 2)
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
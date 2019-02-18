#include <iostream>

using namespace std;

/*** Part 1 ***/
/*
struct A
{
    float data;
};

struct B
{
    A a;
};

struct C
{
    B* b;
};

struct D
{
    C c;
};

int main(){

    D* d = new D;

    d->c.b = new B;

    cout << d->c.b->a.data << endl;

    d->c.b->a.data = 2;

    cout << d->c.b->a.data << endl;



    return 0;
}
*/
struct B
{
    float d, e;
};

struct A
{
    float a, b, c;
    B* ptr;
    B mem;
};



A stack;


int main(){


cout << &stack << endl;
cout << &stack.a << endl;
cout << &stack.b << endl;
cout << &stack.c << endl;

cout << &stack.ptr << endl;

cout << &stack.mem << endl;
cout << &stack.mem.d << endl;
cout << &stack.mem.e << endl;

    return 0;
}


/*** Part 3 ***/
/*
struct point
{
    float x, y;
};

void foo(point& p1, point p2)
{
    point p3;
    p3.x = p1.x + p2.x;
    p3.y = p1.y + p2.y;
    bar(p3);
    p1.x -= p3.x;
    p1.y -= p3.y;
}

void bar(point p3)
{
    p3.x *= .5f;
    p3.y *= .5f;
}

int main(){

    point a, b;
    a.x = b.y = 1;
    a.y = b.x = 0;
    foo(a, b);

    return 0;
}
*/
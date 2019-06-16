#include <cstdio>

class A {
public:
    virtual int cat() {return 3;}
};

class B: public A {
public:
    virtual int cat() {return 5;}
    int b;
    B() { b = 113; }
};

int main() {
    A* pa = new B();
    printf("Cat: %d\n", pa->cat());
    reinterpret_cast<B*>(pa)->b = 1928344;
    printf("Cat': %d\n", reinterpret_cast<B*>(pa)->b);
    return 0;
}

#include <cmath>

#include "Entity.hpp"
#include "Method.hpp"
#include "Property.hpp"
#include "Signal.hpp"
using namespace std::placeholders;
struct Vector4 {
  double x, y, z, w;

  Vector4(double x, double y, double z, double w) :
    x(x),
    y(y),
    z(z),
    w(w) {
  }

  bool operator==(const Vector4 &o) const {
    return x == o.x && y == o.y && z == o.z && w == o.w;
  }

  operator double() const {
    return std::sqrt(x*x + y*y + z*z + w*w);
  }
};

class MyEntity : public virtual Entity {
protected:
  int m_health;
  Vector4 m_remainingInk = Vector4(1, 2, 3, 4);

public:
  Property<int, PropertyAccess::RW> health;
  Property<Vector4, PropertyAccess::R> remainingInk;
  Method<int(int, int)> myMethod;
  Signal<int, int> signalr;

  MyEntity() :
    Entity(),
    m_health(1337),
    health("health", this, &m_health),
    remainingInk("remainingInk", this, &m_remainingInk),
    myMethod("myMethod", this, &MyEntity::myMethodImpl, _1, _2),
    signalr("signalr", this) {
  }

  ~MyEntity() {
  }

  int myMethodImpl(int a, int b) {
    signalr(a + 2, b - 4);
    return 42;
  }
};

#include <assert.h>
#include <iostream>

class CustomThingy {
public:
  int val;

  CustomThingy(int v) :
    val(v) {
  }

  operator int() const {
    return val;
  }
};

void testPrimitiveAssign() {
    MyEntity mayo;

    // Initial value
    assert(*mayo.health == 1337);

    // Primitive same-type operator=
    mayo.health = 12;
    assert(*mayo.health == 12);

    // Implicit cast other-type lvalue operator=
    CustomThingy ct(-237);
    mayo.health = ct;
    assert(*mayo.health == -237);

    // Implicit cast other-type rvalue operator=
    mayo.health = CustomThingy(8);
    assert(*mayo.health == 8);

    // Implicit cast other-type std::move'd operator=
    mayo.health = std::move(CustomThingy(33));
    assert(*mayo.health == 33);
}

void testClassCompareOperators() {
    MyEntity mayo;
    Vector4 test(1, 2, 3, 4), test2(2, 2, 3, 4);

    assert(*mayo.remainingInk == test);
    //assert(mayo.remainingInk == "test");
    //assert(mayo.remainingInk != test2);
}

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  testPrimitiveAssign();
  testClassCompareOperators();

  MyEntity mayo;

  mayo.signalr.addListener([](Entity&, int a, int b) {
    std::cout << a << ' ' << b << std::endl;
  });
  mayo.signalr.addListener([](Entity&, int a, int b) {
    std::cout << b << ' ' << a << std::endl;
  });
  mayo.signalr(12, 34);


  mayo.myMethod(0, 0);

  return 0;
}

#include <cmath>
#include <iostream>

#include "Entity.hpp"
#include "Method.hpp"
#include "Property.hpp"
#include "Signal.hpp"

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
  bool operator!=(const Vector4 &o) const {
    return !operator==(o);
  }

  operator double() const {
    return std::sqrt(x*x + y*y + z*z + w*w);
  }
};

#include <assert.h>

void testPrimitiveAssign() {
  class MyEntity : public virtual Entity {
  protected:
    int m_health;
  public:
    Property<int, PropertyAccess::RW> health;
    MyEntity() :
      Entity(),
      m_health(1337),
      health("health", this, &m_health) {
    }
    ~MyEntity() {
    }
  } mayo;

  // Initial value
  assert(*mayo.health == 1337);

  // Primitive same-type lvalue operator=
  int newHealth = -888;
  mayo.health = newHealth;
  assert(*mayo.health == -888);

  // Primitive same-type rvalue operator=
  mayo.health = 12;
  assert(*mayo.health == 12);

  // Primitive other-type lvalue operator=
  short newHealth2 = -212;
  mayo.health = newHealth2;
  assert(*mayo.health == -212);

  // Primitive other-type rvalue operator=
  mayo.health = static_cast<short>(31415);
  assert(*mayo.health == 31415);

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
  class MyEntity : public virtual Entity {
  protected:
    Vector4 m_remainingInk = Vector4(1, 2, 3, 4);
  public:
    Property<Vector4, PropertyAccess::R> remainingInk;
    MyEntity() :
      Entity(),
      remainingInk("remainingInk", this, &m_remainingInk) {
    }
    ~MyEntity() {
    }
  } mayo;
  Vector4 test(1, 2, 3, 4), test2(2, 2, 3, 4);

  assert(*mayo.remainingInk == test);
  assert(*mayo.remainingInk != test2);
}

void testSignals() {
  class MyEntity : public virtual Entity {
  public:
    int aField;
    Signal<int, int> signalr;
    MyEntity() :
      Entity(),
      aField(6543),
      signalr("signalr", this) {
    }
    ~MyEntity() {
    }
  } mayo;

  mayo.signalr.addListener<Entity>([](Entity &e, int a, int b) {
    assert(a == 12);
    assert(b == 34);
    MyEntity *pe = dynamic_cast<MyEntity*>(&e);
    assert(pe != nullptr);
    assert(pe->aField == 6543);
  });
  mayo.signalr.addListener<MyEntity>([](MyEntity &e, int a, int b) {
    assert(a == 12);
    assert(b == 34);
    assert(e.aField == 6543);
  });
  mayo.signalr.addListener([](int a, int b) {
    assert(a == 12);
    assert(b == 34);
  });
  mayo.signalr(12, 34);
}

void testMethod() {
  class MyEntity : public virtual Entity {
  public:
    Method<int(int, int)> myMethod, myMethod2, myMethod3, myMethod4;
    MyEntity() :
      Entity(),
      myMethod("myMethod", this, &MyEntity::myMethodImpl),
      myMethod2("myMethod2", this, &myMethod2Impl),
      myMethod3("myMethod3", this, &myMethod3Impl),
      myMethod4("myMethod4", this, &myMethod4Impl) {
    }
    ~MyEntity() {
    }
    int myMethodImpl(int a, int b) {
      assert(a == 731);
      assert(b == -472);
      return 42;
    }
    static int myMethod2Impl(Entity &self, int a, int b) {
      assert(a == 0);
      assert(b == 1);
      std::cout << "Hello I'm method2 " << std::addressof(self) << std::endl;
      return 42*42;
    }
    static int myMethod3Impl(MyEntity &self, int a, int b) {
      assert(a == 0);
      assert(b == 1);
      std::cout << "Hello I'm method3 " << std::addressof(self) << std::endl;
      return 42*42*42;
    }
    static int myMethod4Impl(const MyEntity &self, int a, int b) {
      assert(a == 0);
      assert(b == 1);
      std::cout << "Hello I'm method4 with const " << std::addressof(self) << std::endl;
      return 42*42*42*42;
    }
  } mayo;

  assert(mayo.myMethod(731, -472) == 42);
  assert(mayo.myMethod2(0, 1) == 42*42);
  assert(mayo.myMethod3(0, 1) == 42*42*42);
  assert(mayo.myMethod4(0, 1) == 42*42*42*42);
}

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  testPrimitiveAssign();
  testClassCompareOperators();
  testSignals();
  testMethod();

  return 0;
}

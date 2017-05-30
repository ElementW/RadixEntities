#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

struct EntityIoValueType {
  uint32_t namespc;
  uint32_t type;
}


class PropertyBase {
  std::string name;
  EntityIoValueType valueType;
};

template<class R, typename... Args>
class Method<R(Args...)> : public std::function<R(Args...)> {
public:
  // TODO: forward std::function args
}


class Entity {
protected:
  std::unordered_map<std::string, std::PropertyBase*> m_properties;
};


enum PropertyAccess {
  R = 1,
  W = 2,
  RW = 3,
};

template<std::string N, typename T, PropertyAccess A>
class Property : public PropertyBase {
public:
  // TODO: type-erased callbacks

  using ReadRawFuncPtr = std::remove_reference_t<T>&&(*)(Entity&);
  using ReadStdFuncPtr = std::function<std::remove_reference_t<T>&&(Entity&)>;

  // Could be changed to a T&& new value param
  using WriteRawFuncPtr = std::remove_reference_t<T>&&(*)(Entity&, const T &newval);
  using WriteStdFuncPtr = std::function<std::remove_reference_t<T>&&(Entity&, const T &newval)>;

  using OnChangeRawFuncPtr = void(*)(Entity&, const T &newval);
  using OnChangeStdFuncPtr = std::function<void(Entity&, const T &newval)>;

protected:
  T *m_valuePtr;
  OnChangeStdFuncPtr m_onChange;

public:
  Property(Entity *container, const T *valuePtr, OnChangeStdFuncPtr onChange) :
    m_valuePtr(valuePtr) {
    container.m_properties.emplace_back(std::piecewise_construct,
        std::forward_as_tuple(N),
        std::forward_as_tuple(this));
  }

  operator const T&() {
    static_assert(A | PropertyAccess::R, "Property has no read access");
    return *m_value;
  }

  operator=(const T &v) {
    static_assert(A | PropertyAccess::W, "Property has no write access");
    *m_value = v;
  }
  operator=(const T &&v) {
    static_assert(A | PropertyAccess::W, "Property has no write access");
    *m_value = std::move(v);
  }
}

// TODO: synthetic properties (user get/set method)

template<std::string N, typename... Args>
class Signal {
protected:
  std::vector<std::function<void(Args...)>> m_listeners;

public:
  

  template<typename... CallArgs>
  void operator()(CallArgs&&... ca) {
    for (auto fn : m_listeners) {
      fn(std::forward<CallArgs>(ca)...);
    }
  }
}

// EXAMPLE

class MyEntity : public virtual Entity {
protected:
  int m_health;
  Vector4 m_remainingInk(1, 2, 3, 4);

public:
  Property<"health", int> health(this, &m_health);
  Property<"remainingInk", Vector4> remainingInk(this, &m_remainingInk);
  Method<"myMethod", void(int, int)> myMethod(this, &MyEntity::myMethodImpl);
  Signal<"signalr", int, int> signalr(this);

  void myMethodImpl(int a, int b) {
    signalr(a + 2, b - 4);
  }
};

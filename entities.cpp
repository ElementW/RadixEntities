#include <unordered_map>
#include <utility>

struct EntityIoValueType {
  uint32_t namespc;
  uint32_t type;
}


class PropertyBase {
  std::string name;
  EntityIoValueType valueType;
};

template<class R, class... Args>
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

template<typename T, std::string N, PropertyAccess A>
class Property : public PropertyBase {
public:
  // Could be changed to a T&& new value param
  using OnChangeRawFuncPtr = void(*)(Entity*, const T &newval);
  using OnChangeStdFuncPtr = std::function<void(Entity*, const T &newval)>;

protected:
  T m_value;
  OnChangeStdFuncPtr m_onChange;

public:
  Property(Entity *container, const T &initval, OnChangeStdFuncPtr onChange) :
    value(initval) {
    container.m_properties.emplace_back(std::piecewise_construct,
        std::forward_as_tuple(N),
        std::forward_as_tuple(this));
  }

  operator const T&() {
    static_assert(A | PropertyAccess::R, "Property has no read access");
    return m_value;
  }

  operator=(const T &v) {
    static_assert(A | PropertyAccess::W, "Property has no write access");
    m_value = v;
  }
  operator=(const T &&v) {
    static_assert(A | PropertyAccess::W, "Property has no write access");
    m_value = std::move(v);
  }
}

// TODO: synthetic properties (user get/set method)

// EXAMPLE

class MyEntity : public virtual Entity {
public:
  Property<int, "health"> health(this);
  Property<Vector4, "remainingInk"> remainingInk(this, Vector4(1, 2, 3, 4));
  Method<void(int, int)> myMethod(this, &MyEntity::);
};
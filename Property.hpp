#ifndef PROPERTY_HPP
#define PROPERTY_HPP

#include <functional>

class Entity;


struct EntityIoValueType {
  uint32_t namespc;
  uint32_t type;
};

class PropertyBase {
protected:
  EntityIoValueType valueType;

  const char *const m_name;
  Entity *m_container;

  PropertyBase(const char *name, Entity *container);
};

enum PropertyAccess {
  R = 1,
  W = 2,
  RW = 3,
};

template<typename T, PropertyAccess A>
class Property : public PropertyBase {
public:
  // TODO: type-erased callbacks

  using ReadRawFuncPtr = std::remove_reference_t<T>&&(*)(Entity&);
  using ReadStdFuncPtr = std::function<std::remove_reference_t<T>&&(Entity&)>;

  // Could be changed to a T&& new value param
  using WriteRawFuncPtr = void(*)(Entity&, const T &newval);
  using WriteStdFuncPtr = std::function<void(Entity&, const T &newval)>;

  using OnChangeRawFuncPtr = void(*)(Entity&, const T &newval);
  using OnChangeStdFuncPtr = std::function<void(Entity&, const T &newval)>;

protected:
  T *m_valuePtr;
  OnChangeStdFuncPtr m_onChange;

public:
  Property(const char *name, Entity *container, T *valuePtr, OnChangeStdFuncPtr onChange = {}) :
    PropertyBase(name, container) {
    m_valuePtr = valuePtr;
    m_onChange = onChange;
  }

  operator const T&() const {
    static_assert(A | PropertyAccess::R, "Property has no read access");
    return *m_valuePtr;
  }

  template<typename C, typename = typename std::enable_if_t<std::is_convertible<C, T>::value>>
  const T& operator=(const C &v) {
    static_assert(A | PropertyAccess::W, "Property has no write access");
    //if (m_onChange) {
    //  m_onChange(
    //}
    *m_valuePtr = v;
    return *m_valuePtr;
  }
  template<typename C, typename = typename std::enable_if_t<std::is_convertible<C, T>::value>>
  const T& operator=(T &&v) {
    static_assert(A | PropertyAccess::W, "Property has no write access");
    *m_valuePtr = std::move(v);
    return *m_valuePtr;
  }
};

// TODO: synthetic properties (user get/set method)

#endif /* PROPERTY_H */

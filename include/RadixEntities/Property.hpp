#ifndef RADIXENTITIES_PROPERTY_HPP
#define RADIXENTITIES_PROPERTY_HPP

#include <functional>
#include <string>
#include <utility>

#include "iotypes/ValueType.hpp"

namespace RadixEntities {

class Entity;

enum PropertyAccess {
  R = 1,
  W = 2,
  RW = 3,
};

class PropertyBase {
protected:
  const std::string m_name;
  Entity *const m_container;
  const iotypes::ValueType m_valueType;
  const PropertyAccess m_access;

  PropertyBase(std::string &&name, Entity *container, const iotypes::ValueType &valueType,
               PropertyAccess access);

public:
  const std::string& name() const {
    return m_name;
  }
  Entity* container() const {
    return m_container;
  }
  iotypes::ValueType valueType() const {
    return m_valueType;
  }
  PropertyAccess access() const {
    return m_access;
  }
  std::string str() const;
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

  constexpr iotypes::ValueType valueType() const { return iotypes::getValueType<T>(); }

protected:
  T *m_valuePtr;
  OnChangeStdFuncPtr m_onChange;

  template<typename V>
  using ei = std::enable_if_t<V::value>;

public:
  Property(std::string &&name, Entity *container, T *valuePtr, OnChangeStdFuncPtr onChange = {}) :
    PropertyBase(std::move(name), container, valueType(), A) {
    m_valuePtr = valuePtr;
    m_onChange = onChange;
  }

  const T& operator*() const {
    static_assert(A & PropertyAccess::R, "Property has no read access");
    return *m_valuePtr;
  }

  const T* operator->() const {
    static_assert(A & PropertyAccess::R, "Property has no read access");
    return m_valuePtr;
  }


  template<typename C, typename = ei<std::is_convertible<C, T>>>
  const T& operator=(const C &v) {
    static_assert(A & PropertyAccess::W, "Property has no write access");
    //if (m_onChange) {
    //  m_onChange(*container, TODO);
    //}
    *m_valuePtr = v;
    return *m_valuePtr;
  }

  template<typename C, typename = ei<std::is_convertible<C, T>>>
  const T& operator=(T &&v) {
    static_assert(A & PropertyAccess::W, "Property has no write access");
    *m_valuePtr = std::move(v);
    return *m_valuePtr;
  }
};

// TODO: synthetic properties (user get/set method)

} /* namespace RadixEntities */

#endif /* RADIXENTITIES_PROPERTY_HPP */

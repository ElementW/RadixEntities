#ifndef RADIXENTITY_METHOD_HPP
#define RADIXENTITY_METHOD_HPP

#include <functional>
#include <type_traits>
#include <utility>

#include "util/easy_bind"
#include "util/ParamTypes.hpp"

namespace radix {
namespace entity {

class Entity;

class MethodBase {
protected:
  const std::string m_name;
  Entity *const m_container;
  const iotypes::ValueType m_returnType;
  const std::vector<iotypes::ValueType> m_paramTypes;

  MethodBase(std::string &&name, Entity *container, iotypes::ValueType returnType,
             std::vector<iotypes::ValueType> &&paramTypes);

public:
  const std::string& name() const {
    return m_name;
  }
  Entity* container() const {
    return m_container;
  }
  const std::vector<iotypes::ValueType>& paramTypes() const {
    return m_paramTypes;
  }
  std::string str() const;
};

template<class R, typename... Args>
class Method {};

template<class R, typename... Args>
class Method<R(Args...)> : public MethodBase {
public:
  using Func = std::function<R(Args...)>;

protected:
  Func m_func;

public:
  /** Constructor taking a member method, and possibly some of its arguments.
   * Non-specified arguments are substituted with placeholders of their position.
   * Example:
   *     myMethod("myMethod", this, &MyEntity::myMethodImpl)
   */
  template<typename E, typename... CallArgs,
           typename = typename std::enable_if<std::is_base_of<Entity, E>::value>::type>
  Method(std::string &&name, E *container, R(E::*func)(Args...), CallArgs&&... ca) :
    MethodBase(std::move(name), container, iotypes::getValueType<R>(), getParamTypes<Args...>()),
    m_func(easy_bind(func, container, std::forward<CallArgs>(ca)...)) {
  }

  /** Constructor taking a std::function non-member method with an Entity reference parameter,
   *  and possibly some of its arguments.
   * Non-specified arguments are substituted with placeholders of their position.
   */
  template<typename D, typename E, typename... CallArgs,
           typename = typename std::enable_if<std::is_base_of<Entity, E>::value>::type,
           typename = typename std::enable_if<std::is_base_of<std::decay_t<D>, E>::value>::type>
  Method(std::string &&name, E *container, std::function<R(D&, Args...)> func, CallArgs&&... ca) :
    MethodBase(std::move(name), container, iotypes::getValueType<R>(), getParamTypes<Args...>()),
    m_func(easy_bind(func, std::ref(*container), std::forward<CallArgs>(ca)...)) {
  }

  /** Constructor taking a non-member method pointer with an Entity reference parameter, and
   *  possibly some of its arguments.
   * Non-specified arguments are substituted with placeholders of their position.
   * Example:
   *     myMethod("myMethod", this, &myMethodImpl)
   *     [...]
   *     void myMethodImpl(MyEntity &self, int a) {}
   */
  template<typename D, typename E, typename... CallArgs,
           typename = typename std::enable_if<std::is_base_of<Entity, E>::value>::type,
           typename = typename std::enable_if<std::is_base_of<std::decay_t<D>, E>::value>::type>
  Method(std::string &&name, E *container, R(*func)(D&, Args...), CallArgs&&... ca) :
    MethodBase(std::move(name), container, iotypes::getValueType<R>(), getParamTypes<Args...>()),
    m_func(easy_bind(
        std::function<R(D&, Args...)>(func), std::ref(*container), std::forward<CallArgs>(ca)...)) {
  }

  template<typename... CallArgs>
  Method(std::string &&name, Entity *container, CallArgs&&... ca) :
    MethodBase(std::move(name), container, iotypes::getValueType<R>(), getParamTypes<Args...>()),
    m_func(std::forward<CallArgs>(ca)...) {
  }

  template<typename... CallArgs>
  R operator()(CallArgs&&... ca) const {
    return m_func(std::forward<CallArgs>(ca)...);
  }
};

} /* namespace entity */
} /* namespace radix */

#endif /* RADIXENTITY_METHOD_HPP */

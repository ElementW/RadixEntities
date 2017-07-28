#ifndef RADIXENTITIES_METHOD_HPP
#define RADIXENTITIES_METHOD_HPP

#include <functional>
#include <type_traits>

#include "util/easy_bind"

namespace RadixEntities {

class Entity;

class MethodBase {
protected:
  const char *const m_name;
  Entity *m_container;

  MethodBase(const char *name, Entity *container);
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
  Method(const char *name, E *container, R(E::*func)(Args...), CallArgs&&... ca) :
    MethodBase(name, container),
    m_func(easy_bind(func, container, std::forward<CallArgs>(ca)...)) {
  }

  /** Constructor taking a std::function non-member method with an Entity reference parameter,
   *  and possibly some of its arguments.
   * Non-specified arguments are substituted with placeholders of their position.
   */
  template<typename D, typename E, typename... CallArgs,
           typename = typename std::enable_if<std::is_base_of<Entity, E>::value>::type,
           typename = typename std::enable_if<std::is_base_of<std::decay_t<D>, E>::value>::type>
  Method(const char *name, E *container, std::function<R(D&, Args...)> func, CallArgs&&... ca) :
    MethodBase(name, container),
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
  Method(const char *name, E *container, R(*func)(D&, Args...), CallArgs&&... ca) :
    MethodBase(name, container),
    m_func(easy_bind(
        std::function<R(D&, Args...)>(func), std::ref(*container), std::forward<CallArgs>(ca)...)) {
  }

  template<typename... CallArgs>
  Method(const char *name, Entity *container, CallArgs&&... ca) :
    MethodBase(name, container),
    m_func(std::forward<CallArgs>(ca)...) {
  }

  template<typename... CallArgs>
  R operator()(CallArgs&&... ca) const {
    return m_func(std::forward<CallArgs>(ca)...);
  }
};

} /* namespace RadixEntities */

#endif /* RADIXENTITIES_METHOD_HPP */

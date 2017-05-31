#ifndef METHOD_HPP
#define METHOD_HPP

#include <functional>

#include "easy_bind"

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
  using Func = std::function<R(Entity&, Args...)>;
  using Thisfunc = std::function<R(Args...)>;

protected:
  union {
    Func m_func;
    Thisfunc m_thisfunc;
  };
  bool m_isThisfunc;

public:
  template<typename E, typename... CallArgs,
           typename = typename std::enable_if<std::is_base_of<Entity, E>::value>::type>
  Method(const char *name, E *container, R(E::*func)(Args...), CallArgs&&... ca) :
    MethodBase(name, container),
    m_thisfunc(easy_bind(func, container, std::forward<CallArgs>(ca)...)),
    m_isThisfunc(true) {
  }

  template<typename... CallArgs>
  Method(const char *name, Entity *container, CallArgs&&... ca) :
    MethodBase(name, container),
    m_func(std::forward<CallArgs>(ca)...),
    m_isThisfunc(false) {
  }

  ~Method() {
    if (m_isThisfunc) {
      m_thisfunc.~Thisfunc();
    } else {
      m_func.~Func();
    }
  }

  template<typename... CallArgs>
  R operator()(CallArgs&&... ca) const {
    return m_isThisfunc ?
        m_thisfunc(std::forward<CallArgs>(ca)...) :
        m_func(*m_container, std::forward<CallArgs>(ca)...);
  }
};

#endif /* METHOD_HPP */

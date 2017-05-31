#ifndef METHOD_HPP
#define METHOD_HPP

#include <functional>

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
protected:
  std::function<R(Entity&, Args...)> m_func;

public:
  template<typename... CallArgs>
  Method(const char *name, Entity *container, CallArgs&&... ca) :
    MethodBase(name, container) {
    m_func = decltype(m_func)(std::forward<CallArgs>(ca)...);
  }

  template<typename... CallArgs>
  R operator()(CallArgs&&... ca) const {
    return m_func(*m_container, std::forward<CallArgs>(ca)...);
  }
};

#endif /* METHOD_H */

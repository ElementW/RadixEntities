#ifndef METHOD_HPP
#define METHOD_HPP

#include <functional>

class Entity;


template<class R, typename... Args>
class Method {};

template<class R, typename... Args>
class Method<R(Args...)> {
protected:
  std::function<R(Entity&, Args...)> m_func;
  const char *const m_name;
  Entity *m_container;

public:
  template<typename... CallArgs>
  Method(const char *name, Entity *container, CallArgs&&... ca) :
    m_func(std::forward<CallArgs>(ca)...),
    m_name(name),
    m_container(container) {
  }

  template<typename... CallArgs>
  R operator()(CallArgs&&... ca) const {
    return m_func(*m_container, std::forward<CallArgs>(ca)...);
  }
};

#endif /* METHOD_H */

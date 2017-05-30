#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <functional>
#include <vector>

class Entity;


template<typename... Args>
class Signal {
public:
  using Func = std::function<void(Entity&, Args...)>;

protected:
  std::vector<Func> m_listeners;
  const char *const m_name;
  Entity *m_container;

public:
  Signal(const char *name, Entity *container) :
    m_name(name),
    m_container(container) {
  }

  // TODO make holder object to functions are automatically removed upon object death
  void addListener(const Func &func) {
    m_listeners.emplace_back(func);
  }

  template<typename... CallArgs>
  void operator()(CallArgs&&... ca) const {
    for (const Func &fn : m_listeners) {
      fn(*m_container, std::forward<CallArgs>(ca)...);
    }
  }
};

#endif /* SIGNAL_H */

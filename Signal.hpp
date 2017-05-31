#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <functional>
#include <vector>

class Entity;

class SignalBase {
protected:
  const char *const m_name;
  Entity *m_container;

  SignalBase(const char *name, Entity *container);
};

template<typename... Args>
class Signal : public SignalBase {
public:
  using Func = std::function<void(Entity&, Args...)>;

protected:
  std::vector<Func> m_listeners;

public:
  Signal(const char *name, Entity *container) :
    SignalBase(name, container) {
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

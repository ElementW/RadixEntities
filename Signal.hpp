#ifndef RADIXENTITIES_SIGNAL_HPP
#define RADIXENTITIES_SIGNAL_HPP

#include <functional>
#include <type_traits>
#include <vector>

namespace RadixEntities {

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
  using Func = std::function<void(Args...)>;

protected:
  std::vector<Func> m_listeners;

public:
  Signal(const char *name, Entity *container) :
    SignalBase(name, container) {
  }

  template<typename E,
           typename = typename std::enable_if<std::is_base_of<Entity, E>::value>::type>
  void addListener(std::function<void(E&, Args...)> func) {
    m_listeners.emplace_back(easy_bind(func, std::ref(dynamic_cast<E&>(*m_container))));
  }

  // TODO make holder object to functions are automatically removed upon object death
  void addListener(const Func &func) {
    m_listeners.emplace_back(func);
  }

  template<typename... CallArgs>
  void operator()(CallArgs&&... ca) const {
    for (const Func &fn : m_listeners) {
      fn(std::forward<CallArgs>(ca)...);
    }
  }
};

} /* namespace RadixEntities */

#endif /* RADIXENTITIES_SIGNAL_HPP */

#ifndef RADIXENTITIES_SIGNAL_HPP
#define RADIXENTITIES_SIGNAL_HPP

#include <functional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "iotypes/ValueType.hpp"
#include "util/ParamTypes.hpp"

namespace RadixEntities {

class Entity;

class SignalBase {
protected:
  const std::string m_name;
  Entity *const m_container;
  const std::vector<iotypes::ValueType> m_paramTypes;

  SignalBase(std::string &&name, Entity *container, std::vector<iotypes::ValueType> &&paramTypes);

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

template<typename... Args>
class Signal : public SignalBase {
public:
  using Func = std::function<void(Args...)>;

protected:
  std::vector<Func> m_listeners;

public:
  Signal(std::string &&name, Entity *container) :
    SignalBase(std::move(name), container, getParamTypes<Args...>()) {
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

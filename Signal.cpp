#include "Signal.hpp"

#include "Entity.hpp"

SignalBase::SignalBase(const char *name, Entity *container) :
  m_name(name),
  m_container(container) {
  container->m_signals.emplace(std::piecewise_construct,
      std::forward_as_tuple(name),
      std::forward_as_tuple(this));
}

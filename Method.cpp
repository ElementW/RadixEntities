#include "Method.hpp"

#include "Entity.hpp"

namespace RadixEntities {

MethodBase::MethodBase(const char *name, Entity *container) :
  m_name(name),
  m_container(container) {
  container->m_methods.emplace(std::piecewise_construct,
      std::forward_as_tuple(name),
      std::forward_as_tuple(this));
}

} /* namespace RadixEntities */

#include "Property.hpp"

#include "Entity.hpp"

PropertyBase::PropertyBase(const char *name, Entity *container) :
  m_name(name),
  m_container(container) {
  container->m_properties.emplace(std::piecewise_construct,
      std::forward_as_tuple(name),
      std::forward_as_tuple(this));
}

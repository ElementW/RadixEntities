#include <RadixEntity/Property.hpp>

#include <RadixEntity/Entity.hpp>

namespace radix {
namespace entity {

PropertyBase::PropertyBase(std::string &&name, Entity *container,
    const iotypes::ValueType &valueType, PropertyAccess access) :
  m_name(std::move(name)),
  m_container(container),
  m_valueType(valueType),
  m_access(access) {
  if (container) {
    container->m_properties.emplace(std::piecewise_construct,
        std::forward_as_tuple(name),
        std::forward_as_tuple(this));
  }
}

std::string PropertyBase::str() const {
  return m_valueType.str() + ' ' + m_name;
}

} /* namespace entity */
} /* namespace radix */

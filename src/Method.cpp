#include <RadixEntities/Method.hpp>

#include <RadixEntities/Entity.hpp>

namespace RadixEntities {

MethodBase::MethodBase(std::string &&name, Entity *container,
    std::vector<iotypes::ValueType> &&paramTypes) :
  m_name(std::move(name)),
  m_container(container),
  m_paramTypes(std::move(paramTypes)) {
  container->m_methods.emplace(std::piecewise_construct,
      std::forward_as_tuple(name),
      std::forward_as_tuple(this));
}

std::string MethodBase::str() const {
  std::string ret = m_name + '(';
  bool first = true;
  for (const iotypes::ValueType &vt : m_paramTypes) {
    if (first) {
      first = false;
    } else {
      ret += ", ";
    }
    ret += vt.str();
  }
  ret += ')';
  return ret;
}

} /* namespace RadixEntities */

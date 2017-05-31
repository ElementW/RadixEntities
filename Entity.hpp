#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <unordered_map>

#include "Property.hpp"

class Entity {
protected:
  friend class PropertyBase;

  std::unordered_map<std::string, PropertyBase*> m_properties;
};

#endif /* ENTITY_H */

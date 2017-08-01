#ifndef RADIXENTITIES_UTIL_PARAM_TYPES_HPP
#define RADIXENTITIES_UTIL_PARAM_TYPES_HPP

#include <type_traits>
#include <vector>

#include "../iotypes/ValueType.hpp"

namespace RadixEntities {

namespace detail {

template<typename T>
void getParamTypes(std::vector<iotypes::ValueType> &vec) {
  vec.emplace_back(iotypes::getValueType<T>());
}

template<typename T, typename... Types, typename = std::enable_if_t<(sizeof...(Types) > 0)>>
void getParamTypes(std::vector<iotypes::ValueType> &vec) {
  vec.emplace_back(iotypes::getValueType<T>());
  getParamTypes<Types...>(vec);
}

} /* namespace detail */

template<typename... Types>
std::vector<iotypes::ValueType> getParamTypes() {
  std::vector<iotypes::ValueType> vec;
  detail::getParamTypes<Types...>(vec);
  return vec;
}

} /* namespace RadixEntities */

#endif /* RADIXENTITIES_UTIL_PARAM_TYPES_HPP */

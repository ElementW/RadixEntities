#ifndef RADIXENTITY_IOTYPES_VALUE_HINT_HPP
#define RADIXENTITY_IOTYPES_VALUE_HINT_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace radix {
namespace entity {
namespace iotypes {

struct ValueHint {
};

struct ValueHintInteger : public ValueHint {
  int64_t min, max, offset;
  uint64_t step;
};

struct ValueHintDecimal : public ValueHint {
  double min, max, offset, step;
};

struct ValueHintString : public ValueHint {
  uint32_t maxLength;
  std::string formatUserHint;
  std::vector<std::string> examples;
};

struct ValueHintVector : public ValueHint {
  bool relative, hasMinLength, hasMaxLength;
  double minLength, maxLength;
};

} /* namespace iotypes */
} /* namespace entity */
} /* namespace radix */

#endif /* RADIXENTITY_IOTYPES_VALUE_HINT_HPP */

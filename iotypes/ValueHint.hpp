#ifndef RADIXENTITIES_IOTYPES_VALUE_HINT_HPP
#define RADIXENTITIES_IOTYPES_VALUE_HINT_HPP

namespace RadixEntities {
namespace iotypes {

struct ValueHint {
};

struct ValueHintInteger : public ValueHint {
  int64_t min, max;
  uint64_t step;
};

struct ValueHintDecimal : public ValueHint {
  double min, max, step;
};

struct ValueHintString : public ValueHint {
  uint32_t maxLength;
};

struct ValueHintVector : public ValueHint {
  bool relative, hasMinLength, hasMaxLength;
  double minLength, maxLength;
};

} /* namespace iotypes */
} /* namespace RadixEntities */

#endif /* RADIXENTITIES_IOTYPES_VALUE_HINT_HPP */

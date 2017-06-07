#ifndef ENTITY_IOTYPE_HPP
#define ENTITY_IOTYPE_HPP

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

#endif /* ENTITY_IOTYPE_HPP */

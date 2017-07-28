#ifndef RADIXENTITIES_IOTYPES_VALUE_TYPE_HPP
#define RADIXENTITIES_IOTYPES_VALUE_TYPE_HPP

#include <array>
#include <cstdint>
#include <initializer_list>
#include <string>

namespace RadixEntities {
namespace iotypes {

constexpr uint32_t fourcc(const char *p) {
  uint32_t i = 0;
  for (int i = 0; i < 4; ++i) {
    if (p[i] == 0) {
      break;
    }
    i |= (p[i] << (i * 8));
  }
  return i;
}
/*constexpr uint32_t fourcc(const char p[5]) {
  return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}*/
constexpr uint32_t fourcc(char a, char b, char c, char d) {
  return (a << 24) | (b << 16) | (c << 8) | d;
}
constexpr std::array<char, 4> invfourcc(uint32_t i) {
  const char *p = reinterpret_cast<const char*>(&i);
  return { p[3], p[2], p[1], p[0] };
}

struct ValueType {
  uint32_t namespc;
  uint32_t type;

  ValueType() = default;
  constexpr ValueType(uint32_t ns, uint32_t t) :
    namespc(ns),
    type(t) {
  }
  constexpr ValueType(const char *ns, const char *t) :
    ValueType(fourcc(ns), fourcc(t)) {
  }
  constexpr ValueType(std::initializer_list<const char*> l) :
    ValueType(fourcc(*l.begin()), fourcc(*(l.begin() + 1))) {
  }

  std::string str() const {
    return std::string(invfourcc(namespc).data(), 4) + ":" +
           std::string(invfourcc(type).data(), 4);
  }
};

template<typename T>
constexpr ValueType getValueType();

#define iotype(T, ns, tn)  template<>constexpr ValueType getValueType<T>(){return {ns,tn};}
iotype( uint8_t, "std", "u8")
iotype(  int8_t, "std", "i8")
iotype(uint16_t, "std", "u16")
iotype( int16_t, "std", "i16")
iotype(uint32_t, "std", "u32")
iotype( int32_t, "std", "i32")
iotype(uint64_t, "std", "u16")
iotype( int64_t, "std", "i64")
#undef iotype

} /* namespace iotypes */
} /* namespace RadixEntities */

#endif /* RADIXENTITIES_IOTYPES_VALUE_TYPE_HPP */

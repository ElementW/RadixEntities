#ifndef RADIXENTITY_IOTYPES_VALUE_TYPE_HPP
#define RADIXENTITY_IOTYPES_VALUE_TYPE_HPP

#include <array>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <type_traits>

namespace radix {
namespace entity {
namespace iotypes {

constexpr char u32toc(uint32_t i, int o) {
  return static_cast<char>((i >> o) & 0xFF);
}

constexpr uint32_t fourcc(const char *p) {
  uint32_t r = 0;
  for (int i = 0; i < 4 && p[i] != 0; ++i) {
    r |= p[i] << (i * 8); // ((3-i) * 8);
  }
  return r;
}
constexpr uint32_t fourcc(char a, char b, char c, char d) {
  return (d << 24) | (c << 16) | (b << 8) | a;
  // (a << 24) | (b << 16) | (c << 8) | d;
}
constexpr std::array<char, 4> invfourcc(uint32_t i) {
  return { u32toc(i, 0), u32toc(i, 8), u32toc(i, 16), u32toc(i, 24) };
  // { u32toc(i, 24), u32toc(i, 16), u32toc(i, 8), u32toc(i, 0) };
}
static inline std::string strfourcc(uint32_t i) {
  const char s[] = { u32toc(i, 0), u32toc(i, 8), u32toc(i, 16), u32toc(i, 24), 0 };
  return std::string(s);
}

struct ValueType;
template<typename T>
constexpr ValueType getValueType();

struct ValueType {
  uint32_t namespc;
  uint32_t type;
  enum Flags : uint8_t {
    NoFlags = 0,
    Pointer = 1,
    Reference = 2,
    Const = 4
  } flags;
  uint8_t padding[7];

  constexpr ValueType(const ValueType&) = default;
  constexpr ValueType(ValueType&&) = default;
  constexpr ValueType(uint32_t ns, uint32_t t, Flags flags = NoFlags) :
    namespc(ns),
    type(t),
    flags(flags),
    padding{} {
  }
  constexpr ValueType(uint32_t ns, uint32_t t, int flags) :
    ValueType(ns, t, static_cast<Flags>(flags)) {
  }
  constexpr ValueType(const char *ns, const char *t, Flags flags = NoFlags) :
    ValueType(fourcc(ns), fourcc(t), flags) {
  }
  constexpr ValueType(const char *ns, const char *t, int flags) :
    ValueType(fourcc(ns), fourcc(t), static_cast<Flags>(flags)) {
  }
  constexpr ValueType(std::initializer_list<const char*> l) :
    ValueType(fourcc(*l.begin()), fourcc(*(l.begin() + 1))) {
  }
  constexpr ValueType(const ValueType &vt, Flags pflags) :
    namespc(vt.namespc),
    type(vt.type),
    flags(static_cast<Flags>(vt.flags | pflags)),
    padding{} {
  }

  template<typename T>
  constexpr ValueType() :
    ValueType(getValueType<T>()) {
  }

  constexpr ValueType base() const {
    return ValueType(namespc, type);
  }

  constexpr bool operator==(const ValueType &o) const {
    return namespc == o.namespc && type == o.type && flags == o.flags;
  }

  std::string str() const;
};
static_assert(sizeof(ValueType) == 16,
              "ValueType isn't 16 bytes long, that disables optimized vectorized value copies");
static_assert(std::is_compound<ValueType>::value, "ValueType isn't a compound");

template<typename T>
constexpr ValueType getBaseValueType();


template<typename T>
struct ValueTypeResolver {
  static constexpr ValueType value = getBaseValueType<T>();
};

template<typename T>
constexpr ValueType getValueType() {
  return ValueTypeResolver<T>::value;
}

template<typename T>
struct ValueTypeResolver<T*> {
  static constexpr ValueType value = ValueType(getValueType<T>(), ValueType::Pointer);
};

template<typename T>
struct ValueTypeResolver<T&> {
  static constexpr ValueType value = ValueType(getValueType<T>(), ValueType::Reference);
};

template<typename T>
struct ValueTypeResolver<const T> {
  static constexpr ValueType value = ValueType(getValueType<T>(), ValueType::Const);
};


#define iotype(T, ns, tn) template<>constexpr ValueType getBaseValueType<T>(){return {ns,tn};}
iotype(bool, "std", "bool")
iotype( uint8_t, "std", "u8")
iotype(  int8_t, "std", "i8")
iotype(uint16_t, "std", "u16")
iotype( int16_t, "std", "i16")
iotype(uint32_t, "std", "u32")
iotype( int32_t, "std", "i32")
iotype(uint64_t, "std", "u16")
iotype( int64_t, "std", "i64")
iotype(float, "std", "f32")
iotype(double, "std", "f64")
iotype(std::string, "std", "st8")
iotype(std::u32string, "std", "st32")
#undef iotype

} /* namespace iotypes */
} /* namespace entity */
} /* namespace radix */

#endif /* RADIXENTITY_IOTYPES_VALUE_TYPE_HPP */

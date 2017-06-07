#ifndef ENTITY_IOTYPE_HPP
#define ENTITY_IOTYPE_HPP

namespace iotypes {

constexpr uint32_t fourcc(char const p[5]) {
  return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}
constexpr uint32_t fourcc(char a, char b, char c, char d) {
  return (a << 24) | (b << 16) | (c << 8) | d;
}

struct ValueType {
  uint32_t namespc;
  uint32_t type;
};

} /* namespace iotypes */

#endif /* ENTITY_IOTYPE_HPP */

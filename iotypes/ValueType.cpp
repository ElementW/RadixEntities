#include "ValueType.hpp"

namespace RadixEntities {
namespace iotypes {

std::string ValueType::str() const {
  std::string res;
  if (flags & Const) {
    res += "const ";
  }
  res += strfourcc(namespc) + ":" + strfourcc(type);
  if (flags & Reference) {
    res += '&';
  }
  if (flags & Pointer) {
    res += '*';
  }
  return res;
}

} /* namespace iotypes */
} /* namespace RadixEntities */

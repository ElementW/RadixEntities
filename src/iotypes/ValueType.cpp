#include <RadixEntity/iotypes/ValueType.hpp>

namespace radix {
namespace entity {
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
} /* namespace entity */
} /* namespace radix */

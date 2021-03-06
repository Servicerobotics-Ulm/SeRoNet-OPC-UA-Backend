/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 *  \author Christian von Arnim  - ISW, University of Stuttgart
 *  \author Sebastian Friedl - ISW, University of Stuttgart
 **/

#ifndef SERONETSDK_ENUMPARAMRESPONSETYPE_H
#define SERONETSDK_ENUMPARAMRESPONSETYPE_H

#include "EnumParamResponseTypeBase.hpp"
#include <string>
#include <ostream>

namespace SeRoNet {
namespace DefaultCommObjects {

class ParamResponseType {
 public:
  enum literal {

    BUSY = LITERAL_BUSY,
    INVALID = LITERAL_INVALID,
    OK = LITERAL_OK
  };

  ParamResponseType()
      : value(INVALID) {
  }

  explicit ParamResponseType(literal t) {
    value = t;
  }

  explicit operator literal() const {
    return value;
  }

  explicit ParamResponseType(ENUM_ParamResponseType t) {
    value = (literal) t;
  }

  bool operator==(ParamResponseType &t) {
    return this->value == t.value;
  }

  ENUM_ParamResponseType get_value_idl() const {
    return (ENUM_ParamResponseType) value;
  }

  literal get_value() const {
    return value;
  }

  std::string get_string() const {
    switch (value) {

      case BUSY:return "ParamResponseType::BUSY";
      case INVALID:return "ParamResponseType::INVALID";
      case OK:return "ParamResponseType::OK";
      default:return "UNKNOWN!";
    }
  }

 private:
  literal value;
};

inline std::ostream &operator<<(std::ostream &os, const ParamResponseType &e) {
  os << e.get_string();
  return os;
}

} // end namespace DefaultCommObjects
} // end namespace SeRoNet

#endif //SERONETSDK_ENUMPARAMRESPONSETYPE_H

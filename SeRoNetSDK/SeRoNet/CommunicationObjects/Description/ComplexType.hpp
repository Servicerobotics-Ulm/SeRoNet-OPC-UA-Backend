/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 *  \author Christian von Arnim  - ISW, University of Stuttgart
 *  \author Sebastian Friedl - ISW, University of Stuttgart
 **/


#pragma once

#include "../../../Definitions.hpp"
#include "IDescription.hpp"
#include <list>

namespace SeRoNet {
namespace CommunicationObjects {
namespace Description {
class SERONETSDK_EXPORT ComplexType : public IDescription {
 public:
  typedef std::list<IDescription::shp_t> ListOfElements_t;
  typedef std::shared_ptr<ComplexType> shp_t;

  using IDescription::IDescription;
  ListOfElements_t::const_iterator begin() { return m_elements.begin(); }
  ListOfElements_t::const_iterator end() { return m_elements.end(); }
  //Return type allows chain of add (e.g. X.add(...).add(...))
  ComplexType &add(IDescription::shp_t element);

  void accept(IVisitorDescription *visitor) override;

 private:
  ListOfElements_t m_elements;
};
}
}
}

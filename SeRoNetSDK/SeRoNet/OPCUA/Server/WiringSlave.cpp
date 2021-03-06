/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 *  \author Christian von Arnim  - ISW, University of Stuttgart
 *  \author Sebastian Friedl - ISW, University of Stuttgart
 **/


#include "WiringSlave.hpp"
#include "../../DefaultCommObjects/Description/WiringCommObjectDescription.hpp"

namespace SeRoNet {
namespace OPCUA {
namespace Server {

WiringQueryHandler::WiringQueryHandler(WiringSlave *slave) noexcept
     : wiringSlave(slave) {}

void WiringQueryHandler::handleQuery(IQueryServer &server, const Smart::QueryIdPtr &id,
                                     const DefaultCommObjects::WiringCommObject &request) {
  DefaultCommObjects::WiringCommObject answer;

  answer = wiringSlave->handleWiring(request);

  server.answer(id, answer);
}

WiringSlave::WiringSlave(Utils::Component *component, std::string slaveaddress)
    : component(component) {
  {

    const std::string SMART_NONS = "NoNamingService"; //FIXME check why this const is needed

    handler = std::make_shared<WiringQueryHandler>(this);

    threadHandler = std::make_shared<SeRoNet::Utils::HsUlm::ThreadQueueQueryHandler<DefaultCommObjects::WiringCommObject,
                                                           DefaultCommObjects::WiringCommObject>>(component,handler);

    // added functionality to work without NamingService
    if (component->getName() == SMART_NONS) {
      if (slaveaddress.empty()) {
        wiring = nullptr;
        std::cout
            << "<WiringSlave> Warning: wiring slave is not initialized properly. "
            << "Please use second parameter of WiringSlave-constuctor!"
            << std::endl;
      } else {
        // Slaveaddress has to contain a propper "ip:portnr" string for WiringSlave.
        // This "ip:portnr" will be used by WiringMaster to connect to WiringSlave!
        wiring = new QueryServer<DefaultCommObjects::WiringCommObject,
                                 DefaultCommObjects::WiringCommObject>(component, slaveaddress, threadHandler);
      }
    } else {
      wiring = new QueryServer<DefaultCommObjects::WiringCommObject,
                               DefaultCommObjects::WiringCommObject>(component, "wiring", threadHandler);
    }
  }
}

DefaultCommObjects::WiringCommObject WiringSlave::handleWiring(const DefaultCommObjects::WiringCommObject &request) {
  DefaultCommObjects::WiringCommObject answer;
  Smart::StatusCode status;
  std::string command, portName, servername, servicename;

  request.getCommand(command, portName, servername, servicename);

  std::unique_lock<decltype(mutex)> lock(mutex);

  if (command == "connect") {
    //
    // look for the portName
    //
    auto it = ports.find(portName);

    if (it != ports.end()) {
      //
      // portName known, now call the connect method of the client object
      //
      status = it->second.connectFunction(servername, servicename);

      answer.setStatus(status);
    } else {
      answer.setStatus(Smart::SMART_UNKNOWNPORT);
    }
  } else if (command == "disconnect") {
    //
    // look for the portName
    //
    auto it = ports.find(portName);

    if (it != ports.end()) {
      //
      //
      // portName known, now call the disconnect method of the client object
      //
      status = it->second.disconnectFunction();

      answer.setStatus(status);
    } else {
      answer.setStatus(Smart::SMART_UNKNOWNPORT);
    }
  } else {
    answer.setStatus(Smart::SMART_ERROR);
  }

  return answer;
}

Smart::StatusCode WiringSlave::add(const std::string &portName,
                                   std::function<Smart::StatusCode(const std::string &,
                                                                   const std::string &)> connectFunction,
                                   std::function<Smart::StatusCode()> disconnectFunction) {
  if (wiring == nullptr) {
    std::cout
        << "<WiringSlave> Warning: wiring slave is not initialized properly. "
        << "Please use second parameter of WiringSlave-constuctor!"
        << std::endl;
  }

  std::unique_lock<decltype(mutex)> lock(mutex);
  PortElement tmp{portName, connectFunction, disconnectFunction};
  auto ret = ports.insert(std::pair<std::string, PortElement>(portName, tmp));
  return ret.second ? Smart::SMART_OK : Smart::SMART_PORTALREADYUSED;
}

Smart::StatusCode WiringSlave::remove(const std::string &portName) {
  std::unique_lock<decltype(mutex)> lock(mutex);
  auto ret = ports.erase(portName);
  return ret == 1 ? Smart::SMART_OK : Smart::SMART_PORTALREADYUSED; // remove was successful if one element was removed
}

WiringSlave::~WiringSlave() {
  std::unique_lock<decltype(mutex)> lock(mutex);
}

bool WiringSlave::PortElement::operator==(const WiringSlave::PortElement &rhs) const {
  return portName == rhs.portName;
}

bool WiringSlave::PortElement::operator!=(const WiringSlave::PortElement &rhs) const {
  return !(rhs == *this);
}

} // end namespace Server
} // end namespace OPCUA
} // end namespace SeRoNet

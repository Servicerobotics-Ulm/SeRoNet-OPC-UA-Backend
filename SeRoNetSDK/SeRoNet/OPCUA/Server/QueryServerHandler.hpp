/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 *  \author Christian von Arnim  - ISW, University of Stuttgart
 *  \author Sebastian Friedl - ISW, University of Stuttgart
 **/

#ifndef QUERY_SERVER_HANDLER_HPP
#define QUERY_SERVER_HANDLER_HPP

////#include "SeRoNet/Utils/IObserver.hpp"
#include "QueryServer.hpp"
#include <smartIQueryServerPattern_T.h>
#include <smartICommunicationObject.h>

namespace SeRoNet {
namespace OPCUA {
namespace Server {

template<typename T_REQUEST, typename T_ANSWER>
class QueryServer;

template<typename T_REQUEST, typename T_ANSWER>
class QueryServerHandler : public Smart::IQueryServerHandler<T_REQUEST, T_ANSWER, int> {
 protected:
  SeRoNet::OPCUA::Server::QueryServer<T_REQUEST, T_ANSWER> *m_server; //TODO check can be deleted

 public:
  QueryServerHandler() = default;

  QueryServerHandler(SeRoNet::OPCUA::Server::QueryServer<T_REQUEST, T_ANSWER> *server)
      : m_server(server),
        Smart::IQueryServerHandler<T_REQUEST, T_ANSWER, int>(server) {}

  ~QueryServerHandler() = default;

  /*
  virtual void handle_input(int id, Smart::ICommunicationObject *input) {
    auto request = static_cast<T_REQUEST *> (input);
    this->handleQuery(id, *request);

  }*/

};

}
}
}

#endif
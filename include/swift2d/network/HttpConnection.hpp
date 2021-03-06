////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_HTTP_CONNECTION_HPP
#define SWIFT2D_HTTP_CONNECTION_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/events.hpp>

namespace RakNet {
  class TCPInterface;
  class HTTPConnection2;
}

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL HttpConnection{

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  Signal<std::string> on_response;

  HttpConnection();
  ~HttpConnection();

  void get(std::string const& uri, std::string const& server, unsigned short port);
  void post(std::string const& uri, std::string const& content, std::string const& server, unsigned short port);
  void del(std::string const& uri, std::string const& server, unsigned short port);

  void update();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  RakNet::TCPInterface*    tcp_;
  RakNet::HTTPConnection2* http_;
};

}

#endif  // SWIFT2D_HTTP_CONNECTION_HPP

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_EVENTS_MAINLOOP_HPP
#define SWIFT2D_EVENTS_MAINLOOP_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/Singleton.hpp>

#include <functional>

namespace boost {
  namespace asio {
    class io_service;
  }
  namespace system {
    class error_code;
  }
}

namespace swift {

// -----------------------------------------------------------------------------
class SWIFT_DLL MainLoop : public Singleton<MainLoop> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  void start();
  void stop();

  boost::asio::io_service& get_io_service();

  friend class Singleton<MainLoop>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  MainLoop() {};
  ~MainLoop() {};
};

// -----------------------------------------------------------------------------

}

#endif /* SWIFT2D_EVENTS_MAINLOOP_HPP */

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

#ifndef SWIFT2D_EVENTS_SCHEDULER_HPP
#define SWIFT2D_EVENTS_SCHEDULER_HPP

// includes  ------------------------------------------------------------------
#include <swift2d/events/MainLoop.hpp>
#include <mutex>
#include <map>

#include <boost/asio/deadline_timer.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// A class which can be used to call a method after a given interval.         //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL Scheduler {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  Scheduler();

  // copy constructor creates new Scheduler
  Scheduler(Scheduler const& to_copy);
  ~Scheduler();

  // ------------------------------------------------------------ public methods
  int execute_delayed(double delay, std::function<void()> callback);
  void cancel(int id);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
   void self_callback(boost::asio::deadline_timer* timer, int id);

   std::map<int, std::pair<boost::asio::deadline_timer*, std::function<void()>>> tasks_;
   int current_id_;
   std::mutex mutex_;
};

// -----------------------------------------------------------------------------

}

#endif /* SWIFT2D_EVENTS_SCHEDULER_HPP */

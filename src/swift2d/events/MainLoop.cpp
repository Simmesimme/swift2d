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

#include <swift2d/events/MainLoop.hpp>
#include <iostream>

// boost asio implmentation
#include <boost/asio.hpp>
#include <boost/asio/impl/src.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

MainLoop::MainLoop() 
  : io_service_(new boost::asio::io_service()) {}

////////////////////////////////////////////////////////////////////////////////

MainLoop::~MainLoop() {
  delete io_service_;
}

////////////////////////////////////////////////////////////////////////////////

void MainLoop::start() {
  io_service_->run();
}

////////////////////////////////////////////////////////////////////////////////

void MainLoop::stop() {
  io_service_->stop();
}

////////////////////////////////////////////////////////////////////////////////

boost::asio::io_service& MainLoop::get_io_service() { 
  return *io_service_; 
}

////////////////////////////////////////////////////////////////////////////////

}

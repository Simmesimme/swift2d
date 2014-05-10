////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_RENDERCLIENT_HPP
#define SWIFT2D_RENDERCLIENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/DoubleBuffer.hpp>
#include <swift2d/utils/FPSCounter.hpp>

// #include <thread>
#include <boost/thread.hpp>
#include <functional>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// The queue_draw method is directly called by the Renderer. Internally it    //
// uses a threaded rendering loop which always waits for queue_draw calls.    //
// When it fails to finish rendering before the next queue_draw is called, it //
// will ignore this call.                                                     //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
template <typename T>
class RenderClient {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  template <typename F>
  RenderClient(F&& fun) : fps_counter(20), forever_(), double_buffer_() {


    forever_ = boost::thread([this, fun]() {
      this->fps_counter.start();

      while (true) {
        auto sg = this->double_buffer_.read();
        fun(sg);
        this->fps_counter.step();
      }
    });
  }

  ~RenderClient() { forever_.detach(); }

  FPSCounter fps_counter;

  // This requests a drawing operation of the given scene. If the client
  // is still processing the last call of this function it will be
  // ignored.
  inline void queue_draw(T const& scene_graphs) {
    double_buffer_.write_blocked(scene_graphs);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // -------------------------------------------------------- protected interface
 private:
  boost::thread    forever_;
  DoubleBuffer<T>  double_buffer_;
};

}

#endif  // SWIFT2D_RENDERCLIENT_HPP
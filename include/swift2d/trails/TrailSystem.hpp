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

#ifndef SWIFT2D_TRAIL_SYSTEM_HPP
#define SWIFT2D_TRAIL_SYSTEM_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/RenderContext.hpp>
#include <swift2d/trails/TrailEmitterComponent.hpp>
#include <swift2d/trails/TrailSystemComponent.hpp>
#include <swift2d/events/Timer.hpp>

#include <unordered_map>
#include <unordered_set>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class TrailSystem;
typedef std::shared_ptr<TrailSystem>       TrailSystemPtr;
typedef std::shared_ptr<const TrailSystem> ConstTrailSystemPtr;
typedef Property<TrailSystemPtr>           TrailSystemProperty;

// -----------------------------------------------------------------------------
class TrailSystem {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  TrailSystem(int max_trail_points);

  // Creates a new component and returns a shared pointer.
  static TrailSystemPtr create(int max_trail_points) {
    return std::make_shared<TrailSystem>(max_trail_points);
  }

  // ------------------------------------------------------------ public methods
  void set_max_trail_points(int max_trail_points);

  void spawn(
    std::vector<TrailSegment> const& end_segments,
    std::vector<TrailSegment> const& new_segments);

  void update_trails(
    TrailSystemComponent::Serialized const& system,
    RenderContext const& context);

  void draw_trails(
    TrailSystemComponent::Serialized const& system,
    RenderContext const& context);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void upload_to(RenderContext const& context);

  inline int current_tf() const { return ping_ ? 1 : 0; }
  inline int current_vb() const { return ping_ ? 0 : 1; }

  std::vector<oglplus::TransformFeedback> transform_feedbacks_;
  std::vector<oglplus::Buffer>            trail_buffers_;
  std::vector<oglplus::VertexArray>       trail_vaos_;

  oglplus::Buffer*                        emitter_buffer_;
  oglplus::VertexArray*                   emitter_vao_;

  bool   ping_;
  int    update_max_trail_points_;

  std::vector<TrailSegment> new_segments_;
  std::vector<TrailSegment> end_segments_ping_;
  std::vector<TrailSegment> end_segments_pong_;
  mutable std::mutex mutex_;
};

}

#endif // SWIFT2D_TRAIL_SYSTEM_HPP

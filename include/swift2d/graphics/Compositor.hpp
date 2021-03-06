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

#ifndef SWIFT2D_COMPOSITOR_HPP
#define SWIFT2D_COMPOSITOR_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/PostProcessor.hpp>
#include <swift2d/graphics/GBuffer.hpp>
#include <swift2d/graphics/LBuffer.hpp>
#include <swift2d/materials/Shader.hpp>
#include <swift2d/properties.hpp>

namespace swift {

// -----------------------------------------------------------------------------
class Compositor {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------- construction interface
  Compositor(RenderContext& ctx);
  ~Compositor();

  // ------------------------------------------------------------ public methods
  void draw_objects (ConstSerializedScenePtr const& scene, RenderContext const& ctx);
  void draw_lights  (ConstSerializedScenePtr const& scene, RenderContext const& ctx);
  void post_process (ConstSerializedScenePtr const& scene, RenderContext const& ctx);
  void draw_gui     (ConstSerializedScenePtr const& scene, RenderContext const& ctx);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  GBuffer*       g_buffer_;
  LBuffer*       l_buffer_;
  PostProcessor* post_processor_;
};

}

#endif // SWIFT2D_COMPOSITOR_HPP

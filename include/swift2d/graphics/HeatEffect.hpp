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

#ifndef SWIFT2D_HEAT_EFFECT_HPP
#define SWIFT2D_HEAT_EFFECT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/scene/SerializedScene.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class HeatEffect {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  HeatEffect(RenderContext const& ctx);

  void process(ConstSerializedScenePtr const& scene, RenderContext const& ctx);
  int bind_buffers(int start, RenderContext const& ctx);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  oglplus::Framebuffer heat_fbo_;
  oglplus::Texture     heat_buffer_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_HEAT_EFFECT_HPP

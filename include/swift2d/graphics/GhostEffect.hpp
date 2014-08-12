////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_GHOST_EFFECT_HPP
#define SWIFT2D_GHOST_EFFECT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/scene/SerializedScene.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class GhostEffect {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  GhostEffect(RenderContext const& ctx);

  void process(RenderContext const& ctx, oglplus::Texture const& threshold_buffer_);
  int bind_buffers(int start, RenderContext const& ctx);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  Shader               blur_shader_;
  Shader               ghost_shader_;

  oglplus::Lazy<oglplus::Uniform<math::vec2>> step_;
  oglplus::Lazy<oglplus::Uniform<int>>        input_tex_;
  oglplus::Lazy<oglplus::Uniform<math::vec4>> scalar_;
  oglplus::Lazy<oglplus::Uniform<int>>        input_tex_1_;
  oglplus::Lazy<oglplus::Uniform<int>>        input_tex_2_;
  oglplus::Lazy<oglplus::Uniform<int>>        input_tex_3_;
  oglplus::Lazy<oglplus::Uniform<math::vec3>> color1_;
  oglplus::Lazy<oglplus::Uniform<math::vec3>> color2_;
  oglplus::Lazy<oglplus::Uniform<math::vec3>> color3_;
  oglplus::Lazy<oglplus::Uniform<math::vec3>> color4_;

  oglplus::Framebuffer fbo_;
  oglplus::Texture     buffer_tmp_;
  oglplus::Texture     blur_buffer_;
  oglplus::Texture     ghost_buffer_1_;
  oglplus::Texture     ghost_buffer_2_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_GHOST_EFFECT_HPP

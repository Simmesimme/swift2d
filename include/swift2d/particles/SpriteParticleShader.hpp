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

#ifndef SWIFT2D_SPRITE_PARTICLE_SHADER_HPP
#define SWIFT2D_SPRITE_PARTICLE_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SpriteParticleShader : public Shader,
                             public Singleton<SpriteParticleShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------ uniforms
  oglplus::Lazy<oglplus::Uniform<math::mat3>>  projection;
  oglplus::Lazy<oglplus::Uniform<int>>         diffuse;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  start_mid_scale;
  oglplus::Lazy<oglplus::Uniform<math::vec2>>  end_scale;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  start_color;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  mid_color;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  end_color;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  glow_mid_life;
  oglplus::Lazy<oglplus::Uniform<math::vec3>>  burn;

  friend class Singleton<SpriteParticleShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  SpriteParticleShader();
  ~SpriteParticleShader() {};
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_SPRITE_PARTICLE_SHADER_HPP

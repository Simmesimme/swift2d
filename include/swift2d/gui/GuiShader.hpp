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

#ifndef SWIFT2D_GUI_SHADER_HPP
#define SWIFT2D_GUI_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class GuiShader : public Shader, public Singleton<GuiShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------ uniforms
  oglplus::Lazy<oglplus::Uniform<float>>       opacity;
  oglplus::Lazy<oglplus::Uniform<math::vec2>>  size;
  oglplus::Lazy<oglplus::Uniform<math::vec3>>  offset_rot;
  oglplus::Lazy<oglplus::Uniform<int>>         diffuse;

  friend class Singleton<GuiShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  GuiShader();
  ~GuiShader() {};
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_GUI_SHADER_HPP

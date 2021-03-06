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

#ifndef SWIFT2D_SCENE_HPP
#define SWIFT2D_SCENE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/SceneObject.hpp>
#include <swift2d/components/CameraComponent.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <swift2d/utils/Color.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Scene;
typedef std::shared_ptr<Scene>       ScenePtr;
typedef std::shared_ptr<const Scene> ConstScenePtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL Scene {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  SceneObjectPtr     Root;
  CameraComponentPtr Camera;

  ColorProperty      VignetteColor;
  Float              VignetteSoftness;
  Float              VignetteCoverage;

  Float              DirtOpacity;
  Float              GlowOpacity;

  String             ColorMapName;
  AnimatedFloat      ColorGradingIntensity;

  // ----------------------------------------------------- contruction interface
  static ScenePtr create() {
    return std::make_shared<Scene>();
  }

  Scene();

  void update(double time);

  // calls serialize() on all enabled components and objects --- a new
  // SerializedScene is created
  virtual SerializedScenePtr serialize() const;
};

}

#endif  // SWIFT2D_SCENE_HPP

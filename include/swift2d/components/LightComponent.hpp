////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_LIGHT_COMPONENT_HPP
#define SWIFT2D_LIGHT_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/DrawableComponent.hpp>
#include <swift2d/geometries/Quad.hpp>
#include <swift2d/textures/Texture.hpp>

#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class LightComponent;
typedef std::shared_ptr<LightComponent>       LightComponentPtr;
typedef std::shared_ptr<const LightComponent> ConstLightComponentPtr;

// -----------------------------------------------------------------------------
class LightComponent : public DrawableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float            Depth;
  MaterialProperty Material;

  // ----------------------------------------------------- contruction interface

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static LightComponentPtr create(Args&& ... a) {
    return std::make_shared<LightComponent>(a...);
  }

  // creates a copy from this
  LightComponentPtr create_copy() const {
    return std::make_shared<LightComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "LightComponent"; }

  void draw(RenderContext const& ctx) {
    Material()->use(ctx, WorldTransform());
    Quad::instance()->draw(ctx);
  }

  void serialize(SerializedScenePtr& scene) const {
    scene->lights.insert(std::make_pair(Depth.get(), create_copy()));
  }

  virtual void save(SceneSaver& saver) {
    DrawableComponent::save(saver);
    saver.save("Depth", &Depth);
  }
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_LIGHT_COMPONENT_HPP

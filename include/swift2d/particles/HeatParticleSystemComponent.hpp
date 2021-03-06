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

#ifndef SWIFT2D_HEAT_PARTICLE_SYSTEM_COMPONENT_HPP
#define SWIFT2D_HEAT_PARTICLE_SYSTEM_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleSystemComponent.hpp>
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/textures/Texture.hpp>
#include <swift2d/utils/Color.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class HeatParticleSystemComponent;
typedef std::shared_ptr<HeatParticleSystemComponent>       HeatParticleSystemComponentPtr;
typedef std::shared_ptr<const HeatParticleSystemComponent> ConstHeatParticleSystemComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL HeatParticleSystemComponent: public ParticleSystemComponent{

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------- inner classes
  struct Serialized : public ParticleSystemComponent::Serialized {
    float MidLife;
    float StartScale,   MidScale,   EndScale;
    float StartOpacity, MidOpacity, EndOpacity;
    TexturePtr Texture;
  };

  class Renderer : public ResourceRenderer<HeatParticleSystemComponent> {
    void predraw(RenderContext const& ctx);
    void draw(RenderContext const& ctx, int start, int end);
  };

  // ---------------------------------------------------------------- properties
  Float           MidLife;
  Float           StartScale,   MidScale,   EndScale;
  Float           StartOpacity, MidOpacity, EndOpacity;
  TextureProperty Texture;

  // ----------------------------------------------------- contruction interface
  HeatParticleSystemComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static HeatParticleSystemComponentPtr create(Args&& ... a) {
    return std::make_shared<HeatParticleSystemComponent>(a...);
  }

  // creates a copy from this
  HeatParticleSystemComponentPtr create_copy() const {
    return std::make_shared<HeatParticleSystemComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "HeatParticleSystemComponent"; }

  void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_HEAT_PARTICLE_SYSTEM_COMPONENT_HPP

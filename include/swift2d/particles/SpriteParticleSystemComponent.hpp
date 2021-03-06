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

#ifndef SWIFT2D_SPRITE_PARTICLE_SYSTEM_COMPONENT_HPP
#define SWIFT2D_SPRITE_PARTICLE_SYSTEM_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/particles/ParticleSystemComponent.hpp>
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/textures/Texture.hpp>
#include <swift2d/utils/Color.hpp>
#include <swift2d/graphics/SubSampler.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class SpriteParticleSystemComponent;
typedef std::shared_ptr<SpriteParticleSystemComponent>       SpriteParticleSystemComponentPtr;
typedef std::shared_ptr<const SpriteParticleSystemComponent> ConstSpriteParticleSystemComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL SpriteParticleSystemComponent : public ParticleSystemComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------- inner classes
  struct Serialized : public ParticleSystemComponent::Serialized {
    float      MidLife;
    math::vec2 StartScale, MidScale, EndScale;
    float      StartGlow,  MidGlow,  EndGlow;
    float      StartBurn,  MidBurn,  EndBurn;
    math::vec4 StartColor, MidColor, EndColor;
    int        SubSamplingLevel;
    TexturePtr Texture;
  };

  class Renderer : public ResourceRenderer<SpriteParticleSystemComponent> {
    void predraw(RenderContext const& ctx);
    void draw(RenderContext const& ctx, int start, int end);
  };

  // ---------------------------------------------------------------- properties
  Float           MidLife;
  Vec2            StartScale, MidScale, EndScale;
  Float           StartGlow,  MidGlow,  EndGlow;
  Float           StartBurn,  MidBurn,  EndBurn;
  ColorProperty   StartColor, MidColor, EndColor;
  Int32           SubSamplingLevel;
  TextureProperty Texture;

  // ----------------------------------------------------- contruction interface
  SpriteParticleSystemComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static SpriteParticleSystemComponentPtr create(Args&& ... a) {
    return std::make_shared<SpriteParticleSystemComponent>(a...);
  }

  // creates a copy from this
  SpriteParticleSystemComponentPtr create_copy() const {
    return std::make_shared<SpriteParticleSystemComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "SpriteParticleSystemComponent"; }

  void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_SPRITE_PARTICLE_SYSTEM_COMPONENT_HPP

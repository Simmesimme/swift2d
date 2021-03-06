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

#ifndef SWIFT2D_FULLSCREEN_GUI_SPRITE_COMPONENT_HPP
#define SWIFT2D_FULLSCREEN_GUI_SPRITE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/components/DepthComponent.hpp>
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/textures/Texture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class FullscreenGuiSpriteComponent;
typedef std::shared_ptr<FullscreenGuiSpriteComponent>       FullscreenGuiSpriteComponentPtr;
typedef std::shared_ptr<const FullscreenGuiSpriteComponent> ConstFullscreenGuiSpriteComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL FullscreenGuiSpriteComponent : public Component,
                                               public DepthComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------- inner classes
  struct Serialized : public SerializedComponent {
    float           Opacity;
    TexturePtr      Texture;
    math::vec2i     Size;
    math::vec2      Anchor;
    math::vec2      Offset;
  };

  class Renderer : public ResourceRenderer<FullscreenGuiSpriteComponent> {
    void draw(RenderContext const& ctx, int start, int end);
  };

  // ---------------------------------------------------------------- properties
  Float           Opacity;
  TextureProperty Texture;

  // ---------------------------------------------------- construction interface
  FullscreenGuiSpriteComponent();

  static FullscreenGuiSpriteComponentPtr create() {
    return std::make_shared<FullscreenGuiSpriteComponent>();
  }

  // creates a copy from this
  FullscreenGuiSpriteComponentPtr create_copy() const {
    return std::make_shared<FullscreenGuiSpriteComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "FullscreenGuiSpriteComponent"; }

  virtual void update(double time);
  void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_FULLSCREEN_GUI_SPRITE_COMPONENT_HPP

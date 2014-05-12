////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SPRITE_COMPONENT_HPP
#define SWIFT2D_SPRITE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/DrawableComponent.hpp>

#include <swift2d/resources/SpriteResource.hpp>
#include <swift2d/resources/TextureResource.hpp>

#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class SpriteComponent;
typedef std::shared_ptr<SpriteComponent>       SpriteComponentPtr;
typedef std::shared_ptr<const SpriteComponent> ConstSpriteComponentPtr;

// -----------------------------------------------------------------------------
class SpriteComponent : public DrawableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float pDepth;

  // ----------------------------------------------------- contruction interface
  SpriteComponent() : sprite_(nullptr), diffuse_(nullptr), normal_(nullptr) {}

  // Creates a new component and returns a shared pointer.
  static SpriteComponentPtr create() {
    return std::make_shared<SpriteComponent>();
  }

  SpriteComponentPtr create_copy() const {
    return std::make_shared<SpriteComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  void draw(RenderContext const& ctx) {
    diffuse_->bind(ctx, 0);
    if (normal_) normal_->bind(ctx, 1);

    sprite_->draw(ctx, pWorldTransform.get(), normal_ != nullptr);
  }

  void serialize(SerializedScenePtr& scene) const {
    scene->objects.insert(std::make_pair(pDepth.get(), create_copy()));
  }

  SpriteResource* sprite_;
  TextureResource* diffuse_;
  TextureResource* normal_;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

};

}

#endif  // SWIFT2D_SPRITE_COMPONENT_HPP

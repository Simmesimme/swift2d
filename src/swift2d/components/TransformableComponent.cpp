////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/components/TransformableComponent.hpp>

#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void TransformableComponent::update(double time) {
  if (get_user()) {
    WorldTransform = get_user()->WorldTransform.get() * Transform.get();
  } else {
    WorldTransform = Transform.get();
  }
}

////////////////////////////////////////////////////////////////////////////////

math::vec2 TransformableComponent::get_position() const {
  return math::get_position(Transform.get());
}

////////////////////////////////////////////////////////////////////////////////

math::vec2 TransformableComponent::get_world_position() const {
  return math::get_position(WorldTransform.get());
}

////////////////////////////////////////////////////////////////////////////////

}

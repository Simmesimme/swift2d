////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/SceneObject.hpp>

#include <iostream>
#include <algorithm>

namespace swift {


////////////////////////////////////////////////////////////////////////////////

void SceneObject::remove_component(ComponentPtr const& component) {
  auto delete_pos(std::remove(components_.begin(), components_.end(), component));

  if (delete_pos != components_.end()) {
    (*delete_pos)->pUser = nullptr;
    components_.erase(delete_pos, components_.end());
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::serialize(SerializedScenePtr& scene) const {
  for (auto const& component: components_) {
    if (component->pEnabled.get()) {
      component->serialize(scene);
    }
  }

  for (auto const& child: children_) {
    child->serialize(scene);
  }
}

////////////////////////////////////////////////////////////////////////////////

void SceneObject::update() {
  for (auto const& component: components_) {
    if (component->pEnabled.get()) {
      component->update();
    }
  }

  for (auto const& child: children_) {
    child->update();
  }
}

////////////////////////////////////////////////////////////////////////////////

}

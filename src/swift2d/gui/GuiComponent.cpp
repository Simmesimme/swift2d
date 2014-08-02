////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/gui/GuiComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

GuiComponent::GuiComponent()
  : Depth(0.f)
  , Resource()
  , Size(math::vec2i(10,10))
  , Anchor(math::vec2i(0,0))
  , gui_element_(GuiElement::create(this))
  , Interactive(true) {

  Interactive.on_change().connect([&](bool val){
    gui_element_->set_interactive(val);
  });
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::reload() {
  gui_element_->reload();
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::call_javascript(std::string const& method, std::string const& arg) {
  gui_element_->call_javascript(method, {arg});
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::call_javascript(std::string const& method, std::vector<std::string> const& args) {
  gui_element_->call_javascript(method, args);
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::add_javascript_callback(std::string const& callback) {
  gui_element_->add_javascript_callback(callback);
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::draw(RenderContext const& ctx) {
  gui_element_->draw(ctx);
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::serialize(SerializedScenePtr& scene) const {
  scene->gui_elements.insert(std::make_pair(Depth.get(), create_copy()));
}

////////////////////////////////////////////////////////////////////////////////

void GuiComponent::accept(SavableObjectVisitor& visitor) {
  DrawableComponent::accept(visitor);
  visitor.add_member("Depth", Depth);
  visitor.add_member("Resource", Resource);
  visitor.add_member("Size", Size);
  visitor.add_member("Anchor", Anchor);
  visitor.add_member("Offset", Offset);
  visitor.add_member("Interactive", Interactive);
}

////////////////////////////////////////////////////////////////////////////////

}

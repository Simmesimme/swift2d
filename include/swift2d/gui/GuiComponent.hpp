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

#ifndef SWIFT2D_GUI_COMPONENT_HPP
#define SWIFT2D_GUI_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/components/DepthComponent.hpp>
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/gui/Interface.hpp>
#include <swift2d/utils/stl_helpers.hpp>

#include <unordered_map>

namespace Awesomium {
  class WebView;
  class JSValue;
}

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class GuiComponent;
typedef std::shared_ptr<GuiComponent>       GuiComponentPtr;
typedef std::shared_ptr<const GuiComponent> ConstGuiComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL GuiComponent : public Component,
                               public DepthComponent,
                               public std::enable_shared_from_this<GuiComponent> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------- inner classes
  struct Serialized : public SerializedComponent {
    float                Opacity;
    math::vec2i          Size;
    math::vec2           Anchor;
    math::vec2           Offset;
    ConstGuiComponentPtr Self;
  };

  class Renderer : public ResourceRenderer<GuiComponent> {
    void draw(RenderContext const& ctx, int start, int end);
  };

  // ---------------------------------------------------------------- properties
  Float  Opacity;
  String Resource;
  Vec2i  Size;
  Vec2   Anchor;
  Vec2   Offset;
  Bool   Interactive;

  // ------------------------------------------------------------------- signals
  Signal<std::string, std::vector<std::string>> on_javascript_callback;
  Signal<>                                      on_loaded;

  // ---------------------------------------------------- construction interface
  GuiComponent();
  ~GuiComponent();

  static GuiComponentPtr create() {
    return std::make_shared<GuiComponent>();
  }

  // creates a copy from this
  GuiComponentPtr create_copy() const {
    return std::make_shared<GuiComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "GuiComponent"; }

  void reload();
  void focus();

  template<typename ...Args>
  void call_javascript(std::string const& method, Args&& ... a) const {
    std::vector<std::string> args = {(to_string(a))...};
    call_javascript_impl(method, args);
  }

  void add_javascript_callback(std::string const& name);
  void add_javascript_getter(std::string const& name, std::function<std::string()> callback);

  virtual void update(double time);
  virtual void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);

  std::unordered_map<std::string, std::function<std::string()>> const& get_result_callbacks() const {
    return result_callbacks_;
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void call_javascript_impl(std::string const& method, std::vector<std::string> const& args) const;
  void update_mouse_position(math::vec2 const& pos) const;
  void add_javascript_callback(std::string const& callback, bool with_result);

  std::unordered_map<std::string, std::function<std::string()>> result_callbacks_;
  Awesomium::WebView* view_;
  Awesomium::JSValue* js_window_;
  std::vector<int>    callbacks_;
  bool                interactive_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_GUI_COMPONENT_HPP

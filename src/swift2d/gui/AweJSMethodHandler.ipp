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

class AweJSMethodHandler : public Awesomium::JSMethodHandler {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  AweJSMethodHandler(GuiComponent* parent)
    : parent_(parent) {}

  // ------------------------------------------------------------ public methods
  void OnMethodCall(
    Awesomium::WebView* caller, unsigned int remote_object_id,
    Awesomium::WebString const& method_name, Awesomium::JSArray const& jargs) {

    std::vector<std::string> args;

    for (int i(0); i<jargs.size(); ++i) {
      args.push_back(Awesomium::ToString(jargs.At(i).ToString()));
    }

    parent_->on_javascript_callback.emit(Awesomium::ToString(method_name), args);
  }

  Awesomium::JSValue OnMethodCallWithReturnValue(
    Awesomium::WebView* caller, unsigned int remote_object_id,
    Awesomium::WebString const& method_name, Awesomium::JSArray const& jargs) {

    auto name(Awesomium::ToString(method_name));
    auto callback(parent_->get_result_callbacks().find(name));
    if (callback != parent_->get_result_callbacks().end()) {
      return Awesomium::JSValue(Awesomium::WSLit(callback->second().c_str()));
    }

    return Awesomium::JSValue::Undefined();
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  GuiComponent* parent_;

};

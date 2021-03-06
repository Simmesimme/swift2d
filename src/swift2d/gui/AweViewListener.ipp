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

class AweViewListener : public Awesomium::WebViewListener::View {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  void OnChangeTitle(
    Awesomium::WebView* caller,
    const Awesomium::WebString& title) {}

  void OnChangeAddressBar(
    Awesomium::WebView* caller,
    const Awesomium::WebURL& url) {}

  void OnChangeTooltip(
    Awesomium::WebView* caller,
    const Awesomium::WebString& tooltip) {}

  void OnChangeTargetURL(
    Awesomium::WebView* caller,
    const Awesomium::WebURL& url) {}

  void OnChangeCursor(
    Awesomium::WebView* caller,
    Awesomium::Cursor cursor) {

    Interface::get().on_cursor_change.emit(static_cast<Cursor>(cursor));
  }

  void OnChangeFocus(
    Awesomium::WebView* caller,
    Awesomium::FocusedElementType focused_type) {}

  void OnAddConsoleMessage(
    Awesomium::WebView* caller, const Awesomium::WebString& message,
    int line_number, const Awesomium::WebString& source) {

    LOG_MESSAGE << message << " (" << source << ":"
                        << line_number << ")" << std::endl;
  }

  void OnShowCreatedWebView(
    Awesomium::WebView* caller, Awesomium::WebView* new_view,
    const Awesomium::WebURL& opener_url, const Awesomium::WebURL& target_url,
    const Awesomium::Rect& initial_pos, bool is_popup) {}

};

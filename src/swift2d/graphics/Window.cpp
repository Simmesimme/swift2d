////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/Window.hpp>
#include <swift2d/graphics/WindowManager.hpp>
#include <swift2d/utils/Logger.hpp>

#include <GLFW/glfw3.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Window::Window()
  : VSync(false)
  , Open(false)
  , Fullscreen(false)
  , ShadingQuality(5)
  , SubSampling(false)
  , window_(nullptr)
  , joystick_axis_cache_(static_cast<int>(JoystickId::JOYSTICK_NUM),
                         std::vector<float>(
                         static_cast<int>(JoystickAxisId::JOYSTICK_AXIS_NUM)))
  , joystick_button_cache_(static_cast<int>(JoystickId::JOYSTICK_NUM),
                           std::vector<int>(
                           static_cast<int>(
                           JoystickButtonId::JOYSTICK_BUTTON_NUM)))
  , vsync_dirty_(true)
  , fullscreen_dirty_(false)
  , init_glew_(true) {

  Open.on_change().connect([this](bool val) {
    if (val) open();
    else     close();
  });

  VSync.on_change().connect([this](bool) {
    vsync_dirty_ = true;
  });

  Fullscreen.on_change().connect([this](bool) {
    fullscreen_dirty_ = true;
  });
}

////////////////////////////////////////////////////////////////////////////////

Window::~Window() {
  close();
  glfwDestroyWindow(window_);
}


////////////////////////////////////////////////////////////////////////////////

void Window::process_input() {
  if (window_) {
    glfwPollEvents();
    update_joysticks();
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::set_active(bool active) {
  if (window_) {
    glfwMakeContextCurrent(window_);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::display() {

  if (vsync_dirty_) {
    vsync_dirty_ = false;
    glfwSwapInterval(VSync() ? 1 : 0);
  }

  if (window_) {
    glfwSwapBuffers(window_);
  }

  // if (fullscreen_dirty_) {
  //   fullscreen_dirty_ = false;

  //   close();
  //   open();
  // }
}

////////////////////////////////////////////////////////////////////////////////

bool Window::key_pressed(Key key) const {
  if (!window_) {
    return false;
  }

  return glfwGetKey(window_, (int)key) == GLFW_PRESS;
}

////////////////////////////////////////////////////////////////////////////////

math::vec2 Window::get_cursor_pos() const {
  if (!window_) {
    return math::vec2(0.f, 0.f);
  }
  double x, y;
  glfwGetCursorPos(window_, &x, &y);
  int height(render_context_.window_size.y());
  return math::vec2(x, height-y);
}

////////////////////////////////////////////////////////////////////////////////

void Window::open() {

  if (!window_) {

    if (Fullscreen()) {
      auto desktop_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
      int desktop_height = desktop_mode->height;
      int desktop_width = desktop_mode->width;

      render_context_.window_size = math::vec2i(desktop_width, desktop_height);
    } else {
      render_context_.window_size = math::vec2i(1024, 768);
    }

    // glfwWindowHint(GLFW_DECORATED, false);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(
      render_context_.window_size.x(), render_context_.window_size.y(),
      Title().c_str(), Fullscreen() ? glfwGetPrimaryMonitor() : nullptr, nullptr);

    WindowManager::instance()->glfw_windows[window_] = this;

    set_active(true);

    render_context_.gl.Disable(oglplus::Capability::DepthTest);
    render_context_.gl.DepthMask(false);

    glfwSetWindowCloseCallback(window_, [](GLFWwindow* w) {
      WindowManager::instance()->glfw_windows[w]->on_close.emit();
    });

    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* w, int width, int height) {
      WindowManager::instance()->glfw_windows[w]->get_context().window_size = math::vec2i(width, height);
      WindowManager::instance()->glfw_windows[w]->on_resize.emit(math::vec2i(width, height));
    });

    glfwSetKeyCallback(window_, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
      WindowManager::instance()->glfw_windows[w]->on_key_press.emit(static_cast<Key>(key), scancode, action, mods);
    });

    glfwSetCursorPosCallback(window_, [](GLFWwindow* w, double x, double y) {
      auto window(WindowManager::instance()->glfw_windows[w]);
      int height(window->get_context().window_size.y());
      window->on_mouse_move.emit(math::vec2(x, height - y));
    });

    glfwSetMouseButtonCallback(window_, [](GLFWwindow* w, int button, int action, int mods) {
      WindowManager::instance()->glfw_windows[w]->on_mouse_button_press.emit(static_cast<Button>(button), action, mods);
    });

    glfwSetScrollCallback(window_, [](GLFWwindow* w, double x, double y) {
      WindowManager::instance()->glfw_windows[w]->on_mouse_scroll.emit(math::vec2(x, y));
    });

    glfwSetCharCallback(window_, [](GLFWwindow* w, unsigned c) {
      WindowManager::instance()->glfw_windows[w]->on_char.emit(c);
    });

    // hide cursor -------------------------------------------------------------
    auto on_hide_cursor_change = [&](bool val) {
      glfwSetInputMode(window_, GLFW_CURSOR, val ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
    };
    on_hide_cursor_change(HideCursor.get());
    HideCursor.on_change().connect(on_hide_cursor_change);
  }

  if (init_glew_) {
    init_glew_ = false;

    // init glew... seems a bit hacky, but works this way
    glewExperimental = GL_TRUE;
    glewInit(); glGetError();

    render_context_.ready = true;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::close() {
  if (window_) {
    glfwDestroyWindow(window_);
    WindowManager::instance()->glfw_windows.erase(window_);
    window_ = nullptr;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::update_joysticks() {

  const int joystick_num(static_cast<int>(JoystickId::JOYSTICK_NUM));
  for (int joy(0); joy < joystick_num; ++joy) {
    if (glfwJoystickPresent(joy)) {
      JoystickId joy_id(static_cast<JoystickId>(joy));
      int axes_count(0);
      auto axes_array(glfwGetJoystickAxes(joy, &axes_count));
      for (int axis(0); axis < axes_count; ++axis) {
        float axis_value(axes_array[axis]);
        if (axis_value != joystick_axis_cache_[joy][axis]) {
          JoystickAxisId axis_id(static_cast<JoystickAxisId>(axis));
          on_joystick_axis_changed.emit(joy_id, axis_id, axis_value);
          joystick_axis_cache_[joy][axis] = axis_value;
        }
      }

      int button_count(0);
      auto button_array(glfwGetJoystickButtons(joy, &button_count));
      for (int button(0); button < button_count; ++button) {
        JoystickButtonId button_id(static_cast<JoystickButtonId>(button));
        int button_value(static_cast<int>(button_array[button]));

        if (button_value != joystick_button_cache_[joy][button]) {

          if (button_value == 0) {
            on_joystick_button_released.emit(joy_id, button_id);
          }
          else if (button_value == 1) {
            on_joystick_button_pressed.emit(joy_id, button_id);
          }

          joystick_button_cache_[joy][button] = button_value;
        }

      }
    }
  }

}

////////////////////////////////////////////////////////////////////////////////

}

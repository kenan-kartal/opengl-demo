#ifndef GLFW_ADAPTERS_H
#define GLFW_ADAPTERS_H

#include "common.h"

#include <stdexcept>
#include <string>

namespace glfw {

void error_callback(int error, const char *description);

class Init {
public:
  explicit Init() {
    glfwSetErrorCallback(error_callback);
    if (glfwInit() != GLFW_TRUE) {
      throw std::runtime_error("Failed to initialize GLFW.");
    }
  }
  Init(const Init &) = delete;
  Init(Init &&) = delete;
  Init &operator=(const Init &) = delete;
  Init &operator=(Init &&) = delete;

  ~Init() { glfwTerminate(); }
};

class Window {
public:
  explicit Window(int width, int height, const char *title,
                  GLFWmonitor *monitor, GLFWwindow *share) {
    _window = glfwCreateWindow(width, height, title, monitor, share);
    if (!_window) {
      throw std::runtime_error("Failed to create GLFW window.");
    }
  }

  ~Window() { glfwDestroyWindow(_window); }

  GLFWwindow *handle() const { return _window; }

private:
  GLFWwindow *_window;
};
} // namespace glfw

#endif

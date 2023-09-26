#include "gl-adapters.h"
#include "glfw-adapters.h"

#include "config.h"

#include <array>
#include <iostream>
#include <memory>
#include <span>
#include <stdexcept>

struct Program;

void init(Program &prog);
void render(Program &prog);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);

constexpr std::array VERTICES{// a
                              -.5f, -.5f, .0f,
                              // b
                              .5f, -.5f, .0f,
                              // c
                              .0f, .5f, .0f};

struct Program {
  std::unique_ptr<glfw::Init> init;
  std::unique_ptr<glfw::Window> window;
  std::unique_ptr<gl::Buffer_names> buffer_names;
};

int main() {
  try {
    Program prog;
    init(prog);

    GLFWwindow *window = prog.window->handle();
    while (glfwWindowShouldClose(window) != GLFW_TRUE) {
      render(prog);
      glfwPollEvents();
      glfwSwapBuffers(window);
    }
  } catch (const std::exception &except) {
    std::cerr << except.what() << '\n';
  }
  return 0;
}

void init(Program &prog) {
  prog.init = std::make_unique<glfw::Init>();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config::opengl::VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config::opengl::VERSION_MINOR);
  glfwWindowHint(GLFW_OPENGL_PROFILE, config::glfw::OPENGL_PROFILE);
  prog.window = std::make_unique<glfw::Window>(
      config::window::WIDTH, config::window::HEIGHT, config::window::TITLE,
      nullptr, nullptr);
  GLFWwindow *window = prog.window->handle();
  glfwMakeContextCurrent(window);

  const int glad_version = gladLoadGL(glfwGetProcAddress);
  if (glad_version == 0) {
    throw std::runtime_error("Failed to initialize glad.");
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  prog.buffer_names = std::make_unique<gl::Buffer_names>(1);
  const auto buffer_names = prog.buffer_names->vector();
  glBindBuffer(GL_ARRAY_BUFFER, buffer_names[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES.data(),
               GL_STATIC_DRAW);
}

void render(Program &prog) {
  // GLFWwindow *window = prog.window->handle();
  const std::span clear_color{
      static_cast<const float *>(config::render::CLEAR_COLOR), 4};
  glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
  glClear(GL_COLOR_BUFFER_BIT);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

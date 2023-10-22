#include "common.h"
#include "config.h"
#include "gl-adapters.h"
#include "glfw-adapters.h"
#include "png-adapters.h"

#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace demo::texture {
struct Program;

void init(Program &prog);
void render(Program &prog);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);

struct Program {
  std::unique_ptr<glfw::Init> init;
  std::unique_ptr<glfw::Window> window;
  std::unique_ptr<gl::Vertex_array_names> vertex_array_names;
  std::unique_ptr<gl::Buffer_names> buffer_names;
  std::unique_ptr<gl::Shader> vert_shader;
  std::unique_ptr<gl::Shader> frag_shader;
  std::unique_ptr<gl::Shader_program> shader_program;
  std::unique_ptr<gl::Texture_names> texture_names;
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

  gl_query();

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  prog.vertex_array_names = std::make_unique<gl::Vertex_array_names>(1);
  const auto &vector_array_names = prog.vertex_array_names->vector();
  glBindVertexArray(vector_array_names[0]);
  prog.buffer_names = std::make_unique<gl::Buffer_names>(2);
  const auto &buffer_names = prog.buffer_names->vector();
  const auto vertex_buffer_name = buffer_names[0];
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_name);
  glBufferData(GL_ARRAY_BUFFER, sizeof(config::quad::VERTICES),
               static_cast<const void *>(config::quad::VERTICES),
               GL_STATIC_DRAW);
  const auto vert_indices_buffer_name = buffer_names[1];
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_indices_buffer_name);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(config::quad::TRI_INDS),
               static_cast<const void *>(config::quad::TRI_INDS),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(config::Vertex),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(config::Vertex),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(config::Vertex),
                        reinterpret_cast<void *>(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  const png::Image image0(config::demo::texture::TEXTURE0_FILENAME, nullptr,
                          png_user_error_fn, png_user_warning_fn);
  const png::Image image1(config::demo::texture::TEXTURE1_FILENAME, nullptr,
                          png_user_error_fn, png_user_warning_fn);
  prog.texture_names = std::make_unique<gl::Texture_names>(2);
  const auto texture0_name = prog.texture_names->vector()[0];
  const auto texture1_name = prog.texture_names->vector()[1];
  glBindTexture(GL_TEXTURE_2D, texture0_name);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image0.width(), image0.height(), 0,
               GL_RGBA, GL_UNSIGNED_BYTE, image0.vector().data());
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture1_name);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image1.width(), image1.height(), 0,
               GL_RGBA, GL_UNSIGNED_BYTE, image1.vector().data());
  glGenerateMipmap(GL_TEXTURE_2D);

  prog.vert_shader = std::make_unique<gl::Shader>(GL_VERTEX_SHADER);
  const auto vert_shader_id = prog.vert_shader->id();
  compile_shader(config::demo::texture::VERT_SHADER_FILENAME, vert_shader_id);
  prog.frag_shader = std::make_unique<gl::Shader>(GL_FRAGMENT_SHADER);
  const auto frag_shader_id = prog.frag_shader->id();
  compile_shader(config::demo::texture::FRAG_SHADER_FILENAME, frag_shader_id);
  prog.shader_program = std::make_unique<gl::Shader_program>();
  const auto shader_program_id = prog.shader_program->id();
  glAttachShader(shader_program_id, vert_shader_id);
  glAttachShader(shader_program_id, frag_shader_id);
  link_shader_program(shader_program_id);

  glUseProgram(shader_program_id);
  glUniform1i(glGetUniformLocation(shader_program_id, "texture0"), 0);
  glUniform1i(glGetUniformLocation(shader_program_id, "texture1"), 1);
}

void render(Program &prog) {
  const float *clear_color = config::render::CLEAR_COLOR;
  glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
  glClear(GL_COLOR_BUFFER_BIT);

  const auto shader_prog_id = prog.shader_program->id();
  glUseProgram(shader_prog_id);

  const auto &texture_names = prog.texture_names->vector();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_names[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture_names[1]);
  glBindVertexArray(prog.vertex_array_names->vector()[0]);
  glDrawElements(GL_TRIANGLES,
                 sizeof(config::quad::TRI_INDS) / sizeof(unsigned),
                 GL_UNSIGNED_INT, reinterpret_cast<void *>(0));
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}
} // namespace demo::texture

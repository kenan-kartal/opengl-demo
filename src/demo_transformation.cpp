#include "config.h"
#include "gl-adapters.h"
#include "glfw-adapters.h"
#include "png-adapters.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <png.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <span>
#include <stdexcept>

namespace demo::transformation {

struct Program;

void init(Program &prog);
void render(Program &prog);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);

void query();
void compile_shader(const char *filename, GLuint shader_id);
void link_shader_program(GLuint program_id);

void png_user_error_fn(png_structp png_ptr, png_const_charp error_msg);
void png_user_warning_fn(png_structp png_ptr, png_const_charp warning_msg);

struct Program {
  std::unique_ptr<glfw::Init> init;
  std::unique_ptr<glfw::Window> window;
  std::unique_ptr<gl::Vertex_array_names> vertex_array_names;
  std::unique_ptr<gl::Buffer_names> buffer_names;
  std::unique_ptr<gl::Shader_program> shader_program;
  std::unique_ptr<gl::Textures> textures;
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

  query();

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  prog.vertex_array_names = std::make_unique<gl::Vertex_array_names>(1);
  const auto &vector_array_names = prog.vertex_array_names->vector();
  glBindVertexArray(vector_array_names[0]);
  prog.buffer_names = std::make_unique<gl::Buffer_names>(2);
  const auto &buffer_names = prog.buffer_names->vector();
  const auto vertex_buffer_name = buffer_names[0];
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_name);
  glBufferData(
      GL_ARRAY_BUFFER, sizeof(config::demo::transformation::VERTICES),
      static_cast<const void *>(config::demo::transformation::VERTICES),
      GL_STATIC_DRAW);
  const auto vert_indices_buffer_name = buffer_names[1];
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_indices_buffer_name);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      sizeof(config::demo::transformation::VERT_INDICES),
      static_cast<const void *>(config::demo::transformation::VERT_INDICES),
      GL_STATIC_DRAW);
  //  NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast,
  //  performance-no-int-to-ptr): Be explicit about offset.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        sizeof(config::demo::transformation::Vertex_data),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                        sizeof(config::demo::transformation::Vertex_data),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast,
  // performance-no-int-to-ptr)
  const png::Image image(config::demo::transformation::TEXTURE_FILENAME,
                         nullptr, png_user_error_fn, png_user_warning_fn);
  prog.textures = std::make_unique<gl::Textures>(1);
  const auto texture_name = prog.textures->vector()[0];
  glBindTexture(GL_TEXTURE_2D, texture_name);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0,
               GL_RGBA, GL_UNSIGNED_BYTE, image.vector().data());
  glGenerateMipmap(GL_TEXTURE_2D);

  gl::Shader vert_shader{GL_VERTEX_SHADER};
  compile_shader(config::demo::transformation::VERT_SHADER_FILENAME,
                 vert_shader.id());
  gl::Shader frag_shader{GL_FRAGMENT_SHADER};
  compile_shader(config::demo::transformation::FRAG_SHADER_FILENAME,
                 frag_shader.id());
  prog.shader_program = std::make_unique<gl::Shader_program>();
  const auto shader_program_id = prog.shader_program->id();
  glAttachShader(shader_program_id, vert_shader.id());
  glAttachShader(shader_program_id, frag_shader.id());
  link_shader_program(shader_program_id);

  glUseProgram(shader_program_id);
  glUniform1i(glGetUniformLocation(shader_program_id, "texture0"), 0);
}

void render(Program &prog) {
  // GLFWwindow *window = prog.window->handle();
  const std::span clear_color{
      static_cast<const float *>(config::render::CLEAR_COLOR), 4};
  glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
  glClear(GL_COLOR_BUFFER_BIT);

  const auto shader_prog_id = prog.shader_program->id();
  glUseProgram(shader_prog_id);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, prog.textures->vector()[0]);

  float time = glfwGetTime();
  glm::mat4 trans{1.f};
  glm::vec3 translation{cosf(time) / 2.f, sinf(time) / 2.f, 0.f};
  float rotation{time};
  glm::vec3 scale{cosf(time)};
  trans = glm::translate(trans, translation);
  trans = glm::rotate(trans, rotation, glm::vec3{0.f, 0.f, 1.f});
  trans = glm::scale(trans, scale);
  GLint transform_loc{glGetUniformLocation(shader_prog_id, "transform")};
  glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(trans));

  glBindVertexArray(prog.vertex_array_names->vector()[0]);
  // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast,
  // performance-no-int-to-ptr): Be explicit about offset.
  glDrawElements(GL_TRIANGLES,
                 sizeof(config::demo::transformation::VERT_INDICES) /
                     sizeof(unsigned int),
                 GL_UNSIGNED_INT, reinterpret_cast<void *>(0));
  // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast,
  // performance-no-int-to-ptr)
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

void query() {
  // NOLINTBEGIN(cppcoreguidelines-init-variables): Initialized by query calls.
  GLint num_attrs;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num_attrs);
  std::cout << "Max number of vertex attributes supported: " << num_attrs
            << '\n';
  // NOLINTEND(cppcoreguidelines-init-variables)
}

void compile_shader(const char *filename, const GLuint shader_id) {
  using std::string_literals::operator""s;

  std::ifstream file{filename, std::ios::in | std::ios::ate};
  if (!file) {
    throw std::runtime_error("Failed to open file: "s + filename);
  }
  const size_t size = file.tellg();
  std::string source(size, '\0');
  file.seekg(0);
  file.read(source.data(), static_cast<std::streamsize>(size));
  const GLchar *sources = source.data();
  const auto lengths = static_cast<GLint>(size);
  glShaderSource(shader_id, 1, &sources, &lengths);
  std::cout << "Compiling shader: " << filename << '\n';
  glCompileShader(shader_id);
  GLint res; // NOLINT(cppcoreguidelines-init-variables): Initialized next line.
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
  if (res == GL_FALSE) {
    GLint info_len; // NOLINT(cppcoreguidelines-init-variables): Initialized
                    // next line.
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_len);
    if (info_len > 0) {
      std::vector<GLchar> info(info_len);
      glGetShaderInfoLog(shader_id, info_len, nullptr, info.data());
      std::cerr << static_cast<char *>(info.data()) << '\n';
    } else {
      std::cerr << "No info.\n";
    }
    throw std::runtime_error("Compilation failed.");
  }
  std::cout << "Compiled.\n";
}

void link_shader_program(GLuint program_id) {
  std::cout << "Linking shader program.\n";
  glLinkProgram(program_id);
  GLint res; // NOLINT(cppcoreguidelines-init-variables): Initialized next line.
  glGetProgramiv(program_id, GL_LINK_STATUS, &res);
  if (res == GL_FALSE) {
    GLint info_len; // NOLINT(cppcoreguidelines-init-variables): Initialized
                    // next line.
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_len);
    if (info_len > 0) {
      std::vector<GLchar> info(info_len);
      glGetProgramInfoLog(program_id, info_len, nullptr, info.data());
      std::cerr << static_cast<char *>(info.data()) << '\n';
    } else {
      std::cerr << "No info.\n";
    }
    throw std::runtime_error("Linking failed.");
  }
  std::cout << "Linked.\n";
}

void png_user_error_fn(png_structp png_ptr, png_const_charp error_msg) {
  using std::string_literals::operator""s;
  throw std::runtime_error("PNG error: "s + error_msg);
}

void png_user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
  std::cerr << "PNG warning: " << warning_msg << '\n';
}

} // namespace demo::transformation

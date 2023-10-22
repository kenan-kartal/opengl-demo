#include "common.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void gl_query() {
  GLint num_attrs;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num_attrs);
  std::cout << "Max number of vertex attributes supported: " << num_attrs
            << '\n';
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
  GLint res;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
  if (res == GL_FALSE) {
    GLint info_len;
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
  GLint res;
  glGetProgramiv(program_id, GL_LINK_STATUS, &res);
  if (res == GL_FALSE) {
    GLint info_len;
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

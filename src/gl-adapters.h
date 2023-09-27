#ifndef GL_ADAPTERS_H
#define GL_ADAPTERS_H

#include "common.h"

#include <vector>

namespace gl {

class Vertex_array_names {
public:
  explicit Vertex_array_names(GLsizei size) : _names(size) {
    glGenVertexArrays(size, _names.data());
  }
  ~Vertex_array_names() {
    glDeleteVertexArrays(static_cast<GLsizei>(_names.size()), _names.data());
  }

  const auto &vector() { return _names; }

private:
  std::vector<GLuint> _names;
};

class Buffer_names {
public:
  explicit Buffer_names(GLsizei size) : _names(size) {
    glGenBuffers(size, _names.data());
  }
  ~Buffer_names() {
    glDeleteBuffers(static_cast<GLsizei>(_names.size()), _names.data());
  }

  const auto &vector() { return _names; }

private:
  std::vector<GLuint> _names;
};

class Shader {
public:
  explicit Shader(GLenum type) { _id = glCreateShader(type); }
  ~Shader() { glDeleteShader(_id); }

  auto id() { return _id; }

private:
  GLuint _id;
};

class Shader_program {
public:
  explicit Shader_program() { _id = glCreateProgram(); };
  ~Shader_program() { glDeleteProgram(_id); }

  auto id() { return _id; }

private:
  GLuint _id;
};

} // namespace gl

#endif

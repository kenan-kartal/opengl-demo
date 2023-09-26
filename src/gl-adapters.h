#ifndef GL_ADAPTERS_H
#define GL_ADAPTERS_H

#include "common.h"

#include <vector>

namespace gl {

class Buffer_names {
public:
  explicit Buffer_names(GLsizei size) : _names(size) {
    glGenBuffers(size, _names.data());
  }
  Buffer_names(const Buffer_names &other) = delete;
  Buffer_names(Buffer_names &&other) = delete;
  Buffer_names &operator=(const Buffer_names &other) = delete;
  Buffer_names &operator=(Buffer_names &&other) = delete;

  ~Buffer_names() {
    glDeleteBuffers(static_cast<GLsizei>(_names.size()), _names.data());
  }

  const auto &vector() { return _names; }

private:
  std::vector<GLuint> _names;
};

} // namespace gl

#endif

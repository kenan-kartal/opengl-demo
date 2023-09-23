#include "glfw-adapters.h"

#include <iostream>

namespace glfw {

void error_callback(int error, const char *description) {
  std::cerr << "GLFW Error: " << error << ", " << description << '\n';
}

} // namespace glfw

#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"

#include <array>

namespace config {

namespace opengl {
constexpr int VERSION_MAJOR = 3;
constexpr int VERSION_MINOR = 3;
} // namespace opengl

namespace window {
constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;
constexpr const char *TITLE = "OpenGL Demo";
} // namespace window

namespace glfw {
constexpr int OPENGL_PROFILE = GLFW_OPENGL_CORE_PROFILE;
}

namespace render {
constexpr float CLEAR_COLOR[4] = {0.06f, 0.30f, 0.32f, 1.0f};
}

constexpr const char *VERT_SHADER_FILENAME = "res/simple.vert";
constexpr const char *FRAG_SHADER_FILENAME = "res/simple.frag";
constexpr std::array VERTICES{// a
                              -.5f, -.5f, .0f,
                              // b
                              .5f, -.5f, .0f,
                              // c
                              .0f, .5f, .0f};

} // namespace config

#endif

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
constexpr std::array VERTICES{
    .5f,  .5f,  .0f, // top right
    .5f,  -.5f, .0f, // bottom right
    -.5f, -.5f, .0f, // bottom left
    -.5f, .5f,  .0f, // top left
};
constexpr std::array VERT_INDICES{
    0, 1, 3, // 1st
    1, 2, 3  // 2nd
};

} // namespace config

#endif

#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"

namespace config {

struct Vertex {
  struct Position {
    float x;
    float y;
    float z;
  };
  struct Colour {
    float r;
    float g;
    float b;
  };
  struct Tex_coord {
    float s;
    float t;
  };
  Position pos;
  Colour col;
  Tex_coord tex;
};

struct Face {
  unsigned a;
  unsigned b;
  unsigned c;
};

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

namespace triangle {
constexpr Vertex VERTICES[] = {
    Vertex{-0.5F, -0.5F, 0.F, 1.F, 0.F, 0.F, 0.F, 0.F},
    Vertex{0.5F, -0.5F, 0.F, 0.F, 1.F, 0.F, 1.F, 0.F},
    Vertex{0.F, 0.5F, 0.F, 0.F, 0.F, 1.F, 0.5F, 1.F}};
constexpr Face FACES[] = {Face{0, 1, 2}};
} // namespace triangle

namespace quad {
constexpr Vertex VERTICES[] = {
    Vertex{-0.5F, -0.5F, 0.F, 0.F, 0.F, 0.F, 0.F, 0.F},
    Vertex{0.5F, -0.5F, 0.F, 1.F, 0.F, 0.F, 1.F, 0.F},
    Vertex{-0.5F, 0.5F, 0.F, 1.F, 0.F, 0.F, 0.F, 1.F},
    Vertex{0.5F, 0.5F, 0.F, 1.F, 1.F, 0.F, 1.F, 1.F}};
constexpr Face FACES[] = {Face{0, 1, 2}, Face{2, 1, 3}};
} // namespace quad

namespace cube {
constexpr Vertex VERTICES[] = {
    Vertex{-0.5F, -0.5F, 0.5F, 0.F, 0.F, 0.F, 0.F, 0.F},
    Vertex{0.5F, -0.5F, 0.5F, 1.F, 0.F, 0.F, 1.F, 0.F},
    Vertex{-0.5F, 0.5F, 0.5F, 1.F, 0.F, 0.F, 0.F, 1.F},
    Vertex{0.5F, 0.5F, 0.5F, 1.F, 1.F, 0.F, 1.F, 1.F},
    Vertex{-0.5F, -0.5F, -0.5F, 0.F, 0.F, 0.F, 0.F, 0.F},
    Vertex{0.5F, -0.5F, -0.5F, 1.F, 0.F, 0.F, 1.F, 0.F},
    Vertex{-0.5F, 0.5F, -0.5F, 1.F, 0.F, 0.F, 0.F, 1.F},
    Vertex{0.5F, 0.5F, -0.5F, 1.F, 1.F, 0.F, 1.F, 1.F}};
constexpr Face FACES[] = {Face{0, 1, 2}, Face{2, 1, 3}, Face{4, 6, 5},
                          Face{5, 6, 7}, Face{4, 0, 6}, Face{6, 0, 2},
                          Face{1, 5, 3}, Face{3, 5, 7}, Face{0, 4, 1},
                          Face{1, 4, 5}, Face{2, 3, 6}, Face{6, 3, 7}};
} // namespace cube

namespace demo {
namespace simple {
constexpr const char *VERT_SHADER_FILENAME = "res/simple.vert";
constexpr const char *FRAG_SHADER_FILENAME = "res/simple.frag";
} // namespace simple

namespace texture {
constexpr const char *VERT_SHADER_FILENAME = "res/texture.vert";
constexpr const char *FRAG_SHADER_FILENAME = "res/texture.frag";
constexpr const char *TEXTURE0_FILENAME = "res/bricks.png";
constexpr const char *TEXTURE1_FILENAME = "res/metal.png";
} // namespace texture

namespace transformation {
constexpr const char *VERT_SHADER_FILENAME = "res/transformation.vert";
constexpr const char *FRAG_SHADER_FILENAME = "res/transformation.frag";
constexpr const char *TEXTURE_FILENAME = "res/bricks.png";
} // namespace transformation

namespace coordinates {
constexpr const char *VERT_SHADER_FILENAME = "res/coordinates.vert";
constexpr const char *FRAG_SHADER_FILENAME = "res/coordinates.frag";
constexpr const char *TEXTURE_FILENAME = "res/bricks.png";
} // namespace coordinates
} // namespace demo

} // namespace config

#endif

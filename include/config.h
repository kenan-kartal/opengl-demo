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
  struct Normal {
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
  Normal normal;
  Colour col;
  Tex_coord tex;
};

struct Tri_inds {
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
	{
		{-0.5F, -0.5F, 0.F},
		{0.F, 0.F, 1.F},
		{1.F, 0.F, 0.F},
		{0.F, 0.F}
	},
	{
		{0.5F, -0.5F, 0.F},
		{0.F, 0.F, 1.F},
		{0.F, 1.F, 0.F},
		{1.F, 0.F}
	},
	{
		{0.F, 0.5F, 0.F},
		{0.F, 0.F, 1.F},
		{0.F, 0.F, 1.F},
		{0.5F, 1.F}
	}
};
constexpr Tri_inds TRI_INDS[] = {{0, 1, 2}};
} // namespace triangle

namespace quad {
constexpr Vertex VERTICES[] = {
	{
		{-0.5F, -0.5F, 0.F},
		{0.F, 0.F, 1.F},
		{0.F, 0.F, 0.F},
		{0.F, 0.F}
	},
	{
		{0.5F, -0.5F, 0.F},
		{0.F, 0.F, 1.F},
		{1.F, 0.F, 0.F},
		{1.F, 0.F}
	},
	{
		{-0.5F, 0.5F, 0.F},
		{0.F, 0.F, 1.F},
		{0.F, 1.F, 0.F},
		{0.F, 1.F}
	},
	{
		{0.5F, 0.5F, 0.F},
		{0.F, 0.F, 1.F},
		{1.F, 1.F, 0.F},
		{1.F, 1.F}
	}
};
constexpr Tri_inds TRI_INDS[] = {{0, 1, 2}, {2, 1, 3}};
} // namespace quad

namespace cube {
constexpr Vertex VERTICES[] = {
	{
		{-0.5F, -0.5F, 0.5F},
		{0.F, 0.F, 1.F},
		{0.F, 0.F, 0.F},
		{0.F, 0.F}
	},
	{
		{0.5F, -0.5F, 0.5F},
		{0.F, 0.F, 1.F},
		{1.F, 0.F, 0.F},
		{1.F, 0.F}
	},
	{
		{-0.5F, 0.5F, 0.5F},
		{0.F, 0.F, 1.F},
		{0.F, 1.F, 0.F},
		{0.F, 1.F}
	},
	{
		{0.5F, 0.5F, 0.5F},
		{0.F, 0.F, 1.F},
		{1.F, 1.F, 0.F},
		{1.F, 1.F}
	},

	{
		{0.5F, -0.5F, 0.5F},
		{1.F, 0.F, 0.F},
		{1.F, 0.F, 0.F},
		{0.F, 0.F}
	},
	{
		{0.5F, -0.5F, -0.5F},
		{1.F, 0.F, 0.F},
		{1.F, 0.F, 1.F},
		{1.F, 0.F}
	},
	{
		{0.5F, 0.5F, 0.5F},
		{1.F, 0.F, 0.F},
		{1.F, 1.F, 0.F},
		{0.F, 1.F}
	},
	{
		{0.5F, 0.5F, -0.5F},
		{1.F, 0.F, 0.F},
		{1.F, 1.F, 1.F},
		{1.F, 1.F}
	},

	{
		{-0.5F, -0.5F, -0.5F},
		{0.F, 0.F, -1.F},
		{0.F, 0.F, 1.F},
		{0.F, 0.F}
	},
	{
		{0.5F, -0.5F, -0.5F},
		{0.F, 0.F, -1.F},
		{1.F, 0.F, 1.F},
		{1.F, 0.F}
	},
	{
		{-0.5F, 0.5F, -0.5F},
		{0.F, 0.F, -1.F},
		{0.F, 1.F, 1.F},
		{0.F, 1.F}
	},
	{
		{0.5F, 0.5F, -0.5F},
		{0.F, 0.F, -1.F},
		{1.F, 1.F, 1.F},
		{1.F, 1.F}
	},

	{
		{-0.5F, -0.5F, -0.5F},
		{-1.F, 0.F, 0.F},
		{0.F, 0.F, 1.F},
		{0.F, 0.F}
	},
	{
		{-0.5F, -0.5F, 0.5F},
		{-1.F, 0.F, 0.F},
		{0.F, 0.F, 0.F},
		{1.F, 0.F}
	},
	{
		{-0.5F, 0.5F, -0.5F},
		{-1.F, 0.F, 0.F},
		{0.F, 1.F, 1.F},
		{0.F, 1.F}
	},
	{
		{-0.5F, 0.5F, 0.5F},
		{-1.F, 0.F, 0.F},
		{0.F, 1.F, 0.F},
		{1.F, 1.F}
	},

	{
		{0.5F, -0.5F, 0.5F},
		{0.F, -1.F, 0.F},
		{1.F, 0.F, 0.F},
		{0.F, 0.F}
	},
	{
		{-0.5F, -0.5F, 0.5F},
		{0.F, -1.F, 0.F},
		{0.F, 0.F, 0.F},
		{1.F, 0.F}
	},
	{
		{0.5F, -0.5F, -0.5F},
		{0.F, -1.F, 0.F},
		{1.F, 0.F, 1.F},
		{0.F, 1.F}
	},
	{
		{-0.5F, -0.5F, -0.5F},
		{0.F, -1.F, 0.F},
		{0.F, 0.F, 1.F},
		{1.F, 1.F}
	},

	{
		{-0.5F, 0.5F, 0.5F},
		{0.F, 1.F, 0.F},
		{0.F, 1.F, 0.F},
		{0.F, 0.F}
	},
	{
		{0.5F, 0.5F, 0.5F},
		{0.F, 1.F, 0.F},
		{1.F, 1.F, 0.F},
		{1.F, 0.F}
	},
	{
		{-0.5F, 0.5F, -0.5F},
		{0.F, 1.F, 0.F},
		{0.F, 1.F, 1.F},
		{0.F, 1.F}
	},
	{
		{0.5F, 0.5F, -0.5F},
		{0.F, 1.F, 0.F},
		{1.F, 1.F, 1.F},
		{1.F, 1.F}
	}
};
constexpr Tri_inds TRI_INDS[] = {{0, 1, 2},    {2, 1, 3},    {4, 5, 6},
                                 {6, 5, 7},    {8, 9, 10},   {10, 9, 11},
                                 {12, 13, 14}, {14, 13, 15}, {16, 17, 18},
                                 {18, 17, 19}, {20, 21, 22}, {22, 21, 23}};
} // namespace cube
} // namespace config

#endif

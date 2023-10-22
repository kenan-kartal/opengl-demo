#ifndef COMMON_H
#define COMMON_H

// clang-format off: Include ordering is required.
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <png.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void gl_query();
void compile_shader(const char *filename, GLuint shader_id);
void link_shader_program(GLuint program_id);

void png_user_error_fn(png_structp png_ptr, png_const_charp error_msg);
void png_user_warning_fn(png_structp png_ptr, png_const_charp warning_msg);

#endif

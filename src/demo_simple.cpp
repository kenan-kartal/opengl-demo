#include "camera.h"
#include "common.h"
#include "config.h"
#include "gl-adapters.h"
#include "glfw-adapters.h"
#include "png-adapters.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace demo::simple {
constexpr const char *VERT_SHADER_FILENAME = "res/simple.vert";
constexpr const char *FRAG_SHADER_FILENAME = "res/simple.frag";
constexpr const char *TEXTURE_FILENAME = "res/bricks.png";

struct Program;

Program *program;

void init(Program &prog);
void render(Program &prog);
void start(Program &prog);
void update(Program &prog);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

struct Program {
  std::unique_ptr<glfw::Init> init;
  std::unique_ptr<glfw::Window> window;
  std::unique_ptr<gl::Vertex_array_names> vertex_array_names;
  std::unique_ptr<gl::Buffer_names> buffer_names;
  std::unique_ptr<gl::Shader_program> shader_program;
  std::unique_ptr<gl::Texture_names> texture_names;
  Camera cam{};
  float delta{};
  glm::vec2 mouse_pos{};
};

int main() {
  try {
    Program prog;
    program = &prog;
    init(prog);

    start(prog);
    GLFWwindow *window = prog.window->handle();
    float last = glfwGetTime();
    while (glfwWindowShouldClose(window) != GLFW_TRUE) {
      float now = glfwGetTime();
      prog.delta = now - last;
      last = now;
      glfwPollEvents();
      update(prog);
      render(prog);
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

  gl_query();

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetKeyCallback(window, key_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glEnable(GL_DEPTH_TEST);

  prog.vertex_array_names = std::make_unique<gl::Vertex_array_names>(1);
  const auto &vector_array_names = prog.vertex_array_names->vector();
  glBindVertexArray(vector_array_names[0]);
  prog.buffer_names = std::make_unique<gl::Buffer_names>(2);
  const auto &buffer_names = prog.buffer_names->vector();
  const auto vertex_buffer_name = buffer_names[0];
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_name);
  glBufferData(GL_ARRAY_BUFFER, sizeof(config::cube::VERTICES),
               static_cast<const void *>(config::cube::VERTICES),
               GL_STATIC_DRAW);
  const auto vert_indices_buffer_name = buffer_names[1];
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_indices_buffer_name);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(config::cube::TRI_INDS),
               static_cast<const void *>(config::cube::TRI_INDS),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(config::Vertex),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(config::Vertex),
                        reinterpret_cast<void *>(offsetof(config::Vertex, tex)));
  glEnableVertexAttribArray(1);
  const png::Image image(TEXTURE_FILENAME, nullptr,
                         png_user_error_fn, png_user_warning_fn);
  prog.texture_names = std::make_unique<gl::Texture_names>(1);
  const auto texture_name = prog.texture_names->vector()[0];
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
  compile_shader(VERT_SHADER_FILENAME, vert_shader.id());
  gl::Shader frag_shader{GL_FRAGMENT_SHADER};
  compile_shader(FRAG_SHADER_FILENAME, frag_shader.id());
  prog.shader_program = std::make_unique<gl::Shader_program>();
  const auto shader_program_id = prog.shader_program->id();
  glAttachShader(shader_program_id, vert_shader.id());
  glAttachShader(shader_program_id, frag_shader.id());
  link_shader_program(shader_program_id);

  glUseProgram(shader_program_id);
  glUniform1i(glGetUniformLocation(shader_program_id, "texture0"), 0);

  prog.cam.set_pos({0.F, 0.F, 30.F});
}

void render(Program &prog) {
  const float *clear_color = config::render::CLEAR_COLOR;
  glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  const auto shader_prog_id = prog.shader_program->id();
  glUseProgram(shader_prog_id);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, prog.texture_names->vector()[0]);

  auto *window = prog.window->handle();
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  float aspect = static_cast<float>(width) / static_cast<float>(height);

  glBindVertexArray(prog.vertex_array_names->vector()[0]);

  Camera &cam = prog.cam;
  glm::mat4 view = cam.look();
  glm::mat4 projection = glm::perspective(cam.fov(), aspect, 0.1F, 100.F);
  GLint view_loc{glGetUniformLocation(shader_prog_id, "view")};
  glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
  GLint projection_loc{glGetUniformLocation(shader_prog_id, "projection")};
  glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

  for (int i = 0; i < 6; ++i) {
    for (int j = 0; j < 6; ++j) {
      for (int k = 0; k < 6; ++k) {
        glm::vec3 translation{-6.F + i * 2.F, -6.F + j * 2.F, -6.F + k * 2.F};
        glm::mat4 model{1.F};
        model = glm::translate(model, translation);
        GLint model_loc{glGetUniformLocation(shader_prog_id, "model")};
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES,
                       sizeof(config::cube::TRI_INDS) / sizeof(unsigned),
                       GL_UNSIGNED_INT, reinterpret_cast<void *>(0));
      }
    }
  }
}

void start(Program &prog) {
  glfwPollEvents();
  GLFWwindow *window = prog.window->handle();
  double mouse_x, mouse_y;
  glfwGetCursorPos(window, &mouse_x, &mouse_y);
  prog.mouse_pos = {mouse_x, mouse_y};
}

void update(Program &prog) {
  GLFWwindow *window = prog.window->handle();
  float delta = prog.delta;
  Camera &cam = prog.cam;
  glm::vec3 pos = cam.pos();
  const float cam_speed = 10.F;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    pos += cam.view() * cam_speed * delta;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    pos += cam.view() * -cam_speed * delta;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    pos += glm::normalize(glm::cross(cam.view(), cam.up())) * cam_speed * delta;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    pos +=
        -glm::normalize(glm::cross(cam.view(), cam.up())) * cam_speed * delta;
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    pos += cam.up() * cam_speed * delta;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    pos += -cam.up() * cam_speed * delta;
  }
  cam.set_pos(pos);
  double mouse_x, mouse_y;
  glfwGetCursorPos(window, &mouse_x, &mouse_y);
  glm::vec2 mouse_delta{mouse_x - prog.mouse_pos.x, mouse_y - prog.mouse_pos.y};
  prog.mouse_pos.x = mouse_x;
  prog.mouse_pos.y = mouse_y;
  const float mouse_sensitivity = 0.001F;
  float yaw = cam.yaw();
  float pitch = cam.pitch();
  yaw += mouse_delta.x * mouse_sensitivity;
  pitch -= mouse_delta.y * mouse_sensitivity;
  if (pitch > glm::radians(89.F)) {
    pitch = glm::radians(89.F);
  } else if (pitch < glm::radians(-89.F)) {
    pitch = glm::radians(-89.F);
  }
  cam.set_angles(yaw, pitch);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  float fov = program->cam.fov();
  fov -= yoffset * 0.1F;
  if (fov < glm::radians(1.F)) {
    fov = glm::radians(1.F);
  } else if (fov > glm::radians(45.F)) {
    fov = glm::radians(45.F);
  }
  program->cam.set_fov(fov);
}
} // namespace demo::simple

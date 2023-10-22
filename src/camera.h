#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
  explicit Camera() = default;
  explicit Camera(glm::vec3 &&pos, glm::vec3 &&view, glm::vec3 up)
      : _pos{pos}, _view{glm::normalize(view)}, _up{glm::normalize(up)} {
    glm::vec3 right{glm::cross(up, -_view)};
    _up = glm::cross(-_view, right);
  }

  glm::vec3 pos() const { return _pos; }
  glm::vec3 view() const { return _view; }
  glm::vec3 up() const { return _up; }

  void set_pos(const glm::vec3 &pos) { _pos = pos; }

  void set_view(const glm::vec3 &view) {
    _view = glm::normalize(view);
    glm::vec3 right = glm::cross(_up, -_view);
    _up = glm::cross(-_view, right);
  }

  glm::mat4 look() const { return glm::lookAt(_pos, _pos + _view, _up); }

  glm::mat4 look_at(const glm::vec3 &target) const {
    return glm::lookAt(_pos, target, _up);
  }

private:
  glm::vec3 _pos{0.F, 0.F, 0.F};
  glm::vec3 _view{0.F, 0.F, -1.F};
  glm::vec3 _up{0.F, 1.F, 0.F};
};

#endif

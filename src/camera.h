#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
  explicit Camera() = default;
  explicit Camera(glm::vec3 &&pos, glm::vec3 &&view, glm::vec3 up)
      : _pos{pos}, _view{glm::normalize(view)}, _up{glm::normalize(up)} {
    update_from_view();
  }

  glm::vec3 pos() const { return _pos; }
  glm::vec3 view() const { return _view; }
  glm::vec3 up() const { return _up; }
  float yaw() const { return _yaw; }
  float pitch() const { return _pitch; }

  void set_pos(const glm::vec3 &pos) { _pos = pos; }

  void set_view(const glm::vec3 &view) {
    _view = glm::normalize(view);
    update_from_view();
  }

  void set_angles(float yaw, float pitch) {
    _yaw = yaw;
    _pitch = pitch;
    update_from_angles();
  }

  glm::mat4 look() const { return glm::lookAt(_pos, _pos + _view, _up); }

  glm::mat4 look_at(const glm::vec3 &target) const {
    return glm::lookAt(_pos, target, _up);
  }

private:
  void update_from_view() {
    glm::vec3 right{glm::cross(_up, -_view)};
    _up = glm::normalize(glm::cross(-_view, right));
    _pitch = glm::asin(_view.y);
    _yaw = glm::acos(_view.x / glm::cos(_pitch));
  }

  void update_from_angles() {
    _view.x = glm::cos(_yaw) * glm::cos(_pitch);
    _view.y = glm::sin(_pitch);
    _view.z = glm::sin(_yaw) * glm::cos(_pitch);
    _up = {0.F, 1.F, 0.F};
    glm::vec3 right{glm::cross(_up, -_view)};
    _up = glm::normalize(glm::cross(-_view, right));
  }

private:
  glm::vec3 _pos{0.F, 0.F, 0.F};
  glm::vec3 _view{0.F, 0.F, -1.F};
  glm::vec3 _up{0.F, 1.F, 0.F};
  float _yaw{glm::radians(-90.F)};
  float _pitch{0.F};
};

#endif

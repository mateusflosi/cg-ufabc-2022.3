#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

#include "camera.hpp"
#include "planet.hpp"

struct Vertex {
  glm::vec3 position;

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onUpdate() override;

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};

  Camera m_camera;

  float m_dollySpeed{};
  float m_truckSpeed{};
  float m_truckXSpeed{};
  float m_panSpeed{};
  float m_panXSpeed{};
  float m_angle_planet{};
  float m_planet_velocity{10.0f};
  bool m_planet_distance_scale{true};

  std::vector<Planet> m_planets;
};

#endif

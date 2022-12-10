#ifndef PLANET_HPP_
#define PLANET_HPP_

#include "abcgOpenGL.hpp"
#include "camera.hpp"

struct VertexPlanet {
  glm::vec3 position;

  friend bool operator==(VertexPlanet const &, VertexPlanet const &) = default;
};

class Planet {
public:
  void create(GLuint program, std::string nameFile);
  void paint(GLuint program, Camera camera, float m_angle, bool scale);
  void update(float deltaTime);
  void destroy();

  glm::vec3 m_translate_scale = glm::vec3(.0f);
  glm::vec3 m_translate_not_scale = glm::vec3(.0f);
  glm::vec3 m_scale = glm::vec3(.0f);
  glm::vec4 m_color = glm::vec4(.0f);
  float m_velocity{0.0f};

private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  GLint m_viewMatrixLoc{};
  GLint m_projMatrixLoc{};
  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};

  std::vector<VertexPlanet> m_vertices;
  std::vector<GLuint> m_indices;

  void loadModelFromFile(std::string_view path);
};

#endif

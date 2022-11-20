#ifndef SATURN_HPP_
#define SATURN_HPP_

#include "abcgOpenGL.hpp"
#include "camera.hpp"


struct VertexSaturn {
  glm::vec3 position;

  friend bool operator==(VertexSaturn const &, VertexSaturn const &) = default;
};

class Saturn {
public:
  void create(GLuint program);
  void paint(GLint program, Camera m_camera);
  void destroy();

private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};
  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};
  
  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};
  
  std::vector<VertexSaturn> m_vertices;
  std::vector<GLuint> m_indices;
  
  void loadModelFromFile(std::string_view path);
};

#endif

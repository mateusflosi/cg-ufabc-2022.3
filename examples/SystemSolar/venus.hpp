#ifndef VENUS_HPP_
#define VENUS_HPP_

#include "abcgOpenGL.hpp"
#include "camera.hpp"


struct VertexVenus {
  glm::vec3 position;

  friend bool operator==(VertexVenus const &, VertexVenus const &) = default;
};

class Venus {
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
  
  std::vector<VertexVenus> m_vertices;
  std::vector<GLuint> m_indices;
  
  void loadModelFromFile(std::string_view path);
};

#endif

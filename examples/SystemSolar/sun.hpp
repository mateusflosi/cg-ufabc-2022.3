#ifndef SUN_HPP_
#define SUN_HPP_

#include "abcgOpenGL.hpp"
#include "camera.hpp"


struct VertexSun {
  glm::vec3 position;

  friend bool operator==(VertexSun const &, VertexSun const &) = default;
};

class Sun {
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
  
  std::vector<VertexSun> m_vertices;
  std::vector<GLuint> m_indices;
  
  void loadModelFromFile(std::string_view path);
};

#endif

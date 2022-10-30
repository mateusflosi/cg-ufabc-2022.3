#ifndef BLOCKS_HPP_
#define BLOCKS_HPP_

#include <list>
#include <random>

#include "abcgOpenGL.hpp"

class Blocks {
public:
  void create(GLuint program);
  void paint();
  void destroy();

  struct Block {
    GLuint m_VAO{};
    GLuint m_VBO{};
    GLuint m_EBO{};

    glm::vec4 m_color{};
    float m_rotation{};
    float m_scale{1};
    float m_size{};
    bool m_hit{false};
    int m_nivel{};
    glm::vec2 m_translation{};
    glm::vec2 m_velocity{};
  };

  std::list<Block> m_blocks;

  Block makeBlock(float size, glm::vec4 color, int nivel);

private:
  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  auto static const m_lines{3};
  auto static const m_blocks_per_line{8};

  glm::vec4 const first_line_color{0, 1, 0, 0};
  glm::vec4 const second_line_color{1, 0.65, 0, 0};
  glm::vec4 const third_line_color{1, 0.16, 0, 0};

  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{0.20f, 0.25f};
};

#endif
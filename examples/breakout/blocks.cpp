#include "blocks.hpp"

void Blocks::create(GLuint program) {
  destroy();

  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;

  // Get location of uniforms in the program
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Create asteroids
  m_blocks.clear();
  m_blocks.resize(m_lines * m_blocks_per_line);
  auto &re{m_randomEngine}; // Shortcut

  // auto aux{-1.f};
  auto block = m_blocks.begin();
  for (auto i : iter::range(m_lines)) {
    auto aux{-1.f};
    auto y{0.25f + i * (0.22f)};
    auto color = i == 0   ? first_line_color
                 : i == 1 ? second_line_color
                          : third_line_color;
    for (auto j : iter::range(m_blocks_per_line)) {
      auto size{m_randomDist(re)};

      if (j == m_blocks_per_line - 1)
        size = 0.96 - aux;

      *block = makeBlock(size, color, i + 1);
      block->m_translation = {aux + size / 2 + 0.02f, y};
      aux += size + 0.02f;
      ++block;
    }
  }
}

void Blocks::paint() {
  abcg::glUseProgram(m_program);

  for (auto const &block : m_blocks) {
    abcg::glBindVertexArray(block.m_VAO);

    abcg::glUniform1f(m_scaleLoc, block.m_scale);
    abcg::glUniform1f(m_rotationLoc, block.m_rotation);
    abcg::glUniform2fv(m_translationLoc, 1, &block.m_translation.x);

    abcg::glUniform4fv(m_colorLoc, 1, &block.m_color.r);
    abcg::glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Blocks::destroy() {
  for (auto &block : m_blocks) {
    abcg::glDeleteBuffers(1, &block.m_VBO);
    abcg::glDeleteBuffers(1, &block.m_EBO);
    abcg::glDeleteVertexArrays(1, &block.m_VAO);
  }
}

Blocks::Block Blocks::makeBlock(float size, glm::vec4 color, int nivel) {
  Block block;

  // Reset Player attributes
  block.m_rotation = 0.0f;
  block.m_translation = glm::vec2(0);
  block.m_velocity = glm::vec2(0);
  block.m_color = color;
  block.m_size = size;
  block.m_nivel = nivel;

  // clang-format off
  std::array positions{
      glm::vec2{size/2, +.1f}, glm::vec2{-size/2, +.1f},
      glm::vec2{size/2, -.1f}, glm::vec2{-size/2, -.1f},
      };

  std::array const indices{0, 2, 3,
                           0, 1, 3};

                           // Generate VBO
  abcg::glGenBuffers(1, &block.m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, block.m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &block.m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, block.m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &block.m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(block.m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, block.m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, block.m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return block;
}
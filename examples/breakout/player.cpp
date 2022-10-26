#include "player.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

// TODO: Refatoração no código
void Player::create(GLuint program) {
  destroy();

  m_program = program;

  // Get location of uniforms in the program
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Reset Player attributes
  m_rotation = 0.0f;
  m_translation = glm::vec2(0);
  m_velocity = glm::vec2(0);

  // TODO: desenhar player sem positions ou indices
  // clang-format off
  std::array positions{
      glm::vec2{m_length, +.5f}, glm::vec2{-m_length, +.5f},
      glm::vec2{m_length, -.5f}, glm::vec2{-m_length, -.5f},
      };

  // Normalize
  for (auto &position : positions) {
    position /= glm::vec2{15.5f, 15.5f};
  }

  std::array const indices{0, 2, 3,
                           0, 1, 3};
  // clang-format on                           

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Player::paint(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_VAO);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform1f(m_rotationLoc, m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  // Restart thruster blink timer every 100 ms
  if (m_trailBlinkTimer.elapsed() > 100.0 / 1000.0) m_trailBlinkTimer.restart();

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
  
  m_translation.y = -0.85f;
}

void Player::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Player::update(GameData const &gameData, float deltaTime) {
  // Desloca pra esquerda e pra direita
  m_velocity = {0, 0};
  if (gameData.m_input[gsl::narrow<size_t>(Input::Left)])
    m_velocity = {-.5, 0};
  if (gameData.m_input[gsl::narrow<size_t>(Input::Right)])
    m_velocity = {.5, 0};

  m_translation += m_velocity*deltaTime;

  // parede
    if (m_translation.x < -1.0f + m_scale*(m_length/15.5f))
      m_translation.x = -1.0f + m_scale*(m_length/15.5f);
    if (m_translation.x > +1.0f - m_scale*(m_length/15.5f))
      m_translation.x = 1.0f - m_scale*(m_length/15.5f);
}
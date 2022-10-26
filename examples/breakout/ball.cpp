#include "ball.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

// TODO: Refatoração no código
void Ball::create(GLuint program) {
  destroy();

  m_program = program;

  // Get location of uniforms in the program
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Reset Ball attributes
  m_rotation = 0.0f;
  m_translation = glm::vec2(0);
  m_velocity = {0.5f, 0.5f};

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create geometry data
  auto const sides{10};
  std::vector<glm::vec2> positions{{0, 0}};
  auto const step{M_PI * 2 / sides};
  for (auto const angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO of positions
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  m_translation.y = -0.75f;
}

void Ball::paint(const GameData &gameData) {
  if (gameData.m_state != State::Playing)
    return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_VAO);
  abcg::glUniform4f(m_colorLoc, 1, 1, 1, 1);
  abcg::glUniform1f(m_rotationLoc, 0);
  abcg::glUniform1f(m_scaleLoc, m_scale);

  abcg::glUniform2f(m_translationLoc, m_translation.x, m_translation.y);
  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 12);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Ball::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Ball::update(float deltaTime) {
  m_translation += m_velocity * deltaTime;

  // parede
  if (m_translation.x < -1.0f) {
    m_translation.x = -1.0f;
    m_velocity = {0.5f, m_velocity[1]};
  }
  if (m_translation.x > +1.0f) {
    m_translation.x = 1.0f;
    m_velocity = {-0.5f, m_velocity[1]};
  }
  if (m_translation.y > +1.0f) {
    m_translation.y = 1.0f;
    m_velocity = {m_velocity[0], -0.5f};
  }
}
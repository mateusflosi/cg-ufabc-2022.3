#include "window.hpp"

#include <unordered_map>

// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};

void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_q)
      m_truckSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_e)
      m_truckSpeed = 1.0f;
  }
  if (event.type == SDL_KEYUP) {
    if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_RIGHT ||
         event.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_q && m_truckSpeed < 0)
      m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_e && m_truckSpeed > 0)
      m_truckSpeed = 0.0f;
  }
}

void Window::onCreate() {
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "lookat.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "lookat.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  // create sun
  auto sun = new Planet();
  sun->m_scale = glm::vec3(0.1f);
  sun->m_color = glm::vec4(1.0f, 0.25f, 0.25f, 1.0f);
  sun->create(m_program, "Sun.obj");
  sun->m_velocity = 0.0f;
  m_planets.push_back(*sun);

  // create mercurio
  auto mercurio = new Planet();
  mercurio->m_translate = glm::vec3(2.0f, 0.0f, 2.0f);
  mercurio->m_scale = glm::vec3(0.025f);
  mercurio->m_color = glm::vec4(1.0f, 0.8f, 0.0f, 1.0f);
  mercurio->create(m_program, "Sun.obj");
  mercurio->m_velocity = 1.0f;
  m_planets.push_back(*mercurio);

  // create venus
  auto venus = new Planet();
  venus->m_translate = glm::vec3(4.0f, 0.0f, 4.0f);
  venus->m_scale = glm::vec3(0.04f);
  venus->m_color = glm::vec4(1.0f, 0.8f, 0.0f, 1.0f);
  venus->create(m_program, "Sun.obj");
  venus->m_velocity = 0.9f;
  m_planets.push_back(*venus);

  // create eart
  auto earth = new Planet();
  earth->m_translate = glm::vec3(6.0f, 0.0f, 6.0f);
  earth->m_scale = glm::vec3(0.07f);
  earth->m_color = glm::vec4(0.0f, 0.8f, 1.0f, 1.0f);
  earth->create(m_program, "earth_new.obj");
  earth->m_velocity = 0.8f;
  m_planets.push_back(*earth);

  // create mars
  auto mars = new Planet();
  mars->m_translate = glm::vec3(8.0f, 0.0f, 8.0f);
  mars->m_scale = glm::vec3(0.07f);
  mars->m_color = glm::vec4(1.0f, 0.25f, 0.25f, 1.0f);
  mars->create(m_program, "Sun.obj");
  mars->m_velocity = 0.7f;
  m_planets.push_back(*mars);

  // create jupiter
  auto jupiter = new Planet();
  jupiter->m_translate = glm::vec3(10.0f, 0.0f, 10.0f);
  jupiter->m_scale = glm::vec3(0.1f);
  jupiter->m_color = glm::vec4(1.0f, 0.8f, 0.0f, 1.0f);
  jupiter->create(m_program, "Sun.obj");
  jupiter->m_velocity = 0.6f;
  m_planets.push_back(*jupiter);

  // create saturn
  auto saturn = new Planet();
  saturn->m_translate = glm::vec3(12.0f, 0.0f, 12.0f);
  saturn->m_scale = glm::vec3(0.085f);
  saturn->m_color = glm::vec4(0.0f, 0.8f, 1.0f, 1.0f);
  saturn->create(m_program, "Sun.obj");
  saturn->m_velocity = 0.5f;
  m_planets.push_back(*saturn);

  // create uranus
  auto uranus = new Planet();
  uranus->m_translate = glm::vec3(14.0f, 0.0f, 14.0f);
  uranus->m_scale = glm::vec3(0.055f);
  uranus->m_color = glm::vec4(0.0f, 0.8f, 1.0f, 1.0f);
  uranus->create(m_program, "Sun.obj");
  uranus->m_velocity = 0.4f;
  m_planets.push_back(*uranus);

  // create neptune
  auto neptune = new Planet();
  neptune->m_translate = glm::vec3(16.0f, 0.0f, 16.0f);
  neptune->m_scale = glm::vec3(0.050f);
  neptune->m_color = glm::vec4(0.0f, 0.8f, 1.0f, 1.0f);
  neptune->create(m_program, "Sun.obj");
  neptune->m_velocity = 0.3f;
  m_planets.push_back(*neptune);

  // Get location of uniform variables
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");
}

void Window::onPaint() {
  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  // m_earth.paint(m_program, m_camera);
  for (auto planet : m_planets)
    planet.paint(planet.m_velocity * m_angle_planet);

  abcg::glUseProgram(0);
}

void Window::onPaintUI() { abcg::OpenGLWindow::onPaintUI(); }

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);

  for (auto planet : m_planets)
    planet.destroy();
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};
  m_angle_planet += deltaTime;

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}

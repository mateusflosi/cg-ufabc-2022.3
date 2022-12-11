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
    if (event.key.keysym.sym == SDLK_r)
      m_panXSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_f)
      m_panXSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_q)
      m_truckSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_e)
      m_truckSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_t)
      m_truckXSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_g)
      m_truckXSpeed = 1.0f;
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
    if (event.key.keysym.sym == SDLK_r && m_panXSpeed < 0)
      m_panXSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_f && m_panXSpeed > 0)
      m_panXSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_q && m_truckSpeed < 0)
      m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_e && m_truckSpeed > 0)
      m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_t && m_truckXSpeed < 0)
      m_truckXSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_g && m_truckXSpeed > 0)
      m_truckXSpeed = 0.0f;
  }
}

void Window::onCreate() {
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "blinnphong.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "blinnphong.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  auto const distanceScale = glm::vec3(4.f, 4.f, 4.f);

  // create sun
  auto sun = new Planet();
  sun->m_scale = glm::vec3(0.1f);
  sun->m_color = glm::vec4(1.0f, 0.25f, 0.25f, 1.0f);
  sun->create(m_program, "Sun.obj");
  sun->m_velocity = 0.0f;
  m_planets.push_back(*sun);

  // create mercurio
  auto mercurio = new Planet();
  mercurio->m_translate_scale =
      glm::vec3(0.3867f, 0.0f, 0.3867f) * distanceScale;
  mercurio->m_translate_not_scale = glm::vec3(2.0f, 0.0f, 2.0f);
  mercurio->m_scale = glm::vec3(0.025f);
  mercurio->m_color = glm::vec4(1.0f, 0.8f, 0.0f, 1.0f);
  mercurio->create(m_program, "Sun.obj");
  mercurio->m_velocity = 1.0f;
  m_planets.push_back(*mercurio);

  // create venus
  auto venus = new Planet();
  venus->m_translate_scale = glm::vec3(0.72f, 0.0f, 0.72f) * distanceScale;
  venus->m_translate_not_scale = glm::vec3(4.0f, 0.0f, 4.0f);
  venus->m_scale = glm::vec3(0.04f);
  venus->m_color = glm::vec4(1.0f, 0.8f, 0.0f, 1.0f);
  venus->create(m_program, "Sun.obj");
  venus->m_velocity = 0.39111111f;
  m_planets.push_back(*venus);

  // create eart
  auto earth = new Planet();
  earth->m_translate_scale = glm::vec3(0.9933f, 0.0f, 0.9933f) * distanceScale;
  earth->m_translate_not_scale = glm::vec3(6.0f, 0.0f, 6.0f);
  earth->m_scale = glm::vec3(0.07f);
  earth->m_color = glm::vec4(0.0f, 0.8f, 1.0f, 1.0f);
  earth->create(m_program, "earth_new.obj");
  earth->m_velocity = 0.24109589f;
  m_planets.push_back(*earth);

  // create mars
  auto mars = new Planet();
  mars->m_translate_scale = glm::vec3(1.5133f, 0.0f, 1.5133f) * distanceScale;
  mars->m_translate_not_scale = glm::vec3(8.0f, 0.0f, 8.0f);
  mars->m_scale = glm::vec3(0.07f);
  mars->m_color = glm::vec4(1.0f, 0.25f, 0.25f, 1.0f);
  mars->create(m_program, "Sun.obj");
  mars->m_velocity = 0.12809316f;
  m_planets.push_back(*mars);

  // create jupiter
  auto jupiter = new Planet();
  jupiter->m_translate_scale = glm::vec3(5.18f, 0.0f, 5.18f) * distanceScale;
  jupiter->m_translate_not_scale = glm::vec3(10.0f, 0.0f, 10.0f);
  jupiter->m_scale = glm::vec3(0.1f);
  jupiter->m_color = glm::vec4(1.0f, 0.8f, 0.0f, 1.0f);
  jupiter->create(m_program, "Sun.obj");
  jupiter->m_velocity = 0.02032849f;
  m_planets.push_back(*jupiter);

  // create saturn
  auto saturn = new Planet();
  saturn->m_translate_scale = glm::vec3(9.5067f, 0.0f, 9.5067f) * distanceScale;
  saturn->m_translate_not_scale = glm::vec3(12.0f, 0.0f, 12.0f);
  saturn->m_scale = glm::vec3(0.085f);
  saturn->m_color = glm::vec4(0.0f, 0.8f, 1.0f, 1.0f);
  saturn->create(m_program, "Sun.obj");
  saturn->m_velocity = 0.00818383f;
  m_planets.push_back(*saturn);

  // create uranus
  auto uranus = new Planet();
  uranus->m_translate_scale =
      glm::vec3(19.1267f, 0.0f, 19.1267f) * distanceScale;
  uranus->m_translate_not_scale = glm::vec3(14.0f, 0.0f, 14.0f);
  uranus->m_scale = glm::vec3(0.055f);
  uranus->m_color = glm::vec4(0.0f, 0.8f, 1.0f, 1.0f);
  uranus->create(m_program, "Sun.obj");
  uranus->m_velocity = 0.00286885f;
  m_planets.push_back(*uranus);

  // create neptune
  auto neptune = new Planet();
  neptune->m_translate_scale =
      glm::vec3(29.9733f, 0.0f, 29.9733f) * distanceScale;
  neptune->m_translate_not_scale = glm::vec3(16.0f, 0.0f, 16.0f);
  neptune->m_scale = glm::vec3(0.050f);
  neptune->m_color = glm::vec4(0.0f, 0.8f, 1.0f, 1.0f);
  neptune->create(m_program, "Sun.obj");
  neptune->m_velocity = 0.00143690f;
  m_planets.push_back(*neptune);
}

void Window::onPaint() {
  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  // m_earth.paint(m_program, m_camera);
  for (auto planet : m_planets)
    planet.paint(m_program, m_camera,
                 planet.m_velocity * m_angle_planet * m_planet_velocity,
                 m_planet_distance_scale);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();
  {
    ImGui::SetNextWindowSize(ImVec2(400, 100));
    // Window begin
    ImGui::Begin("Configurações do usuário");
    ImGui::SliderFloat("Velocidade", &m_planet_velocity, 0.0f, 100.0f);
    ImGui::Checkbox("Distância entre planetas em escala",
                    &m_planet_distance_scale);

    ImGui::End();
  }
}

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
  m_camera.truckX(m_truckXSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
  m_camera.panX(m_panXSpeed * deltaTime);
}

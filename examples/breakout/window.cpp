#include "window.hpp"

void Window::onEvent(SDL_Event const &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Right));
  }
}

void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  auto const filename{assetsPath + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError("Cannot load font file");
  }

  // Create program to render the other objects
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "objects.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "objects.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  restart();
}

void Window::restart() {
  m_gameData.m_state = State::Playing;
  m_player.create(m_program);
  m_ball.create(m_program);
  m_blocks.create(m_program);
  m_nivel = 1;
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }

  m_player.update(m_gameData, deltaTime);
  m_ball.update(deltaTime);

  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
  }
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);
  m_player.paint(m_gameData);
  m_ball.paint(m_gameData);
  m_blocks.paint();
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    auto const size{ImVec2(300, 85)};
    auto const position{ImVec2((m_viewportSize.x - size.x) / 2.0f,
                               (m_viewportSize.y - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags const flags{ImGuiWindowFlags_NoBackground |
                                 ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::GameOver) {
      ImGui::Text("Game Over!");
    } else if (m_gameData.m_state == State::Win) {
      ImGui::Text("*You Win!*");
    }

    ImGui::PopFont();
    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);

  m_player.destroy();
  m_ball.destroy();
  m_blocks.destroy();
}

void Window::checkCollisions() {
  // Check collision between ball and player
  if (abs(m_ball.m_translation.y - m_player.m_translation.y) < 0.05f &&
      abs(m_ball.m_translation.x - m_player.m_translation.x) <
          m_player.m_scale * (m_player.m_length / 15.5f)) {
    if (m_ball.m_translation.x > m_player.m_translation.x)
      m_ball.m_velocity = {m_ball.m_velocity_value, m_ball.m_velocity_value};
    else
      m_ball.m_velocity = {-m_ball.m_velocity_value, m_ball.m_velocity_value};
  }
  // Check collision between ball and blocks
  for (auto &block : m_blocks.m_blocks) {
    auto const distanceY = abs(m_ball.m_translation.y - block.m_translation.y);
    auto const distanceX = abs(m_ball.m_translation.x - block.m_translation.x);

    if (distanceY < .12f && distanceX < block.m_size / 2 + .02f) {
      block.m_hit = true;

      if (block.m_nivel > m_nivel) {
        m_ball.m_velocity_value += 0.3;
        m_nivel++;
      }

      float velocityX{m_ball.m_velocity[0] > 0 ? m_ball.m_velocity_value
                                               : -m_ball.m_velocity_value};
      float velocityY(m_ball.m_velocity[1] > 0 ? m_ball.m_velocity_value
                                               : -m_ball.m_velocity_value);

      if (abs(distanceX - block.m_size / 2 - .02f) < abs(distanceY - .12f))
        velocityX *= -1;
      else
        velocityY *= -1;
      m_ball.m_velocity = {velocityX, velocityY};
      m_blocks.m_blocks.remove_if([](auto const &a) { return a.m_hit; });
      checkWinCondition();
      break;
    }
  }

  // Check lose condition
  if (m_ball.m_translation.y < -1.0f) {
    m_gameData.m_state = State::GameOver;
    m_restartWaitTimer.restart();
  }
}

void Window::checkWinCondition() {
  if (m_blocks.m_blocks.empty()) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
  }
}
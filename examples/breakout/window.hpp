#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"

#include "ball.hpp"
#include "blocks.hpp"
#include "player.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_program{};

  GameData m_gameData;

  abcg::Timer m_restartWaitTimer;

  Ball m_ball;
  Blocks m_blocks;
  Player m_player;

  void restart();
  void checkCollisions();
};

#endif
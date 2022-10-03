#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaintUI() override;

private:
  static int const m_N{8}; // Board size is m_N x m_N

  enum class GameState { Bomb, Flag, Win, Lose };
  GameState m_gameState;

  bool m_FlagSelected{false};
  /*
   * B - o quadrado tem uma boma
   * F - o quadrado está flegado
   * [0...9] - indica quantas bombas estão ao redor do quadrado
   */
  std::array<char, m_N * m_N> m_board{};

  // void checkWinCondition();
  void restartGame();
};

#endif
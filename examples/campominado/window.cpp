#include "window.hpp"
#include <cstdlib>
#include <iostream>
#include <time.h>

void Window::onCreate() { restartGame(); }

void Window::onPaintUI() {
  // Get size of application's window
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};

  // "Campo Minado" window
  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto const flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse};
    ImGui::Begin("Campo Minado", nullptr, flags);

    // Menu
    {
      bool restartSelected{};
      if (ImGui::BeginMenuBar()) {
        // TODO: Adicionar tamanhos de jogo: 8x8, 16x16, 30x16
        if (ImGui::BeginMenu("Game")) {
          ImGui::MenuItem("Restart", nullptr, &restartSelected);
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }
      if (restartSelected) {
        restartGame();
      }
    }

    // Static text showing current turn or win/draw messages
    {
      std::string text;
      switch (m_gameState) {
      case GameState::Bomb:
        text = "Faça sua jogada";
        break;
      case GameState::Flag:
        text = "Faça sua jogada";
        break;
      case GameState::Win:
        text = "Você venceu";
        break;
      case GameState::Lose:
        text = "Você perdeu";
        break;
      }
      // Center text
      ImGui::SetCursorPosX(
          (appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) / 2);
      ImGui::Text("%s", text.c_str());
      ImGui::Spacing();
    }

    // TODO: Botão para controlar se é bomba ou flag

    ImGui::Spacing();

    // Game board
    {
      auto const gridHeight{appWindowHeight - 22 - 60 - (m_N * 10) - 60};
      auto const buttonHeight{gridHeight / m_N};

      if (ImGui::BeginTable("Game board", m_N)) {
        for (auto i : iter::range(m_N)) {
          ImGui::TableNextRow();
          for (auto j : iter::range(m_N)) {
            ImGui::TableSetColumnIndex(j);
            auto const offset{i * m_N + j};

            // Get current character
            auto ch{m_board.at(offset)};

            // Replace null character with whitespace because the button label
            // cannot be an empty string
            if (ch == 0 || (ch == 'B' && m_gameState != GameState::Lose)) {
              ch = ' ';
            }

            // Button text is ch followed by an ID in the format ##ij
            auto buttonText{fmt::format("{}##{}{}", ch, i, j)};
            if (ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight))) {
              if (m_gameState == GameState::Bomb && ch == ' ') {
                if (m_board.at(offset) == 'B') {
                  m_gameState = GameState::Lose;
                } else {
                  clickButton(i, j);
                  checkWinCondition();
                }
              }
            }
          }
          ImGui::Spacing();
        }
        ImGui::EndTable();
      }
    }

    ImGui::Spacing();

    // "Restart game" button
    {
      if (ImGui::Button("Restart game", ImVec2(-1, 50))) {
        restartGame();
      }
    }

    ImGui::End();
  }
}

void Window::checkWinCondition() {
  for (auto i : iter::range(m_N)) {
    for (auto j : iter::range(m_N)) {
      auto const offset{i * m_N + j};
      auto ch{m_board.at(offset)};
      if (ch == 0) {
        return;
      }
    }
  }
  m_gameState = GameState::Win;
}

void Window::restartGame() {
  m_board.fill('\0');

  srand(time(0));

  for (auto i : iter::range(m_N)) {
    auto offset{-1};
    do {
      offset = rand() % m_N * m_N + rand() % m_N;
    } while (m_board.at(offset) == 'B');
    m_board.at(offset) = 'B';
  }

  m_gameState = GameState::Bomb;
}

void Window::clickButton(int i, int j) {
  int offset{i * m_N + j};
  auto const numberBombs = getNumberBombsInPerimeter(i, j);
  m_board.at(offset) = fmt::format("{}", numberBombs)[0];

  if (numberBombs == 0) {
    for (int a = i - 1; a <= i + 1; a++) {
      for (int b = j - 1; b <= j + 1; b++) {
        offset = a * m_N + b;
        if (a >= 0 && a < m_N && b >= 0 && b < m_N && m_board.at(offset) == 0) {
          clickButton(a, b);
        }
      }
    }
  }
}

int Window::getNumberBombsInPerimeter(int i, int j) {
  int count = 0;
  for (int a = i - 1; a <= i + 1; a++) {
    for (int b = j - 1; b <= j + 1; b++) {
      if (a >= 0 && a < m_N && b >= 0 && b < m_N) {
        auto const offset{a * m_N + b};
        auto ch{m_board.at(offset)};
        if (ch == 'B') {
          count++;
        }
      }
    }
  }
  return count;
}
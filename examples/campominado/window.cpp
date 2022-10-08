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
      ImGui::PushStyleColor(ImGuiCol_Button, m_gameState == GameState::Flag
                                                 ? button_hovered_default
                                                 : button_color_default);
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            m_gameState == GameState::Flag
                                ? button_color_default
                                : button_hovered_default);
      ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                            m_gameState == GameState::Flag
                                ? button_color_default
                                : button_hovered_default);

      ImGui::SetCursorPosX((appWindowWidth - 102) / 2);
      ImGui::Text(fmt::format("{}", m_bombs).c_str());
      ImGui::SameLine();
      if (ImGui::Button("Flag", ImVec2(100, 25))) {
        if (m_gameState == GameState::Flag) {
          m_gameState = GameState::Bomb;
        } else if (m_gameState == GameState::Bomb) {
          m_gameState = GameState::Flag;
        }
      }

      ImGui::PopStyleColor(3);

      ImGui::Spacing();
    }

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

            unsigned int button_collor;
            unsigned int button_hovered;

            switch (ch) {
            case '0':
              button_collor = button_zero;
              button_hovered = button_zero;
              break;
            case '1':
              button_collor = button_one;
              button_hovered = button_one;
              break;
            case '2':
              button_collor = button_two;
              button_hovered = button_two;
              break;
            case '3':
              button_collor = button_three;
              button_hovered = button_three;
              break;
            case '4':
              button_collor = button_four;
              button_hovered = button_four;
              break;
            case '5':
              button_collor = button_five;
              button_hovered = button_five;
              break;
            case 'f':
              if (m_gameState == GameState::Lose) {
                button_collor = button_erro;
                button_hovered = button_erro;
              } else {
                button_collor = button_flag;
                button_hovered = button_flag;
              }
              break;
            case 'F':
              button_collor = button_flag;
              button_hovered = button_flag;
              break;
            case 'B':
              if (m_gameState == GameState::Lose) {
                button_collor = button_erro;
                button_hovered = button_erro;
                break;
              }
            default:
              button_collor = button_color_default;
              button_hovered = button_hovered_default;
              break;
            }

            // Replace null character with whitespace because the button label
            // cannot be an empty string
            if (ch == 0 || ch == '0' ||
                (ch == 'B' && m_gameState != GameState::Lose)) {
              ch = ' ';
            }

            if (ch == 'f') {
              ch = 'F';
            }

            ImGui::PushID(offset);
            ImGui::PushStyleColor(ImGuiCol_Text, button_text);
            ImGui::PushStyleColor(ImGuiCol_Button, button_collor);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hovered);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_hovered);

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

              if (m_gameState == GameState::Flag &&
                  (m_board.at(offset) == 'B' || ch == 'F' ||
                   m_board.at(offset) == '\0')) {
                if (ch == 'F') {
                  m_board.at(offset) = m_board.at(offset) == 'F' ? 'B' : '\0';
                  m_bombs++;
                } else if (m_bombs > 0) {
                  m_board.at(offset) = m_board.at(offset) == 'B' ? 'F' : 'f';
                  m_bombs--;
                }
              }
            }

            ImGui::PopStyleColor(4);
            ImGui::PopID();
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

    // Show another simple window
    if (m_gameState == GameState::Win || m_gameState == GameState::Lose) {
      ImGui::SetNextWindowSize(ImVec2(appWindowWidth, -1));
      ImGui::SetNextWindowPos(ImVec2(0, appWindowHeight / 2));
      ImGui::Begin(m_gameState == GameState::Win ? "Você venceu"
                                                 : "Você perdeu",
                   nullptr);
      if (ImGui::Button("Play Again", ImVec2(-1, 25))) {
        restartGame();
      }
      ImGui::End();
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
  m_bombs = 8;
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
        if (ch == 'B' || ch == 'F') {
          count++;
        }
      }
    }
  }
  return count;
}
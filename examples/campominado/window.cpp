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

    // Revela o número de bombas que ainda tem pra identifcar e o botão
    // para adicionar as bandeiras no tabuleiro
    {
      // Adição de cor ao botão para saber quando o botão está ou não ativo
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

      // adiciona o número de bombas que ainda não foram identificadas na tela
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

            // verifica qual a cor do botão
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
              // se o quadrado está flegado, mas não é uma bomba
              // caso o jogador perca a cor do quadrado é alterada para erro
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
              // caso o jogo não esteja em estado de derrota a cor da bomba fica
              // em default
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

            // troca de caracteres para não exibir o valor
            if (ch == 0 || ch == '0' ||
                (ch == 'B' && m_gameState != GameState::Lose)) {
              ch = ' ';
            }

            // troca de f para F
            if (ch == 'f') {
              ch = 'F';
            }

            // adição de cor no botão
            ImGui::PushID(offset);
            ImGui::PushStyleColor(ImGuiCol_Text, button_text);
            ImGui::PushStyleColor(ImGuiCol_Button, button_collor);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hovered);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_hovered);

            // Button text is ch followed by an ID in the format ##ij
            auto buttonText{fmt::format("{}##{}{}", ch, i, j)};
            if (ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight))) {
              // verifica o estado e se o quadrado está vazio
              if (m_gameState == GameState::Bomb && ch == ' ') {
                // se B perde o jogo, senão executa a ação de revelar o número
                // de bombas e verifica se o jogador ganhou
                if (m_board.at(offset) == 'B') {
                  m_gameState = GameState::Lose;
                } else {
                  clickButton(i, j);
                  checkWinCondition();
                }
              }

              // verifica se é para flegar o quadrado e o que está preenchido no
              // mesmo
              if (m_gameState == GameState::Flag &&
                  (m_board.at(offset) == 'B' || ch == 'F' ||
                   m_board.at(offset) == '\0')) {
                // se o quadrado está flegado troca para B ou '\0'
                // se não, verifica o número de bombas faltantes e flega
                if (ch == 'F') {
                  // se F a casa tinha sido flegada corretamente
                  m_board.at(offset) = m_board.at(offset) == 'F' ? 'B' : '\0';
                  m_bombs++;
                } else if (m_bombs > 0) {
                  // verifica se o jogador identificou corretamente a bomba
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

    // Mostra se o jogador ganhou ou perdeu o jogo
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

  // seleciona aleatoriamente as casas que vão ser bombas
  for (auto i : iter::range(m_N)) {
    auto offset{-1};
    // gera casas aleatórias enquanto não encontra uma casa vazia
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

  // se o número de bombas for 0 revela os quadrados ao redos
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

// verifica o número de bombas que tem ao redos do quadrado
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
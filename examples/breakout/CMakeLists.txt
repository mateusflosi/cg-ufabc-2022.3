project(breakout)
add_executable(${PROJECT_NAME} main.cpp window.cpp player.cpp ball.cpp blocks.cpp)
enable_abcg(${PROJECT_NAME})
project(breakout)
add_executable(${PROJECT_NAME} main.cpp window.cpp player.cpp ball.cpp)
enable_abcg(${PROJECT_NAME})
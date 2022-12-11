project(SystemSolar)
add_executable(${PROJECT_NAME} camera.cpp planet.cpp main.cpp window.cpp trackball.cpp)
enable_abcg(${PROJECT_NAME})

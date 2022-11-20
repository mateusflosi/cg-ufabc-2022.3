project(SystemSolar)
add_executable(${PROJECT_NAME} camera.cpp neptune.cpp uranus.cpp saturn.cpp jupiter.cpp mars.cpp venus.cpp mercury.cpp earth.cpp sun.cpp ground.cpp main.cpp window.cpp)
enable_abcg(${PROJECT_NAME})

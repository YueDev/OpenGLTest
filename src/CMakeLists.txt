cmake_minimum_required(VERSION 3.19)
project(OpenGLTest)

set(CMAKE_CXX_STANDARD 17)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)
add_executable(OpenGLTest src/main.cpp src/glad.c src/Shader.cpp src/Shader.h)

target_link_libraries(OpenGLTest ${PROJECT_SOURCE_DIR}/lib/glfw3.dll )
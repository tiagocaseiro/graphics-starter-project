cmake_minimum_required(VERSION 3.31)

project(starter-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_directories(libs)

include_directories(src src/GLFW)

add_executable(starter-project src/main.cpp src/Window.cpp src/Logger.cpp)

set_target_properties(starter-project PROPERTIES LINKER_LANGUAGE CXX)

target_compile_features(starter-project PUBLIC cxx_std_20)

find_package(OpenGL REQUIRED)

target_link_libraries(starter-project glfw3)
target_link_libraries(starter-project OpenGL::GL)

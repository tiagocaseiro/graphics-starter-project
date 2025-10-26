cmake_minimum_required(VERSION 3.21)

project(starter-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_directories(libs)

include_directories(src src/GLFW)


file(GLOB SOURCE_FILES_PATH src/opengl/*.cpp src/tools/*.cpp src/*.cpp src/imgui/*.cpp)

add_executable(starter-project ${SOURCE_FILES_PATH})

set_target_properties(starter-project PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(starter-project PUBLIC src/glm)

target_compile_features(starter-project PUBLIC cxx_std_20)

find_package(OpenGL REQUIRED)

target_link_libraries(starter-project glfw3)
target_link_libraries(starter-project OpenGL::GL)

add_subdirectory(shaders)
add_subdirectory(textures)

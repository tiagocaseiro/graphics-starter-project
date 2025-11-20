cmake_minimum_required(VERSION 3.21)

project(starter-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

file(GLOB SOURCE_FILES_PATH src/tinygltf/tiny_gltf.cc src/opengl/*.cpp src/tools/*.cpp src/model/*.cpp src/*.cpp src/imgui/*.cpp)

add_executable(starter-project ${SOURCE_FILES_PATH}) 

set_target_properties(starter-project PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(starter-project PUBLIC src PUBLIC src/glm PUBLIC src/glfw/include)

find_package(OpenGL REQUIRED)

target_link_libraries(starter-project OpenGL::GL)
target_link_libraries(starter-project src/glfw/src/Debug/glfw3)

add_subdirectory(assets)
add_subdirectory(shaders)
add_subdirectory(textures)

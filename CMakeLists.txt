cmake_minimum_required(VERSION 3.21)

project(starter-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")
file(GLOB SOURCE_FILES_PATH src/opengl/*.cpp src/tools/*.cpp src/model/*.cpp src/*.cpp src/imgui/*.cpp)

add_executable(starter-project ${SOURCE_FILES_PATH}) 

set_target_properties(starter-project PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(starter-project PRIVATE ${TINYGLTF_INCLUDE_DIRS})
target_include_directories(starter-project PUBLIC src)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(starter-project OpenGL::GL)
target_link_libraries(starter-project glfw)
target_link_libraries(starter-project glm::glm)

add_custom_command(
        TARGET starter-project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/assets
                ${CMAKE_CURRENT_BINARY_DIR}/assets)

add_custom_command(
        TARGET starter-project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/shaders
                ${CMAKE_CURRENT_BINARY_DIR}/shaders)
  
add_custom_command(
        TARGET starter-project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/textures
                ${CMAKE_CURRENT_BINARY_DIR}/textures)

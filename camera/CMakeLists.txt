# Top level CMakeLists.txt

cmake_minimum_required(VERSION 3.18)

project(Camera VERSION 1.0)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/glad)

configure_file(Config.h.in Config.h)

add_executable(camera
    ${SRC_DIR}/Application.cpp
    ${SRC_DIR}/Shader.cpp
    ${SRC_DIR}/Camera.cpp
    ${GLAD_DIR}/src/glad.c)

target_include_directories(camera PUBLIC ${GLAD_DIR}/include
    ${PROJECT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# GLFW
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/glfw)
set(GLFW_USE_WAYLAND ON)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate internal target")
add_subdirectory(${GLFW_DIR})
target_link_libraries(camera glfw ${GLFW_LIBRARIES})
target_include_directories(camera PUBLIC ${GLFW_DIR}/include)
target_compile_definitions(camera PRIVATE GLFW_INCLUDE_NONE)

# GLM
set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/glm-0.9.9.8)
add_subdirectory(${GLM_DIR})
target_include_directories(camera PUBLIC ${GLM_DIR})

# Testing
enable_testing()

add_test(NAME Run COMMAND camera)

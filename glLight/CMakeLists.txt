# Top level CMakeLists.txt for the glLight project.

cmake_minimum_required(VERSION 3.18)

project(glLight VERSION 1.0)

# C,C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(DEP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep)

# source files to generate the executable
set(SRC ${SRC_DIR}/Application.cpp
        ${SRC_DIR}/Shader.cpp
        ${SRC_DIR}/Camera.cpp
        ${SRC_DIR}/glad.c)

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# GLFW
set(GLFW_DIR ${DEP_DIR}/glfw)
set(GLFW_USE_WAYLAND ON)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory(${GLFW_DIR})
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_DIR}/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

# GLM
set(GLM_DIR ${DEP_DIR}/glm)
add_subdirectory(${GLM_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_DIR})

##########
# Testing
##########
enable_testing()

# Run Test
add_test(NAME Run COMMAND ${PROJECT_NAME})
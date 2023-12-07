# Top level CMakeLists.txt

cmake_minimum_required(VERSION 3.18)

project(Lights VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/glad)

# ImGui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/imgui)
set(IMGUI_INCLUDE ${IMGUI_DIR})
set(IMGUI_BACKENDS ${IMGUI_DIR}/backends)
set(IMGUI_SRC ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_BACKENDS}/imgui_impl_glfw.cpp
    ${IMGUI_BACKENDS}/imgui_impl_opengl3.cpp)

add_executable(lights
    ${SRC_DIR}/Application.cpp
    ${SRC_DIR}/Camera.cpp
    ${SRC_DIR}/Shader.cpp
    ${GLAD_DIR}/src/glad.c
    ${IMGUI_SRC})

target_include_directories(lights PUBLIC ${GLAD_DIR}/include ${IMGUI_INCLUDE} ${IMGUI_BACKENDS})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

#GLFW
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/glfw)
set(GLFW_USE_WAYLAND ON)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate internal target")
add_subdirectory(${GLFW_DIR})
target_link_libraries(lights glfw ${GLFW_LIBRARIES})
target_include_directories(lights PUBLIC ${GLFW_DIR}/include)
target_compile_definitions(lights PRIVATE GLFW_INCLUDE_NONE)

# GLM
set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/glm-0.9.9.8)
add_subdirectory(${GLM_DIR})
target_include_directories(lights PUBLIC ${GLM_DIR})

# Testing
enable_testing()

add_test(NAME Run COMMAND lights)
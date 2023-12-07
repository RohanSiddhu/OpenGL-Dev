/**
 * @file Application.hpp
 * @author Rohan Siddhu
 * @brief Header file for application.cpp
 * @version 0.1
 * @date 2022-06-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "main.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"



float cubeData[] = {
    // Coords               // Normals              // Texture Coords
    -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f
};


void error_callback(int error, const char* message);
void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mod);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

GLuint load_texture(const char* path);

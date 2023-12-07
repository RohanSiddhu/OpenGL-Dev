/**
 * @file Application.hpp
 * @author Rohan Siddhu
 * @brief This is the header file for Application.cpp
 * @version 0.1
 * @date 2022-04-05
 */

#pragma once

#include <iostream>
#include <cstdlib>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Shader.hpp"
#include "Camera.hpp"


/* Global data */
const int WIDTH = 800;
const int HEIGHT = 600;

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


/* function declaration */
void processInput(GLFWwindow* window);
void error_callback(int errorCode, const char* description);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void key_callback(GLFWwindow* window, int key, int code, int action, int mod);
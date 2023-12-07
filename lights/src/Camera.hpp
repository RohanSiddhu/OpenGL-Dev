/**
 * @file Camera.h
 * @author Rohan Siddhu
 * @brief Camera Class
 * @version 0.1
 * @date 2022-04-13
 */

#pragma once

#define CUSTOM_LOOK_AT_MATRIX

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Controll Commands
enum class Command {
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT,
    UP,
    DOWN,
    RESET
};

// Default Values of Camera Attributes
constexpr float DEFAULT_YAW = -90.0f;
constexpr float DEFAULT_PITCH = -15.0f;
constexpr float DEFAULT_MOVEMENT_SPEED = 2.0f;
constexpr float DEFAULT_MOUSE_SENSITIVITY = 0.1f;
constexpr float DEFAULT_FOV = 45.0f;

class Camera {
public:
    // Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Euler angles
    float yaw;
    float pitch;

    // Controll options
    float movementSpeed;
    float mouseSensitivity;
    float fov;
public:
    // Constructor
    Camera(glm::vec3 position = glm::vec3(0.0f, 1.0f, 3.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = DEFAULT_YAW,
        float pitch = DEFAULT_PITCH) :
        // Initializer list
        front(glm::vec3(0.0f, 0.0f, -1.0f)),
        worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
        movementSpeed(DEFAULT_MOVEMENT_SPEED),
        mouseSensitivity(DEFAULT_MOUSE_SENSITIVITY),
        fov(DEFAULT_FOV)
    {
        this->position = position;
        this->up = up;
        this->yaw = yaw;
        this->pitch = pitch;
        updateVectors();
    }

    // Methods
    glm::mat4 getViewMatrix();
    void keyInput(Command command, float deltaTime = 1.0f);
    void mouseInput(float xOffset, float yOffset);
    void zoom(float yOffset);
private:
    void updateVectors();
};

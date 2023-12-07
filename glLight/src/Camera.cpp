/**
 * @file Camera.cpp
 * @author Rohan Siddhu
 * @brief Camera class definition.
 * @version 0.1
 * @date 2022-04-13
 */

#include "Camera.hpp"


extern float deltaTime;


glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::keyInput(Command command) {
    float speed = movementSpeed * deltaTime;
    if (command == Command::FORWARD) {
        position += speed * front;
    }
    if (command == Command::BACKWARD) {
        position -= speed * front;
    }
    if (command == Command::LEFT) {
        position -= speed * right;
    }
    if (command == Command::RIGHT) {
        position += speed * right;
    }
    if (command == Command::UP) {
        position += speed * up;
    }
    if (command == Command::DOWN) {
        position -= speed * up;
    }
    if (command == Command::RESET) {
        this->position = glm::vec3(0.0f, 1.0f, 3.0f);
        this->up = glm::vec3(0.0f, 1.0f, 0.0f);
        this->right = glm::normalize(glm::cross(this->front, this->up));
        this->front = glm::vec3(0.0f, 0.0f, -1.0f);
        this->yaw = CameraDefault::YAW;
        this->pitch = CameraDefault::PITCH;
        updateVectors();
    }
}

void Camera::mouseInput(float xOffset, float yOffset) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

    updateVectors();
}

void Camera::zoom(float yOffset) {
    fov -= (float)yOffset;

	if (fov < 1.0f) {
		fov = 1.0f;
	}
	if (fov > 45.0f) {
		fov = 45.0f;
	}
}


/*
* Private Methods
*/

void Camera::updateVectors() {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(this->front, worldUp));
    up = glm::normalize(glm::cross(right, this->front));
}
/**
 * @file Camera.cpp
 * @author Rohan Siddhu
 * @brief Camera class definition.
 * @version 0.1
 * @date 2022-04-13
 */

#include "Camera.hpp"


glm::mat4 Camera::getViewMatrix() {
#ifdef CUSTOM_LOOK_AT_MATRIX
    glm::vec3 zAxis = glm::normalize(-front);
    glm::vec3 xAxis = glm::normalize(glm::cross(glm::normalize(worldUp), zAxis));
    glm::vec3 yAxis = glm::normalize(glm::cross(zAxis, xAxis));
    
    glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
    translation[3][0] = -position.x; // Third column, first row
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = xAxis.x; // First column, first row
    rotation[1][0] = xAxis.y;
    rotation[2][0] = xAxis.z;
    rotation[0][1] = yAxis.x; // First column, second row
    rotation[1][1] = yAxis.y;
    rotation[2][1] = yAxis.z;
    rotation[0][2] = zAxis.x; // First column, third row
    rotation[1][2] = zAxis.y;
    rotation[2][2] = zAxis.z;

    return rotation * translation;
#else 
    return glm::lookAt(position, position + front, up);
#endif
}

void Camera::keyInput(Command command, float deltaTime) {
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
        this->yaw = DEFAULT_YAW;
        this->pitch = DEFAULT_PITCH;
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

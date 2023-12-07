/**
 * @file Shader.cpp
 * @author Rohan Siddhu
 * @brief This file defines the Shader class.
 * @version 0.1
 * @date 2022-04-05
 */

#include "Shader.hpp"


void Shader::addShader(GLenum type, const char* path) {
    const char* src;
    std::string line, source;
    std::ifstream file(path);

    while (std::getline(file, line)) {
        source += line + '\n';
    }

    src = source.c_str();
    file.close();

    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        GLsizei log_length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
        GLchar* message = new GLchar[log_length];
        glGetShaderInfoLog(id, log_length, &log_length, message);

        std::cout << "Failed to compile " << ((type == GL_VERTEX_SHADER) ? "Vertex Shader" : "Fragment Shader") << std::endl;
        std::cout << message << std::endl;

        delete[] message;
        return;
    }

    glAttachShader(program, id);
    glDeleteShader(id);
}

void Shader::createProgram() {
    glLinkProgram(program);


    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        GLsizei log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        GLchar* message = new GLchar[log_length];
        glGetProgramInfoLog(program, log_length, &log_length, message);

        std::cout << "Failed to link program" << std::endl;
        std::cout << message << std::endl;

        delete[] message;
    }
}

void Shader::setMat4(const char* name, const GLfloat* value) {
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, value);
}

void Shader::setVec3(const char* name, const float v0, const float v1, const float v2) {
    glUniform3f(glGetUniformLocation(program, name), v0, v1, v2);
}

void Shader::setVec3(const char* name, glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(program, name), 1, glm::value_ptr(value));
}

void Shader::clean() {
    glDeleteProgram(program);
}
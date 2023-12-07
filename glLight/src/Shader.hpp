/**
 * @file Shader.hpp
 * @author Rohan Siddhu
 * @brief This is declares the Shader class.
 * @version 0.1
 * @date 2022-04-05
 */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {
private:
    GLuint program;
public:
    Shader() {
        program = glCreateProgram();
    }

    void addShader(GLenum type, const char* path);
    void createProgram();
    void use() { glUseProgram(program); }
    GLuint id() { return program; }

    void setMat4(const char* name, const GLfloat* value);
    void setVec3(const char* name, const float x, const float y, const float z);
    void setVec3(const char* name, glm::vec3 value);

    void clean();
};
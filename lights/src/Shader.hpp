/**
 * @file Shader.hpp
 * @author Rohan Siddhu
 * @brief Header file for Shader class
 * @version 0.1
 * @date 15/08/2022
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
    void setVec3(const char* name, const GLfloat x, const GLfloat y, const GLfloat z);
    void setVec3(const char* name, glm::vec3 value);
    void setFloat(const char* name, const GLfloat value);
    void setInt(const char* name, const GLint value);

    void clean();
};

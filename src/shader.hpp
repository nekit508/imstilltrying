#pragma once

#include "vec4.hpp"
#include "GLFW/glfw3.h"
#include "GL/glew.h"
#include <fstream>
#include <iostream>
#include <gtc/type_ptr.hpp>
#include "shader.hpp"
#include "camera.hpp"

class Shader {
public:
    GLuint program;

    Shader(char *vertFile, char *fragFile) {
        GLint vertSize, fragSize;
        char *vertData = readFile(vertFile, &vertSize), *fragData = readFile(fragFile, &fragSize);
        GLuint vert = compileShader(&vertData, &vertSize, GL_VERTEX_SHADER);
        GLuint frag = compileShader(&fragData, &fragSize, GL_FRAGMENT_SHADER);

        if (vert == 0 || frag == 0) {
            return;
        }

        program = compileProgram(vert, frag);

        glDeleteShader(vert);
        glDeleteShader(frag);
    }

    void apply(Camera *camera) {
        glUniformMatrix4fv(glGetUniformLocation(program, "u_proj"), 1, GL_FALSE, glm::value_ptr(camera->proj));
        glUniformMatrix4fv(glGetUniformLocation(program, "u_view"), 1, GL_FALSE, glm::value_ptr(camera->view));
        glUniform3fv(glGetUniformLocation(program, "u_playerPos"), 1, glm::value_ptr(camera->position));
        glUniform3fv(glGetUniformLocation(program, "u_light.ambientColor"), 1, glm::value_ptr(glm::vec3(0.1f, 0.1f, 0.1f)));
        glUniform3fv(glGetUniformLocation(program, "u_light.sunColor"), 1, glm::value_ptr(glm::vec3(0.9f, 0.8f, 0.7f)));
        glm::vec3 sv = glm::vec3(1, 1, 0.0f);
        glUniform3fv(glGetUniformLocation(program, "u_light.sunVector"), 1, glm::value_ptr(glm::normalize(sv)));
    }

    void bind() {
        glUseProgram(program);
    }

    static GLuint compileProgram(GLuint vert, GLuint frag) {
        GLuint ptr = glCreateProgram();

        glAttachShader(ptr, vert);
        glAttachShader(ptr, frag);

        glLinkProgram(ptr);

        GLint success;
        char infoLog[512];
        glGetProgramiv(ptr, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(ptr, 512, nullptr, infoLog);
            std::cout << "Program compilation failed\n" << std::endl << infoLog << std::endl;
            return 0;
        }

        return ptr;
    }

    static GLuint compileShader(char **data, GLint *sizes, GLuint type) {
        GLuint ptr = glCreateShader(type);

        glShaderSource(ptr, 1, data, sizes);
        glCompileShader(ptr);

        GLint success;
        char infoLog[512];
        glGetShaderiv(ptr, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(ptr, 512, nullptr, infoLog);
            std::cout << "Shader compilation failed " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl << infoLog << std::endl;
            return 0;
        }

        return ptr;
    }

    static char* readFile(char *file, GLint *l) {
        std::ifstream stream(file, std::ios::binary | std::ios::ate);

        auto length = stream.tellg();
        char* ptr = new char[length];

        stream.seekg(0, std::ios::beg);
        stream.read(ptr, length);

        stream.close();

        *l = length;
        return ptr;
    }
};
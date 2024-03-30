#pragma once

#include "shader.hpp"
#include "camera.hpp"

class Mesh {
public:
    Shader *shader;

    GLuint vbo, vao, ebo;
    GLsizei vertexesCount;

    Mesh(GLfloat *vertices, GLsizei verticesLength, GLuint *indices, GLsizei indicesLength, Shader *shader) {
        vertexesCount = indicesLength;
        this->shader = shader;

        glGenBuffers(1, &vbo);
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, verticesLength * sizeof(GLfloat), (GLvoid*) vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLength * sizeof(GLuint), (GLvoid*) indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    void render(Camera *camera) {
        shader->bind();
        shader->apply(camera);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, vertexesCount, GL_UNSIGNED_INT, (GLvoid*) 0);
        glBindVertexArray(0);
    }

    ~Mesh() {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &ebo);
    }
};

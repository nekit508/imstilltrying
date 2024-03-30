#pragma once

#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "vars.hpp"

class Camera {
public:
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 direction = glm::vec3(1, 0, 0);
    glm::vec3 right = glm::vec3(0, 1, 0);

    glm::vec3 up = glm::vec3(0, 1, 0);

    glm::mat4x4 proj = glm::mat4x4(), view = glm::mat4x4();

    float fov, near, far, pitch = 0, yaw = 0;

    Camera(float fov, float near, float far) {
        this->fov = fov;
        this->near = near;
        this->far = far;
    }

    void update() {
        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        proj = glm::perspective(fov, vars::windowRatio, near, far);

        direction = glm::vec3(
                std::cos(glm::radians(pitch)) * std::cos(glm::radians(yaw)),
                std::sin(glm::radians(pitch)),
                std::cos(glm::radians(pitch)) * std::sin(glm::radians(yaw))
                );

        view = glm::lookAt(position, position + direction, up);

        right = glm::normalize(glm::cross(direction, up));
    }
};
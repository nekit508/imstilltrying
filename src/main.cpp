#include <iostream>
#include <fstream>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "vec4.hpp"
#include "shader.hpp"
#include "chunks.hpp"

GLbitfield clearBits = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
glm::vec4 clearColor = glm::vec4(0.0, 0.0, 0.0, 1.0);

Camera *camera;
GLFWwindow* window;

bool polygonMode = true, mouseMode = true;

void init() {
    vars::events::keys::callbacks.push_back([](int key, int scancode, int action, int mods){
        if (action == 1) {
            switch (key) {
                case GLFW_KEY_TAB:
                    polygonMode = !polygonMode;
                    glPolygonMode(GL_FRONT_AND_BACK, polygonMode ? GL_FILL : GL_LINE);
                    break;
                case GLFW_KEY_ESCAPE:
                    mouseMode = !mouseMode;
                    glfwSetInputMode(window, GLFW_CURSOR, mouseMode ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
                    break;
                case GLFW_KEY_F1:
                    chunk::disposeChunks = !chunk::disposeChunks;
                    break;
            }
        }
    });
}

void load() {
    auto *shader = new Shader("generic-mesh.vert", "generic-mesh.frag");
    chunk::shader = shader;

    camera = new Camera(glm::radians(80.0f), 0.1f, 1000000.0);
    camera->position.x = 8;
    camera->position.y = 8;
    camera->position.z = 8;
}

void update() {
    if (mouseMode) {
        camera->pitch += -vars::events::mouse::dy * 100;
        camera->yaw += vars::events::mouse::dx * 100;
    }

    float cf = 0, cs = 0, boost = 1;
    if (vars::events::keys::keys[GLFW_KEY_W])
        cf += 1;
    if (vars::events::keys::keys[GLFW_KEY_S])
        cf -= 1;
    if (vars::events::keys::keys[GLFW_KEY_A])
        cs -= 1;
    if (vars::events::keys::keys[GLFW_KEY_D])
        cs += 1;

    if (vars::events::keys::keys[GLFW_KEY_LEFT_CONTROL])
        boost /= 4;
    if (vars::events::keys::keys[GLFW_KEY_LEFT_SHIFT])
        boost *= 4;

    camera->position += camera->direction * cf * boost;
    camera->position += camera->right * cs * boost;

    camera->update();

    chunk::update();
}

void render() {
    chunk::render();
}

void exit() {
    glfwTerminate();
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
    window = glfwCreateWindow(400, 300, "imstilltrying", nullptr, nullptr);
    glfwSwapInterval(1);

    int w, h;
    glfwGetWindowSize(window, &w, &h);
    vars::windowWidth = (float) w;
    vars::windowHeight = (float) h;
    vars::windowRatio = vars::windowWidth  / vars::windowHeight;

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        exit();
        return -1;
    }
    glfwMakeContextCurrent(window);

    std::cout << glGetString(GL_VERSION) << std::endl;

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << glGetError() << std::endl;
        exit();
        return -1;
    } else {
        glGetError();
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam){
        if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        std::cout << "---------------" << std::endl;
        std::cout << "Debug message (" << id << "): " <<  message << std::endl;

        switch (source)
        {
            case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
            case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
        } std::cout << std::endl;

        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
            case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
        } std::cout << std::endl;

        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
            case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
            case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
        } std::cout << std::endl;
        std::cout << std::endl;
    }, window);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

    // setup input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height){
        vars::windowHeight = (float) width;
        vars::windowWidth = (float) height;
        vars::windowRatio = (float) width / (float) height;
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double px, double py){
        static float prevX = 0.5, prevY = 0.5;

        float lx = ((float) px / vars::windowWidth);
        float ly = ((float) py / vars::windowHeight);

        float dx = lx - prevX;
        float dy = ly - prevY;

        prevX = lx;
        prevY = ly;

        vars::events::mouse::dx = dx;
        vars::events::mouse::dy = dy;

        for(auto callback: vars::events::mouse::callbacks)
            callback((float) px, (float) py, lx, ly, dx, dy);
    });
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
        vars::events::keys::keys[key] = (bool) action;

        for(auto callback: vars::events::keys::callbacks)
            callback(key, scancode, action, mods);
    });

    init();
    load();

    while(true) {
        if (glfwWindowShouldClose(window)) {
            exit();
            break;
        }

        vars::events::mouse::dx = 0;
        vars::events::mouse::dy = 0;
        glfwPollEvents();

        update();

        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(clearBits);
        render();
        glfwSwapBuffers(window);
    }

    return 0;
}
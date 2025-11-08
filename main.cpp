#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"

// global state
int winW = 800, winH = 600;
float lastX = winW / 2.0f, lastY = winH / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f, lastFrame = 0.0f;
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));

// input callbacks
void framebuffer_size_callback(GLFWwindow* w, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* w, double xpos, double ypos) {
    if (firstMouse) { lastX = (float)xpos; lastY = (float)ypos; firstMouse = false; }
    float xoff = (float)xpos - lastX;
    float yoff = lastY - (float)ypos;
    lastX = (float)xpos; lastY = (float)ypos;
    camera.lookDelta(xoff, yoff);
}

void processInput(GLFWwindow* w) {
    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(w, true);
    if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) camera.moveWSAD(0, deltaTime);
    if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) camera.moveWSAD(1, deltaTime);
    if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) camera.moveWSAD(2, deltaTime);
    if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) camera.moveWSAD(3, deltaTime);
}

// --------------------------------------
int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    // GLFW 3.2 core context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(winW, winH, "Labyrinth Escape", nullptr, nullptr);
    if (!window) {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLAD first
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD init failed\n";
        return -1;
    }

    // Now safe to call GL
    int fbw = 0, fbh = 0;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    Shader shader("basic.vert", "basic.frag");
    Mesh cube = Mesh::Cube();
    Mesh ground = Mesh::Plane();

    bool wire = false;
    double timerAcc = 0.0; int frames = 0;

    while (!glfwWindowShouldClose(window)) {
        float current = (float)glfwGetTime();
        deltaTime = current - lastFrame;
        lastFrame = current;
        processInput(window);

        // FPS counter
        timerAcc += deltaTime; frames++;
        if (timerAcc >= 1.0) {
            std::string title = "Labyrinth Escape | FPS: " + std::to_string(frames);
            glfwSetWindowTitle(window, title.c_str());
            frames = 0; timerAcc = 0.0;
        }

        // Toggle wireframe
        if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
            wire = !wire;
        glPolygonMode(GL_FRONT_AND_BACK, wire ? GL_LINE : GL_FILL);

        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 view = camera.getView();
        glm::mat4 proj = glm::perspective(glm::radians(camera.fov), (float)winW / (float)winH, 0.1f, 100.0f);
        shader.setMat4("uView", view);
        shader.setMat4("uProj", proj);

        // draw cube
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0, 0.5f, 0));
        shader.setMat4("uModel", model);
        shader.setVec3("uColor", glm::vec3(0.8f, 0.2f, 0.2f));
        cube.draw();

        // draw ground
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 0, 0));
        model = glm::scale(model, glm::vec3(12.0f, 1.0f, 12.0f));
        shader.setMat4("uModel", model);
        shader.setVec3("uColor", glm::vec3(0.2f, 0.35f, 0.4f));
        ground.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

//
// Created by MTBLE on 24.01.2022.
//

#include "BasicRenderer.h"

BasicRenderer::BasicRenderer() {
    if (!glfwInit()) {
        throw std::exception();
    }

    glfwWindowHint(GLFW_SAMPLES, 16);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL


    m_Window = glfwCreateWindow(1024, 768, "tutor", NULL, NULL);

    if (!m_Window) {
        glfwTerminate();
        throw std::exception();
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error glew init";
        glfwTerminate();
        throw std::exception();
    };

    glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glEnable(GL_MULTISAMPLE);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

BasicRenderer::~BasicRenderer() {
    glfwTerminate();
}

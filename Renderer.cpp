//
// Created by MTBLE on 21.01.2022.
//

#include "Renderer.h"

void GLClearError() {
    while (glGetError() != GL_NO_ERROR) { ;
    }
}

void GLPrintError() {
    while (GLenum err = glGetError()) {
        std::cout << "OpenGL error (" << err << ")" << std::endl;
    }
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) {
    shader.Bind();
    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

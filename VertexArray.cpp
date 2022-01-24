//
// Created by MTBLE on 21.01.2022.
//

#include "VertexArray.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_RendererID);
    
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const {
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    GLuint offset = 0;
    for (GLuint i = 0; i != elements.size(); ++i) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
                i,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                element.count,                  // size
                element.type,           // type
                (element.normalized ? GL_TRUE : GL_FALSE),           // normalized?
                layout.GetStride(),                  // stride
                (const void*) offset            // array buffer offset
        );
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

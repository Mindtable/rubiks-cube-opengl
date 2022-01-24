//
// Created by MTBLE on 21.01.2022.
//

#include "VertexBufferLayout.h"

template<>
void VertexBufferLayout::Push<GLfloat>(GLuint count) {
    m_Elements.push_back({GL_FLOAT, count, false});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<>
void VertexBufferLayout::Push<GLuint>(GLuint count) {
    m_Elements.push_back({GL_UNSIGNED_INT, count, false});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::Push<GLubyte>(GLuint count) {
    m_Elements.push_back({GL_UNSIGNED_BYTE, count, true});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}

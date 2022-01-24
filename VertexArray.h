//
// Created by MTBLE on 21.01.2022.
//

#include "dependences.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#ifndef GLUT_TEST_VERTEXARRAY_H
#define GLUT_TEST_VERTEXARRAY_H


class VertexArray {
private:
    GLuint m_RendererID;

public:
    VertexArray();
    ~VertexArray();

    void Bind() const;

    void Unbind() const;

    void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
};


#endif //GLUT_TEST_VERTEXARRAY_H

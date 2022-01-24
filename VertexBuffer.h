//
// Created by MTBLE on 21.01.2022.
//

#include "dependences.h"

#ifndef GLUT_TEST_VERTEXBUFFER_H
#define GLUT_TEST_VERTEXBUFFER_H


class VertexBuffer {
private:
    GLuint m_RendererID;
public:
    VertexBuffer(const void* data, GLuint size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
};


#endif //GLUT_TEST_VERTEXBUFFER_H

//
// Created by MTBLE on 21.01.2022.
//
#include "dependences.h"

#ifndef GLUT_TEST_INDEXBUFFER_H
#define GLUT_TEST_INDEXBUFFER_H


class IndexBuffer {
private:
    GLuint m_RendererID;
    GLuint m_Count;
public:
    IndexBuffer();
    IndexBuffer(const GLuint *data, GLuint count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline GLuint GetCount() const {
        return m_Count;
    }
};


#endif //GLUT_TEST_INDEXBUFFER_H

//
// Created by MTBLE on 21.01.2022.
//

#include "dependences.h"

#ifndef GLUT_TEST_VERTEXBUFFERLAYOUT_H
#define GLUT_TEST_VERTEXBUFFERLAYOUT_H

struct VertexBufferElement {
    GLuint type;
    GLuint count;
    bool normalized;

    static GLuint GetSizeOfType(GLuint type) {
        switch (type) {
        case GL_FLOAT: return sizeof(GLfloat);
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        }
        return 0;
    }
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> m_Elements;
    GLuint m_Stride;

public:
    VertexBufferLayout() : m_Stride(0) {
        ;
    }

    template<typename T>
    void Push(GLuint count);

    const std::vector<VertexBufferElement>& GetElements() const {
        return m_Elements;
    }

    inline GLuint GetStride() const {
        return m_Stride;
    }
};


#endif //GLUT_TEST_VERTEXBUFFERLAYOUT_H

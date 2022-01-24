//
// Created by MTBLE on 21.01.2022.
//
#include "dependences.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#ifndef GLUT_TEST_RENDERER_H
#define GLUT_TEST_RENDERER_H


class Renderer {
public:
    static void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) ;
    static void Clear();
};

void GLClearError();
void GLPrintError();

#endif //GLUT_TEST_RENDERER_H

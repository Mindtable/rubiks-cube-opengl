//
// Created by MTBLE on 24.01.2022.
//

#include "dependences.h"

#ifndef GLUT_TEST_BASICRENDERER_H
#define GLUT_TEST_BASICRENDERER_H


class BasicRenderer {
protected:
    GLFWwindow *m_Window;
public:
    BasicRenderer();

    virtual ~BasicRenderer();
};


#endif //GLUT_TEST_BASICRENDERER_H

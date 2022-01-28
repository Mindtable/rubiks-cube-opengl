//
// Created by MTBLE on 22.01.2022.
//

#include "Cube.h"
#include "../src/BasicOpenglObj/dependences.h"
#include "../src/BasicOpenglObj/IndexBuffer.h"
#include "dependences.h"
#include "../src/BasicOpenglObj/Shader.h"
#include "../src/BasicOpenglObj/VertexArray.h"
//#include "../Renderer.h"
#include "../src/BasicOpenglObj/Texture.h"

#ifndef GLUT_TEST_VISUALCUBE_H
#define GLUT_TEST_VISUALCUBE_H

class VisualCube : public Cube {
public:
    VisualCube();

    void GetColor(std::vector<Color>& colors, int x, int y, int z);
};


#endif //GLUT_TEST_VISUALCUBE_H

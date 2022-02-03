#include <algorithm>
#include <cmath>
#include <list>
#include <set>
#include "src/BasicOpenglObj/dependences.h"

#include "Renderer.h"
#include "src/BasicOpenglObj/VertexBuffer.h"
#include "src/BasicOpenglObj/IndexBuffer.h"
#include "src/BasicOpenglObj/VertexArray.h"
#include "src/BasicOpenglObj/Shader.h"
#include "src/BasicOpenglObj/Texture.h"
#include "VisualCube/VisualCube.h"

int main() {
    Renderer RubiksCube("res\\vertex.glsl",
                        "res\\fragment.glsl");

    std::cout << "8 9 0\n"
                 "I O P --- CUBE MOVES (SHIFT - 2, CTRL - ')\n"
                 "K L ; '\n"
                 "\n"
                 "Q --- COOL ANIMATION\n"
                 "R/W --- READ/WRITE FROM/TO FILE\n"
                 "A --- CHECK IF CUBE IS CORRECT\n"
                 "Z --- SHUFFLE\n"
                 "X --- RESET\n"
                 "S --- SOLVE\n"
                 "ENTER -- DISABLE/ENABLE ANIMATION" << std::endl;


    RubiksCube.Show();
}

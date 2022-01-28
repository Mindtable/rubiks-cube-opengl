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
    RubiksCube.Show();
}

//
// Created by MTBLE on 21.01.2022.
//
#include "src/BasicOpenglObj/dependences.h"
#include "VisualCube/dependences.h"
#include "VisualCube/VisualCube.h"
#include "src/BasicOpenglObj/IndexBuffer.h"
#include "src/BasicOpenglObj/VertexArray.h"
#include "src/BasicOpenglObj/Shader.h"
#include "src/BasicOpenglObj/Texture.h"
#include "src/BasicOpenglObj/BasicRenderer.h"
#include "VisualCube/Solver.h"

#include "Controller.h"

#include <thread>

#ifndef GLUT_TEST_RENDERER_H
#define GLUT_TEST_RENDERER_H

enum class State {
    NotWorking,
    InProcess,
    ReadyToDrop
};


#ifndef FIXME_PLS_ROTATION
#define FIXME_PLS_ROTATION
struct Rotation {
    GLfloat deltaSign;
    glm::vec3 axis;
    glm::vec3 point;
    std::set<int> CubesToRotateX;
    std::set<int> CubesToRotateY;
    std::set<int> CubesToRotateZ;
    std::string Move;
};

#endif

class Renderer : BasicRenderer {
private:
    //-----------Indices-----------
    const GLfloat g_RubiksCubeVertices[192] = {
            //DLF
            //--COORDINATES--//----UV----//------NORMALS-----
            -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, //Down      0
            -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, //Left      1
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, //Front     2

            //DFR
            1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, //Down       3
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//Front      4
            1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //Right      5

            //DRB
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,//Down      6
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,//Right     7
            1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, //Back      8

            //DBL
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,//Down     9
            -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,//Back     10
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,//Left     11

            //UFL
            -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, //Up         12
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,//Front      13
            -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,//Left       14

            //URF
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//Up          15
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,//Right       16
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,//Front       17

            //UBR
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,//Up         18
            1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,//Back       19
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,//Right      20

            //ULB
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,//Up        21
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,//Left      22
            -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f//Back       23
    };

    const GLuint g_FrontIndices[6] = {
            2, 4, 17,
            2, 17, 13
    };
    const GLuint g_RightIndices[6] = {
            5, 7, 20,
            5, 20, 16
    };
    const GLuint g_LeftIndices[6] = {
            11, 1, 14,
            11, 14, 22
    };
    const GLuint g_BackIndices[6] = {
            8, 10, 23,
            8, 23, 19
    };
    const GLuint g_UpIndices[6] = {
            12, 15, 18,
            12, 18, 21
    };
    const GLuint g_DownIndices[6] = {
            9, 6, 3,
            9, 3, 0
    };

    const glm::vec3 m_CubeLocations[3][3][3]{
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -2.0f),
            glm::vec3(0.0f, 0.0f, -4.0f),
            glm::vec3(0.0f, 2.0f, 0.0f),
            glm::vec3(0.0f, 2.0f, -2.0f),
            glm::vec3(0.0f, 2.0f, -4.0f),
            glm::vec3(0.0f, 4.0f, 0.0f),
            glm::vec3(0.0f, 4.0f, -2.0f),
            glm::vec3(0.0f, 4.0f, -4.0f),

            glm::vec3(2.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 0.0f, -2.0f),
            glm::vec3(2.0f, 0.0f, -4.0f),
            glm::vec3(2.0f, 2.0f, 0.0f),
            glm::vec3(2.0f, 2.0f, -2.0f),
            glm::vec3(2.0f, 2.0f, -4.0f),
            glm::vec3(2.0f, 4.0f, 0.0f),
            glm::vec3(2.0f, 4.0f, -2.0f),
            glm::vec3(2.0f, 4.0f, -4.0f),

            glm::vec3(4.0f, 0.0f, 0.0f),
            glm::vec3(4.0f, 0.0f, -2.0f),
            glm::vec3(4.0f, 0.0f, -4.0f),
            glm::vec3(4.0f, 2.0f, 0.0f),
            glm::vec3(4.0f, 2.0f, -2.0f),
            glm::vec3(4.0f, 2.0f, -4.0f),
            glm::vec3(4.0f, 4.0f, 0.0f),
            glm::vec3(4.0f, 4.0f, -2.0f),
            glm::vec3(4.0f, 4.0f, -4.0f)
    };

    //-----------Opengl Objects-----------
    VertexArray m_VAO;
    VertexBuffer m_VBO;
    std::vector<IndexBuffer> m_SideIB;
    Shader m_Shader;
    std::vector<Texture> m_Color;

    ////-----------MVP Block-----------
    const glm::vec3 m_XAxis = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 m_YAis = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 m_ZAxis = glm::vec3(0.0f, 0.0f, 1.0f);

    Controller m_Controller;
    ////Логика Кубика Рубика
    VisualCube m_VisualCube;

    ////Лишний блок
    std::list<Rotation> m_RotationOrder;
    GLfloat m_Angle;
//    GLfloat m_Delta = 0.05f; //FIXME
    GLfloat m_Delta = 0.15f;
    GLfloat m_MaxAngle = 3.14f / 2.0f;

public:
    Renderer(const std::string &vertexShader, const std::string &fragmentShader);
    ~Renderer();
    void Show();
private:
    void DrawCubes();
    void DrawCube(int x, int y, int z);
    static void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader);
    void InitTextures();
    void InitIndexBuffers();

    static void Clear();
    void Swap();

    glm::mat4 myrotate(const glm::vec3 &axis, const glm::vec3 &point, GLfloat angle);
};

void GLClearError();
void GLPrintError();

#endif //GLUT_TEST_RENDERER_H

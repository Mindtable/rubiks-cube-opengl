//
// Created by MTBLE on 22.01.2022.
//

#include "Cube.h"
#include "../dependences.h"
#include "../IndexBuffer.h"
#include "dependences.h"
#include "../Shader.h"
#include "../VertexArray.h"
#include "../Renderer.h"
#include "../Texture.h"

#ifndef GLUT_TEST_VISUALCUBE_H
#define GLUT_TEST_VISUALCUBE_H

struct Rotation {
    GLfloat deltaSign;
    glm::vec3 axis;
    glm::vec3 point;
    glm::vec3 cubesToRotate;
};


class VisualCube : public Cube {
public:
     GLfloat g_RubiksCubeVertices[120] = {
            //DLF
            -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, //Down      0
            -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, //Left      1
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, //Front     2

            //DFR
            1.0f, -1.0f, 1.0f, 1.0f, 1.0f, //Down       3
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, //Front      4
            1.0f, -1.0f, 1.0f, 0.0f, 0.0f, //Right      5

            //DRB
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, //Down      6
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, //Right     7
            1.0f, -1.0f, -1.0f, 0.0f, 0.0f, //Back      8

            //DBL
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, //Down     9
            -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, //Back     10
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, //Left     11

            //UFL
            -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //Up         12
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //Front      13
            -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //Left       14

            //URF
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //Up          15
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //Right       16
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //Front       17

            //UBR
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f, //Up         18
            1.0f, 1.0f, -1.0f, 0.0f, 1.0f, //Back       19
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f, //Right      20

            //ULB
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, //Up        21
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, //Left      22
            -1.0f, 1.0f, -1.0f, 1.0f, 1.0f //Back       23
    };

    GLuint g_FrontIndices[6] = {
            2, 4, 17,
            2, 17, 13
    };
    GLuint g_RightIndices[6] = {
            5, 7, 20,
            5, 20, 16
    };
    GLuint g_LeftIndices[6] = {
            11, 1, 14,
            11, 14, 22
    };
    GLuint g_BackIndices[6] = {
            8, 10, 23,
            8, 23, 19
    };
    GLuint g_UpIndices[6] = {
            12, 15, 18,
            12, 18, 21
    };
    GLuint g_DownIndices[6] = {
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

    std::vector<IndexBuffer> m_SideIB;
    Shader m_Shader;
    std::vector<Texture> m_Color;
    VertexArray m_VAO;
    VertexBuffer m_VBO;
    Renderer m_Renderer;

    std::list<Rotation> m_RotationOrder;
    GLfloat m_Angle;

    glm::vec3 position = glm::vec3(14.8f, 7.76f, 5.4f);
    // Initial horizontal angle : toward -Z
    float m_HorizontalAngle = -2.14f;
    // Initial vertical angle : none
    float m_VerticalAngle = -0.37f;

    float m_MovingSpeed = 3.0f; // 3 units / second
    float m_MouseSpeed = 0.005f;

    const glm::mat4 m_ProjectionMatrix = glm::perspective(
            glm::radians(45.0f),
            4.0f / 3.0f, 0.1f, 100.0f);

    glm::vec3 m_Direction = glm::vec3(
            cos(m_VerticalAngle) * sin(m_HorizontalAngle),
            sin(m_VerticalAngle),
            cos(m_VerticalAngle) * cos(m_HorizontalAngle)
    );

    // Right vector
    glm::vec3 m_Right = glm::vec3(
            sin(m_HorizontalAngle - 3.14f / 2.0f),
            0,
            cos(m_HorizontalAngle - 3.14f / 2.0f)
    );

    // Up vector
    glm::vec3 up = glm::cross(m_Right, m_Direction);

    glm::mat4 viewMatrix = glm::lookAt(
            position,           // Camera is here
            position + m_Direction, // and looks here : at the same position, plus "m_Direction"
            up                  // Head is up (set to 0,-1,0 to look upside-down)
    );
public:
    VisualCube(const std::string &vertexShader, const std::string &fragmentShader);

    void DrawCubes();

    void DrawCube(int x, int y, int z);

    void DrawSide(Side side, Color color);

    void InitTextures();
    void InitIndexBuffers();
    void Controller();
    glm::mat4 myrotate(const glm::vec3 &axis, const glm::vec3 &point, GLfloat angle);
};


#endif //GLUT_TEST_VISUALCUBE_H

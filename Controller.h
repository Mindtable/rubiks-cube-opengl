//
// Created by MTBLE on 21.01.2022.
//

#include "src/BasicOpenglObj/dependences.h"
#include "VisualCube/dependences.h"
#include "src/BasicOpenglObj/Shader.h"
#include "VisualCube/VisualCube.h"

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

#ifndef GLUT_TEST_CONTROLLER_H
#define GLUT_TEST_CONTROLLER_H

enum class FileStatus {
    Read = -1,
    DoNothing = 0,
    Write = 1,
};

enum class CubeStatus {
    Correct,
    Incorrect,
    DontAsked
};

class Controller {
private:
    std::vector<bool> m_KeyPressed;

    glm::vec3 m_Position = glm::vec3(11.0123f,  6.59808f,  7.06434f);

    float m_HorizontalAngle = -2.35999f;
    float m_VerticalAngle = -0.38f;

    float m_MovingSpeed = 6.0f; // 3 units / second
    float m_MouseSpeed = 0.005f;

    /// Camera block
    const glm::mat4 m_ProjectionMatrix = glm::perspective(
            glm::radians(45.0f),
            4.0f / 3.0f, 0.1f, 100.0f);

    glm::vec3 m_Direction = glm::vec3(
            glm::cos(m_VerticalAngle) * glm::sin(m_HorizontalAngle),
            glm::sin(m_VerticalAngle),
            glm::cos(m_VerticalAngle) * glm::cos(m_HorizontalAngle)
    );

    glm::vec3 m_Right = glm::vec3(
            glm::sin(m_HorizontalAngle - 3.14f / 2.0f),
            0,
            glm::cos(m_HorizontalAngle - 3.14f / 2.0f)
    );

    glm::vec3 m_Up = glm::cross(m_Right, m_Direction);

    glm::mat4 m_ViewMatrix = glm::lookAt(
            m_Position,
            m_Position + m_Direction,
            m_Up
    );

    /// View Projection Matrix
    glm::mat4 VP;
    bool moving = false;

public:
    explicit Controller(bool moving);

    void ComputeProjectionView(Shader &shader, GLFWwindow *window);

    void ComputeRotations(std::list<Rotation> &RotationOrder, GLFWwindow *window);

    FileStatus FileWorkingCheck(GLFWwindow *window);

    CubeStatus CheckCorrect(GLFWwindow *window, VisualCube &cube);
};

#endif //GLUT_TEST_CONTROLLER_H

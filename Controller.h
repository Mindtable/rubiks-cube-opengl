//
// Created by MTBLE on 21.01.2022.
//

#include "src/BasicOpenglObj/dependences.h"
#include "VisualCube/dependences.h"
#include "src/BasicOpenglObj/Shader.h"
#include "VisualCube/VisualCube.h"

#ifndef FIXME_PLS_ROTATION
#define FIXME_PLS_ROTATION

enum class Axis {
    X, Y, Z
};

struct Rotation {
    GLfloat deltaSign;
    glm::vec3 axis{};
    glm::vec3 point{};
    std::set<int> CubesToRotateX;
    std::set<int> CubesToRotateY;
    std::set<int> CubesToRotateZ;
    std::string Move;

    Rotation() {
        deltaSign = 1.0f;
        axis = glm::vec3(1.0f);
        point = glm::vec3(1.0f);
    }

    Rotation(GLfloat deltaSign, Axis axis, const std::string &Move, std::initializer_list<int> CubesToRotate) {
        this->deltaSign = deltaSign;
        if (axis == Axis::X) {
            this->axis = glm::vec3(1.0f, 0.0f, 0.0f);
            this->point = glm::vec3(4.0f, 2.0f, -2.0f);

            for (const auto &elem: CubesToRotate) {
                this->CubesToRotateX.insert(elem);
            }
        } else if (axis == Axis::Y) {
            this->axis = glm::vec3(0.0f, 1.0f, 0.0f);
            this->point = glm::vec3(2.0f, 4.0f, -2.0f);

            for (const auto &elem: CubesToRotate) {
                this->CubesToRotateY.insert(elem);
            }
        } else if (axis == Axis::Z) {
            this->axis = glm::vec3(0.0f, 0.0f, 1.0f);
            this->point = glm::vec3(2.0f, 2.0f, 0.0f);

            for (const auto &elem: CubesToRotate) {
                this->CubesToRotateZ.insert(elem);
            }
        }

        this->Move = Move;
    }
};

#endif

#ifndef GLUT_TEST_CONTROLLER_H
#define GLUT_TEST_CONTROLLER_H

enum class FileStatus {
    Read = -1,
    DoNothing = 0,
    Write = 1,
};

enum class Status {
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

    std::map<std::string, Rotation> m_RotationMapping = {
            {"z2", Rotation(1.0f, Axis::Z, "z2", {0, 1, 2})},
            {"z'", Rotation(1.0f, Axis::Z, "z'", {0, 1, 2})},
            {"z",  Rotation(-1.0f, Axis::Z, "z", {0, 1, 2})},

            {"y2", Rotation(1.0f, Axis::Y, "y2", {0, 1, 2})},
            {"y'", Rotation(1.0f, Axis::Y, "y'", {0, 1, 2})},
            {"y",  Rotation(-1.0f, Axis::Y, "y", {0, 1, 2})},

            {"x2", Rotation(1.0f, Axis::X, "x2", {0, 1, 2})},
            {"x'", Rotation(1.0f, Axis::X, "x'", {0, 1, 2})},
            {"x",  Rotation(-1.0f, Axis::X, "x", {0, 1, 2})},

            {"U2", Rotation(1.0f, Axis::Y, "U2", {2})},
            {"U'", Rotation(1.0f, Axis::Y, "U'", {2})},
            {"U",  Rotation(-1.0f, Axis::Y, "U", {2})},

            {"D2", Rotation(1.0f, Axis::Y, "D2", {0})},
            {"D'", Rotation(-1.0f, Axis::Y, "D'",{0})},
            {"D",  Rotation(1.0f, Axis::Y, "D",  {0})},

            {"F2", Rotation(1.0f, Axis::Z, "F2", {0})},
            {"F'", Rotation(1.0f, Axis::Z, "F'", {0})},
            {"F",  Rotation(-1.0f, Axis::Z, "F", {0})},

            {"B2", Rotation(1.0f, Axis::Z, "B2", {2})},
            {"B'", Rotation(-1.0f, Axis::Z, "B'",{2})},
            {"B",  Rotation(1.0f, Axis::Z, "B",  {2})},

            {"R2", Rotation(1.0f, Axis::X, "R2", {2})},
            {"R'", Rotation(1.0f, Axis::X, "R'", {2})},
            {"R",  Rotation(-1.0f, Axis::X, "R", {2})},

            {"L2", Rotation(1.0f, Axis::X, "L2", {0})},
            {"L'", Rotation(-1.0f, Axis::X, "L'",{0})},
            {"L",  Rotation(1.0f, Axis::X, "L",  {0})},

            {"M2", Rotation(1.0f, Axis::X, "M2", {1})},
            {"M'", Rotation(-1.0f, Axis::X, "M'",{1})},
            {"M",  Rotation(1.0f, Axis::X, "M",  {1})}
    };

public:
    explicit Controller(bool moving);

    void ComputeProjectionView(Shader &shader, GLFWwindow *window);

    void ComputeRotations(std::list<Rotation> &RotationOrder, GLFWwindow *window);
    void ComputeRotations(std::list<Rotation> &RotationOrder, GLFWwindow *window, const std::string &solution);

    void FileWorkingCheck(GLFWwindow *window, FileStatus &file_status);
    void Shuffle(GLFWwindow *window, VisualCube &cube, std::list<Rotation> &RotationOrder);
    void GetAnimationSkipStatus(GLFWwindow *window, Status &AnimationSkip);
    void CheckCorrect(GLFWwindow *window, VisualCube &cube, Status &CubeStatus);
};

#endif //GLUT_TEST_CONTROLLER_H

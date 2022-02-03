//
// Created by MTBLE on 21.01.2022.
//

#include "Controller.h"

Controller::Controller(bool moving) : moving(moving) {
    m_KeyPressed.reserve(360);
    for (int i = 0; i != 360; ++i) {
        m_KeyPressed[i] = false;
    }
}

void Controller::ComputeProjectionView(Shader &shader, GLFWwindow *window) {
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position


    // Compute new orientation
    if (moving) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Reset mouse position for next frame
        glfwSetCursorPos(window, 1024 / 2, 768 / 2);

        m_HorizontalAngle += m_MouseSpeed * float(1024 / 2 - xpos);
        m_VerticalAngle += m_MouseSpeed * float(768 / 2 - ypos);
    }

    if (m_VerticalAngle > 1.48f) {
        m_VerticalAngle = 1.48f;
    }
    if (m_VerticalAngle < -1.48f) {
        m_VerticalAngle = -1.48f;
    }


    if (fabs(m_HorizontalAngle) > 3.14f * 2.0f) {
        if (m_HorizontalAngle > 0) {
            m_HorizontalAngle -= 3.14f * 2.0f;
        } else {
            m_HorizontalAngle += 3.14f * 2.0f;
        }
    }


    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
            cos(m_VerticalAngle) * sin(m_HorizontalAngle),
            sin(m_VerticalAngle),
            cos(m_VerticalAngle) * cos(m_HorizontalAngle)
    );

    // Right vector
    glm::vec3 right = glm::vec3(
            sin(m_HorizontalAngle - 3.14f / 2.0f),
            0,
            cos(m_HorizontalAngle - 3.14f / 2.0f)
    );

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    if (moving) {
        // Move forward
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            m_Position += direction * deltaTime * m_MovingSpeed;
        }
        // Move backward
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            m_Position -= direction * deltaTime * m_MovingSpeed;
        }
        // Strafe m_Right
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            m_Position += right * deltaTime * m_MovingSpeed;
        }
        // Strafe left
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            m_Position -= right * deltaTime * m_MovingSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
            glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            m_Position -= up * deltaTime * m_MovingSpeed;
        } else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            m_Position += up * deltaTime * m_MovingSpeed;
        }
    }

    // Camera matrix
    m_ViewMatrix = glm::lookAt(
            m_Position,           // Camera is here
            m_Position + direction, // and looks here : at the same position, plus "m_Direction"
            up                  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;

    VP = m_ProjectionMatrix * m_ViewMatrix;

    shader.SetUniformMatrix4fv("u_ProjectionViewMatrix", VP);
}

void Controller::ComputeRotations(std::list<Rotation> &RotationOrder, GLFWwindow *window) {
    //// Z rotation
    GLint key = GLFW_KEY_0;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
        glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["z2"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping[ "z'"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["z"]);

        m_KeyPressed[key] = true;
    }

    if (glfwGetKey(window, key) == GLFW_RELEASE && m_KeyPressed[key]) {

        m_KeyPressed[key] = false;
    }
    //// End z rotation

    //// Y rotation
    key = GLFW_KEY_9;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
        glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["y2"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        RotationOrder.push_back(m_RotationMapping[ "y'"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {


        RotationOrder.push_back(m_RotationMapping["y"]);

        m_KeyPressed[key] = true;
    }

    if (glfwGetKey(window, key) == GLFW_RELEASE && m_KeyPressed[key]) {

        m_KeyPressed[key] = false;
    }
    ////End y rotation

    ////X rotation
    key = GLFW_KEY_8;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
        glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        RotationOrder.push_back(m_RotationMapping["x2"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        RotationOrder.push_back(m_RotationMapping["x'"]);

        m_KeyPressed[key] = true;

    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["x"]);

        m_KeyPressed[key] = true;
    }

    if (glfwGetKey(window, key) == GLFW_RELEASE && m_KeyPressed[key]) {
        m_KeyPressed[key] = false;
    }
    ////End y rotation

    //// U rotation
    key = GLFW_KEY_I;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
        glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["U2"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["U'"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["U"]);

        m_KeyPressed[key] = true;
    }

    if (glfwGetKey(window, key) == GLFW_RELEASE && m_KeyPressed[key]) {

        m_KeyPressed[key] = false;
    }
    ////End U

    ////D rotation
    key = GLFW_KEY_K;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
        glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["D2"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["D'"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        RotationOrder.push_back(m_RotationMapping["D"]);

        m_KeyPressed[key] = true;
    }

    if (glfwGetKey(window, key) == GLFW_RELEASE && m_KeyPressed[key]) {

        m_KeyPressed[key] = false;
    }
    //// D end

    /// F rotation
    key = GLFW_KEY_O;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
        glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["F2"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["F'"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["F"]);

        m_KeyPressed[key] = true;
    }

    if (glfwGetKey(window, key) == GLFW_RELEASE && m_KeyPressed[key]) {

        m_KeyPressed[key] = false;
    }

    /// F end

    ///B start
    key = GLFW_KEY_L;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
        glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["B2"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["B'"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        RotationOrder.push_back(m_RotationMapping["B"]);

        m_KeyPressed[key] = true;
    }

    if (glfwGetKey(window, key) == GLFW_RELEASE && m_KeyPressed[key]) {

        m_KeyPressed[key] = false;
    }
    //// В end

    //// R start
    key = GLFW_KEY_P;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
        glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        RotationOrder.push_back(m_RotationMapping["R2"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        RotationOrder.push_back(m_RotationMapping["R'"]);

        m_KeyPressed[key] = true;

    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["R"]);

        m_KeyPressed[key] = true;
    }

    if (glfwGetKey(window, key) == GLFW_RELEASE && m_KeyPressed[key]) {
        m_KeyPressed[key] = false;
    }
    ////R end

    //// L start
    key = GLFW_KEY_SEMICOLON;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
        glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        RotationOrder.push_back(m_RotationMapping["L2"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {


        RotationOrder.push_back(m_RotationMapping["L'"]);

        m_KeyPressed[key] = true;

    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        RotationOrder.push_back(m_RotationMapping["L"]);

        m_KeyPressed[key] = true;
    }

    if (glfwGetKey(window, key) == GLFW_RELEASE && m_KeyPressed[key]) {
        m_KeyPressed[key] = false;
    }
    ////L end

    //// M start
    key = GLFW_KEY_APOSTROPHE;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
        glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        RotationOrder.push_back(m_RotationMapping["M2"]);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {


        RotationOrder.push_back(m_RotationMapping["M'"]);

        m_KeyPressed[key] = true;

    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        RotationOrder.push_back(m_RotationMapping["M"]);

        m_KeyPressed[key] = true;
    }

    if (glfwGetKey(window, key) == GLFW_RELEASE && m_KeyPressed[key]) {
        m_KeyPressed[key] = false;
    }
    ////M end
}

void Controller::FileWorkingCheck(GLFWwindow *window, FileStatus &file_status) {
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE && m_KeyPressed[GLFW_KEY_R]) {
        m_KeyPressed[GLFW_KEY_R] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && m_KeyPressed[GLFW_KEY_W]) {
        m_KeyPressed[GLFW_KEY_W] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !m_KeyPressed[GLFW_KEY_R]) {
        m_KeyPressed[GLFW_KEY_R] = true;
        file_status = FileStatus::Read;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !m_KeyPressed[GLFW_KEY_W]) {
        m_KeyPressed[GLFW_KEY_W] = true;
        file_status = FileStatus::Write;
    }
}

void Controller::CheckCorrect(GLFWwindow *window, VisualCube &cube, Status &CubeStatus) {
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !m_KeyPressed[GLFW_KEY_A]) {
        m_KeyPressed[GLFW_KEY_A] = true;
        if (cube.is_correct()) {
            CubeStatus = Status::Correct;
        } else {
            CubeStatus = Status::Incorrect;
        }
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE && m_KeyPressed[GLFW_KEY_A]) {
        m_KeyPressed[GLFW_KEY_A] = false;
    }
}

void Controller::ComputeRotations(std::list<Rotation> &RotationOrder, GLFWwindow *window, const std::string &solution) {
    std::istringstream parcer(solution);
    std::string move;
    while (parcer >> move) {
        auto rot = m_RotationMapping[move.c_str()];
        auto kek = move.c_str();
        RotationOrder.push_back(m_RotationMapping[move]);
    }
}

void Controller::GetAnimationSkipStatus(GLFWwindow *window, Status &AnimationSkip) {
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && !m_KeyPressed[GLFW_KEY_ENTER]) {
        if (AnimationSkip == Status::Correct) {
            std::cout << "Animation enabled" << std::endl;
            AnimationSkip = Status::Incorrect;
        } else if (AnimationSkip == Status::Incorrect) {
            std::cout << "Animation disabled" << std::endl;
            AnimationSkip = Status::Correct;
        }
        m_KeyPressed[GLFW_KEY_ENTER] = true;
    } else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE && m_KeyPressed[GLFW_KEY_ENTER]) {
        m_KeyPressed[GLFW_KEY_ENTER] = false;
    }
}

void Controller::Shuffle(GLFWwindow *window, VisualCube &cube, std::list<Rotation> &RotationOrder) {
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !m_KeyPressed[GLFW_KEY_Z]) {
        ComputeRotations(RotationOrder, window, cube.shuffle());
        m_KeyPressed[GLFW_KEY_Z] = true;
    } else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE && m_KeyPressed[GLFW_KEY_Z]) {
        m_KeyPressed[GLFW_KEY_Z] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && !m_KeyPressed[GLFW_KEY_X]) {
        cube = VisualCube();
        m_KeyPressed[GLFW_KEY_X] = true;
    } else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE && m_KeyPressed[GLFW_KEY_X]) {
        m_KeyPressed[GLFW_KEY_X] = false;
    }
}


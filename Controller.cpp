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

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        std::cerr << m_Position.x << " " << m_Position.y << " " << m_Position.z << std::endl;
        std::cerr << m_HorizontalAngle << " " << m_VerticalAngle << std::endl;
    }
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
    //FIXME fixed point for rotation

    //// Z rotation
    GLint key = GLFW_KEY_0;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
        glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        Rotation rot;
        rot.deltaSign = 1.0f;
        rot.axis = glm::vec3(0.0f, 0.0f, 1.0f);
        rot.point = glm::vec3(2.0f, 2.0f, 0.0f);
        rot.CubesToRotateX.insert(2);
        rot.CubesToRotateX.insert(0);
        rot.CubesToRotateX.insert(1);
        rot.Move = "z2";

        RotationOrder.push_back(rot);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        Rotation rot;
        rot.deltaSign = 1.0f;
        rot.axis = glm::vec3(0.0f, 0.0f, 1.0f);
        rot.point = glm::vec3(2.0f, 2.0f, 0.0f);
        rot.CubesToRotateX.insert(2);
        rot.CubesToRotateX.insert(0);
        rot.CubesToRotateX.insert(1);
        rot.Move = "z'";

        RotationOrder.push_back(rot);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        Rotation rot;
        rot.deltaSign = -1.0f;
        rot.axis = glm::vec3(0.0f, 0.0f, 1.0f);
        rot.point = glm::vec3(2.0f, 2.0f, 0.0f);
        rot.CubesToRotateX.insert(2);
        rot.CubesToRotateX.insert(0);
        rot.CubesToRotateX.insert(1);
        rot.Move = "z";

        RotationOrder.push_back(rot);

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

        Rotation rot;
        rot.deltaSign = 1.0f;
        rot.axis = glm::vec3(0.0f, 1.0f, 0.0f);
        rot.point = glm::vec3(2.0f, 4.0f, -2.0f);
        rot.CubesToRotateX.insert(2);
        rot.CubesToRotateX.insert(0);
        rot.CubesToRotateX.insert(1);
        rot.Move = "y2";

        RotationOrder.push_back(rot);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        Rotation rot;
        rot.deltaSign = 1.0f;
        rot.axis = glm::vec3(0.0f, 1.0f, 0.0f);
        rot.point = glm::vec3(2.0f, 4.0f, -2.0f);
        rot.CubesToRotateX.insert(2);
        rot.CubesToRotateX.insert(0);
        rot.CubesToRotateX.insert(1);
        rot.Move = "y'";

        RotationOrder.push_back(rot);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        Rotation rot;
        rot.deltaSign = -1.0f;
        rot.axis = glm::vec3(0.0f, 1.0f, 0.0f);
        rot.point = glm::vec3(2.0f, 4.0f, -2.0f);
        rot.CubesToRotateX.insert(2);
        rot.CubesToRotateX.insert(0);
        rot.CubesToRotateX.insert(1);
        rot.Move = "y";

        RotationOrder.push_back(rot);

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
        Rotation test;
        test.deltaSign = 1.0f;
        test.axis = glm::vec3(1.0f, 0.0f, 0.0f);
        test.point = glm::vec3(4.0f, 2.0f, -2.0f);
        test.CubesToRotateX.insert(2);
        test.CubesToRotateX.insert(0);
        test.CubesToRotateX.insert(1);
        test.Move = "x2";

        RotationOrder.push_back(test);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        Rotation test;
        test.deltaSign = 1.0f;
        test.axis = glm::vec3(1.0f, 0.0f, 0.0f);
        test.point = glm::vec3(4.0f, 2.0f, -2.0f);
        test.CubesToRotateX.insert(2);
        test.CubesToRotateX.insert(0);
        test.CubesToRotateX.insert(1);
        test.Move = "x'";

        RotationOrder.push_back(test);

        m_KeyPressed[key] = true;

    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        Rotation test;
        test.deltaSign = -1.0f;
        test.axis = glm::vec3(1.0f, 0.0f, 0.0f);
        test.point = glm::vec3(4.0f, 2.0f, -2.0f);
        test.CubesToRotateX.insert(2);
        test.CubesToRotateX.insert(0);
        test.CubesToRotateX.insert(1);
        test.Move = "x";

        RotationOrder.push_back(test);

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

        Rotation rot;
        rot.deltaSign = 1.0f;
        rot.axis = glm::vec3(0.0f, 1.0f, 0.0f);
        rot.point = glm::vec3(2.0f, 4.0f, -2.0f);
        rot.CubesToRotateY.insert(2);
        rot.Move = "U2";

        RotationOrder.push_back(rot);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        Rotation rot;
        rot.deltaSign = 1.0f;
        rot.axis = glm::vec3(0.0f, 1.0f, 0.0f);
        rot.point = glm::vec3(2.0f, 4.0f, -2.0f);
        rot.CubesToRotateY.insert(2);
        rot.Move = "U'";

        RotationOrder.push_back(rot);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        Rotation rot;
        rot.deltaSign = -1.0f;
        rot.axis = glm::vec3(0.0f, 1.0f, 0.0f);
        rot.point = glm::vec3(2.0f, 4.0f, -2.0f);
        rot.CubesToRotateY.insert(2);
        rot.Move = "U";

        RotationOrder.push_back(rot);

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

        Rotation rot;
        rot.deltaSign = 1.0f;
        rot.axis = glm::vec3(0.0f, 1.0f, 0.0f);
        rot.point = glm::vec3(2.0f, 4.0f, -2.0f);
        rot.CubesToRotateY.insert(0);
        rot.Move = "D2";

        RotationOrder.push_back(rot);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        Rotation rot;
        rot.deltaSign = -1.0f;
        rot.axis = glm::vec3(0.0f, 1.0f, 0.0f);
        rot.point = glm::vec3(2.0f, 4.0f, -2.0f);
        rot.CubesToRotateY.insert(0);
        rot.Move = "D'";

        RotationOrder.push_back(rot);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        Rotation rot;
        rot.deltaSign = 1.0f;
        rot.axis = glm::vec3(0.0f, 1.0f, 0.0f);
        rot.point = glm::vec3(2.0f, 4.0f, -2.0f);
        rot.CubesToRotateY.insert(0);
        rot.Move = "D";

        RotationOrder.push_back(rot);

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
        Rotation rot;
        rot.deltaSign = 1.0f;
        rot.axis = glm::vec3(0.0f, 0.0f, 1.0f);
        rot.point = glm::vec3(2.0f, 2.0f, 0.0f);
        rot.CubesToRotateZ.insert(0);
        rot.Move = "F2";

        RotationOrder.push_back(rot);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        Rotation rot;
        rot.deltaSign = 1.0f;
        rot.axis = glm::vec3(0.0f, 0.0f, 1.0f);
        rot.point = glm::vec3(2.0f, 2.0f, 0.0f);
        rot.CubesToRotateZ.insert(0);
        rot.Move = "F'";

        RotationOrder.push_back(rot);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        Rotation rot;
        rot.deltaSign = -1.0f;
        rot.axis = glm::vec3(0.0f, 0.0f, 1.0f);
        rot.point = glm::vec3(2.0f, 2.0f, 0.0f);
        rot.CubesToRotateZ.insert(0);
        rot.Move = "F";

        RotationOrder.push_back(rot);

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
        Rotation rot;
        rot.deltaSign = 1.0f;
        rot.axis = glm::vec3(0.0f, 0.0f, 1.0f);
        rot.point = glm::vec3(2.0f, 2.0f, 0.0f);
        rot.CubesToRotateZ.insert(2);
        rot.Move = "B2";

        RotationOrder.push_back(rot);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        Rotation rot;
        rot.deltaSign = -1.0f;
        rot.axis = glm::vec3(0.0f, 0.0f, 1.0f);
        rot.point = glm::vec3(2.0f, 2.0f, 0.0f);
        rot.CubesToRotateZ.insert(2);
        rot.Move = "B'";

        RotationOrder.push_back(rot);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        Rotation rot;
        rot.deltaSign = 1.0f;
        rot.axis = glm::vec3(0.0f, 0.0f, 1.0f);
        rot.point = glm::vec3(2.0f, 2.0f, 0.0f);
        rot.CubesToRotateZ.insert(2);
        rot.Move = "B";

        RotationOrder.push_back(rot);

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
        Rotation test;
        test.deltaSign = 1.0f;
        test.axis = glm::vec3(1.0f, 0.0f, 0.0f);
        test.point = glm::vec3(4.0f, 2.0f, -2.0f);
        test.CubesToRotateX.insert(2);
        test.Move = "R2";

        RotationOrder.push_back(test);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        Rotation test;
        test.deltaSign = 1.0f;
        test.axis = glm::vec3(1.0f, 0.0f, 0.0f);
        test.point = glm::vec3(4.0f, 2.0f, -2.0f);
        test.CubesToRotateX.insert(2);
        test.Move = "R'";

        RotationOrder.push_back(test);

        m_KeyPressed[key] = true;

    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        Rotation test;
        test.deltaSign = -1.0f;
        test.axis = glm::vec3(1.0f, 0.0f, 0.0f);
        test.point = glm::vec3(4.0f, 2.0f, -2.0f);
        test.CubesToRotateX.insert(2);
        test.Move = "R";

        RotationOrder.push_back(test);

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
        Rotation test;
        test.deltaSign = 1.0f;
        test.axis = glm::vec3(1.0f, 0.0f, 0.0f);
        test.point = glm::vec3(4.0f, 2.0f, -2.0f);
        test.CubesToRotateX.insert(0);
        test.Move = "L2";

        RotationOrder.push_back(test);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        Rotation test;
        test.deltaSign = -1.0f;
        test.axis = glm::vec3(1.0f, 0.0f, 0.0f);
        test.point = glm::vec3(4.0f, 2.0f, -2.0f);
        test.CubesToRotateX.insert(0);
        test.Move = "L'";

        RotationOrder.push_back(test);

        m_KeyPressed[key] = true;

    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        Rotation test;
        test.deltaSign = 1.0f;
        test.axis = glm::vec3(1.0f, 0.0f, 0.0f);
        test.point = glm::vec3(4.0f, 2.0f, -2.0f);
        test.CubesToRotateX.insert(0);
        test.Move = "L";

        RotationOrder.push_back(test);

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
        Rotation test;
        test.deltaSign = 1.0f;
        test.axis = glm::vec3(1.0f, 0.0f, 0.0f);
        test.point = glm::vec3(4.0f, 2.0f, -2.0f);
        test.CubesToRotateX.insert(1);
        test.Move = "M2";

        RotationOrder.push_back(test);

        m_KeyPressed[key] = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
               glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {

        Rotation test;
        test.deltaSign = -1.0f;
        test.axis = glm::vec3(1.0f, 0.0f, 0.0f);
        test.point = glm::vec3(4.0f, 2.0f, -2.0f);
        test.CubesToRotateX.insert(1);
        test.Move = "M'";

        RotationOrder.push_back(test);

        m_KeyPressed[key] = true;

    } else if (glfwGetKey(window, key) == GLFW_PRESS && !m_KeyPressed[key]) {
        Rotation test;
        test.deltaSign = 1.0f;
        test.axis = glm::vec3(1.0f, 0.0f, 0.0f);
        test.point = glm::vec3(4.0f, 2.0f, -2.0f);
        test.CubesToRotateX.insert(1);
        test.Move = "M";

        RotationOrder.push_back(test);

        m_KeyPressed[key] = true;
    }

    if (glfwGetKey(window, key) == GLFW_RELEASE && m_KeyPressed[key]) {
        m_KeyPressed[key] = false;
    }
    ////M end
}

FileStatus Controller::FileWorkingCheck(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE && m_KeyPressed[GLFW_KEY_R]) {
        m_KeyPressed[GLFW_KEY_R] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && m_KeyPressed[GLFW_KEY_W]) {
        m_KeyPressed[GLFW_KEY_W] = false;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !m_KeyPressed[GLFW_KEY_R]) {
        m_KeyPressed[GLFW_KEY_R] = true;
        return FileStatus::Read;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !m_KeyPressed[GLFW_KEY_W]) {
        m_KeyPressed[GLFW_KEY_W] = true;
        return FileStatus::Write;
    }

    return FileStatus::DoNothing;
}

CubeStatus Controller::CheckCorrect(GLFWwindow *window, VisualCube& cube) {
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !m_KeyPressed[GLFW_KEY_A]) {
        m_KeyPressed[GLFW_KEY_A] = true;
        if (cube.is_correct()) {
            return CubeStatus::Correct;
        } else {
            return CubeStatus::Incorrect;
        }
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE && m_KeyPressed[GLFW_KEY_A]) {
        m_KeyPressed[GLFW_KEY_A] = false;
    }
    return CubeStatus::DontAsked;
}

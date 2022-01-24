//
// Created by MTBLE on 22.01.2022.
//

#include "VisualCube.h"

VisualCube::VisualCube(const std::string &vertexShader, const std::string &fragmentShader)
        : Cube(), m_VAO(), m_Shader(vertexShader, fragmentShader), m_Angle(0.0f), m_VBO(g_RubiksCubeVertices, sizeof(g_RubiksCubeVertices)){

    InitTextures();
    InitIndexBuffers();

    VertexBufferLayout layout;
    layout.Push<GLfloat>(3);
    layout.Push<GLfloat>(2);
    m_VAO.AddBuffer(m_VBO, layout);
}

void VisualCube::DrawCubes() {
    for (int x = 0; x != 3; ++x) {
        for (int y = 0; y != 3; ++y) {
            for (int z = 0; z != 3; ++z) {
//                std::cerr << "Drawing " << x << " " << y << " " << z << std::endl;
                DrawCube(x, y, z);
            }
        }
    }
}

void VisualCube::DrawCube(int x, int y, int z) {
//    std::cerr << gen_cubestring(true) << std::endl;
    m_VAO.Bind();
    m_Shader.Bind();
    glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), m_CubeLocations[x][y][z]);

    if (!m_RotationOrder.empty()) {
        auto &rot = m_RotationOrder.front();
        if (fabs(m_Angle) > 3.14f / 2.0f) {
            m_Angle = 0.0f;
            m_RotationOrder.pop_front();
        } else if (x == rot.cubesToRotate.x ||
                   y == rot.cubesToRotate.y ||
                   z == rot.cubesToRotate.z) {
            ModelMatrix = myrotate(rot.axis, rot.point, m_Angle) * ModelMatrix;
        }
    }
    glm::mat4 MVP = m_ProjectionMatrix * viewMatrix * ModelMatrix;
//
    m_Shader.SetUniformMatrix4fv("u_ModelMatrix", MVP);

    //-----------------------------Bullshit start---------------------------
    if (z == 0) {
        DrawSide(Front, faces[Front][2 - y][x]);
        
        //Invisible
        DrawSide(Back, Blue);
    } else if (z == 2) {
        DrawSide(Back, faces[Back][x][y]);
        //Invisible
        DrawSide(Front, Blue);
    } else {
        //Invisible
        DrawSide(Front, Red);
        //Invisible
        DrawSide(Back, Red);
    }

    if (x == 0) {
        DrawSide(Left, faces[Left][y][z]);
        //Invisible
        DrawSide(Right, White);
    } else if (x == 2) {
        DrawSide(Right, faces[Right][2 - y][z]);
        //Invisible
        DrawSide(Left, White);
    } else {
        //Invisible
        DrawSide(Right, White);
        //Invisible
        DrawSide(Right, White);
    }

    if (y == 0) {
        DrawSide(Bottom, faces[Bottom][z][x]);
        //Invisible
        DrawSide(Top, Red);
    } else if (y == 2) {
        DrawSide(Top, faces[Top][2 - z][x]);
        //Invisible
        DrawSide(Bottom, Orange);
    } else {
        //Invisible
        DrawSide(Top, White);
        //Invisible
        DrawSide(Bottom, White);
    }

    //-----------------------------Bullshit end---------------------------
}

void VisualCube::DrawSide(Side side, Color color) {
    m_Shader.Bind();
    m_Color[Green].Bind(0);
    m_Color[Orange].Bind(1);
    m_Color[Red].Bind(2);
    m_Color[White].Bind(3);
    m_Color[Yellow].Bind(4);
    m_Color[Blue].Bind(5);
    m_Shader.SetUniform1i("u_NumberTexture", color);

//    std::cerr << "Drawing " << side << " side " << color << " color"<< std::endl;
    
    Renderer::Draw(m_VAO, m_SideIB[side], m_Shader);
}

glm::mat4 VisualCube::myrotate(const glm::vec3 &axis, const glm::vec3 &point, GLfloat angle) {
    GLfloat x = axis.x * sin(angle / 2.0f);
    GLfloat y = axis.y * sin(angle / 2.0f);
    GLfloat z = axis.z * sin(angle / 2.0f);
    GLfloat w = cos(angle / 2.0f);
    glm::quat myQuat(w, x, y, z);

    GLfloat xd = axis.x * sin(3.14f / 4.0f);
    GLfloat yd = axis.y * sin(3.14f / 4.0f);
    GLfloat zd = axis.z * sin(3.14f / 4.0f);
    GLfloat wd = cos(3.14f / 4.0f);
    glm::quat myQuatd(wd, xd, yd, zd);

    glm::quat res = glm::mix(myQuat, myQuatd, angle * 2.0f/ 3.14f);
    return glm::translate(glm::mat4(1.0f), point) * glm::toMat4(res) *
           glm::translate(glm::mat4(1.0f), -point);
}

void VisualCube::InitTextures() {
    //FIXME add relative path
    m_Color.reserve(6);

    m_Color.emplace_back("C:\\my stuff\\itmo\\cpp bullshit\\glut_test\\VisualCube\\green.png");
    m_Shader.SetUniform1i("u_GreenTex", 0);

    m_Color.emplace_back("C:\\my stuff\\itmo\\cpp bullshit\\glut_test\\VisualCube\\orange.png");
    m_Shader.SetUniform1i("u_OrangeTex", 1);

    m_Color.emplace_back("C:\\my stuff\\itmo\\cpp bullshit\\glut_test\\VisualCube\\red.png");
    m_Shader.SetUniform1i("u_RedTex", 2);

    m_Color.emplace_back("C:\\my stuff\\itmo\\cpp bullshit\\glut_test\\VisualCube\\white.png");
    m_Shader.SetUniform1i("u_WhiteTex", 3);

    m_Color.emplace_back("C:\\my stuff\\itmo\\cpp bullshit\\glut_test\\VisualCube\\yellow.png");
    m_Shader.SetUniform1i("u_YellowTex", 4);

    m_Color.emplace_back("C:\\my stuff\\itmo\\cpp bullshit\\glut_test\\VisualCube\\blue.png");
    m_Shader.SetUniform1i("u_BlueTex", 5);

    m_Color[Green].Bind(Green);
    m_Color[Orange].Bind(Orange);
    m_Color[Red].Bind(Red);
    m_Color[White].Bind(White);
    m_Color[Yellow].Bind(Yellow);
    m_Color[Blue].Bind(Blue);
}

void VisualCube::Controller() {
    //    static double lastTime = glfwGetTime();
    //
    //    // Compute time difference between current and last frame
    //    double currentTime = glfwGetTime();
    //    float deltaTime = float(currentTime - lastTime);
    //
    //    // Get mouse position
    //    double xpos, ypos;
    //    glfwGetCursorPos(window, &xpos, &ypos);
    //
    //    // Reset mouse position for next frame
    //    glfwSetCursorPos(window, 1024 / 2, 768 / 2);
    //
    //    // Compute new orientation
    //    m_HorizontalAngle += m_MouseSpeed * float(1024 / 2 - xpos);
    //    m_VerticalAngle += m_MouseSpeed * float(768 / 2 - ypos);
    //
    //    if (fabs(m_HorizontalAngle) > 3.14f * 2.0f) {
    //        if (m_HorizontalAngle > 0) {
    //            m_HorizontalAngle -= 3.14f * 2.0f;
    //        } else {
    //            m_HorizontalAngle += 3.14f * 2.0f;
    //        }
    //    }
    //
    //    if (fabs(m_VerticalAngle) > 3.14f * 2.0f) {
    //        if (m_VerticalAngle > 0) {
    //            m_VerticalAngle -= 3.14f * 2.0f;
    //        } else {
    //            m_VerticalAngle += 3.14f * 2.0f;
    //        }
    //    }
    //
    //
    //    // Direction : Spherical coordinates to Cartesian coordinates conversion
    //    glm::vec3 direction(
    //            cos(m_VerticalAngle) * sin(m_HorizontalAngle),
    //            sin(m_VerticalAngle),
    //            cos(m_VerticalAngle) * cos(m_HorizontalAngle)
    //    );
    //
    //    // Right vector
    //    glm::vec3 right = glm::vec3(
    //            sin(m_HorizontalAngle - 3.14f / 2.0f),
    //            0,
    //            cos(m_HorizontalAngle - 3.14f / 2.0f)
    //    );
    //
    //    // Up vector
    //    glm::vec3 up = glm::cross(right, direction);
    //
    //    // Move forward
    //    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    //        position += direction * deltaTime * m_MovingSpeed;
    //    }
    //    // Move backward
    //    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    //        position -= direction * deltaTime * m_MovingSpeed;
    //    }
    //    // Strafe m_Right
    //    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    //        position += right * deltaTime * m_MovingSpeed;
    //    }
    //    // Strafe left
    //    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    //        position -= right * deltaTime * m_MovingSpeed;
    //    }
    //
    //    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    //        position -= up * deltaTime * m_MovingSpeed;
    //    } else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    //        position += up * deltaTime * m_MovingSpeed;
    //    }
    //
    //
    //    float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.
    //
    //    // Camera matrix
    //    viewMatrix = glm::lookAt(
    //            position,           // Camera is here
    //            position + direction, // and looks here : at the same position, plus "m_Direction"
    //            up                  // Head is up (set to 0,-1,0 to look upside-down)
    //    );
    //
    //    // For the next frame, the "last time" will be "now"
    //    lastTime = currentTime;
;
}

void VisualCube::InitIndexBuffers() {
    m_SideIB.reserve(6);

    m_SideIB.emplace_back(g_FrontIndices, 6);
    m_SideIB.emplace_back(g_LeftIndices, 6);
    m_SideIB.emplace_back(g_RightIndices, 6);
    m_SideIB.emplace_back(g_UpIndices, 6);
    m_SideIB.emplace_back(g_DownIndices, 6);
    m_SideIB.emplace_back(g_BackIndices, 6);
}

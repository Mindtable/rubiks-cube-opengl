//
// Created by MTBLE on 21.01.2022.
//

#include "Renderer.h"

void GLClearError() {
    while (glGetError() != GL_NO_ERROR) { ;
    }
}

void GLPrintError() {
    while (GLenum err = glGetError()) {
        std::cout << "OpenGL error (" << err << ")" << std::endl;
    }
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) {
    shader.Bind();
    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Renderer::~Renderer() {
    glfwTerminate();
}

Renderer::Renderer(const std::string &vertexShader, const std::string &fragmentShader) : m_Shader(vertexShader,
                                                                                                  fragmentShader),
                                                                                         m_VAO(),
                                                                                         m_VBO(g_RubiksCubeVertices,
                                                                                               sizeof(g_RubiksCubeVertices)),
                                                                                         m_VisualCube(), m_Angle(0.0f),
                                                                                         m_Controller(false) {
    InitTextures();
    InitIndexBuffers();

    VertexBufferLayout layout;
    layout.Push<GLfloat>(3);
    layout.Push<GLfloat>(2);
    layout.Push<GLfloat>(3);
    m_VAO.AddBuffer(m_VBO, layout);
}

void Renderer::Swap() {
    glfwSwapBuffers(m_Window);
    glfwPollEvents();

}

void Renderer::InitTextures() {
    //FIXME add relative path
    m_Color.reserve(6);

    //FIXME these uniforms could be excessive
    m_Color.emplace_back("res\\green.png");
//    m_Shader.SetUniform1i("u_GreenTex", 0);

    m_Color.emplace_back("res\\orange.png");
//    m_Shader.SetUniform1i("u_OrangeTex", 1);

    m_Color.emplace_back("res\\red.png");
//    m_Shader.SetUniform1i("u_RedTex", 2);

    m_Color.emplace_back("res\\white.png");
//    m_Shader.SetUniform1i("u_WhiteTex", 3);

    m_Color.emplace_back("res\\yellow.png");
//    m_Shader.SetUniform1i("u_YellowTex", 4);

    m_Color.emplace_back("res\\blue.png");
//    m_Shader.SetUniform1i("u_BlueTex", 5);

    m_Color[Green].Bind(Green);
    m_Color[Orange].Bind(Orange);
    m_Color[Red].Bind(Red);
    m_Color[White].Bind(White);
    m_Color[Yellow].Bind(Yellow);
    m_Color[Blue].Bind(Blue);
}

void Renderer::InitIndexBuffers() {
    m_SideIB.reserve(6);

    m_SideIB.emplace_back(g_FrontIndices, 6);
    m_SideIB.emplace_back(g_LeftIndices, 6);
    m_SideIB.emplace_back(g_RightIndices, 6);
    m_SideIB.emplace_back(g_UpIndices, 6);
    m_SideIB.emplace_back(g_DownIndices, 6);
    m_SideIB.emplace_back(g_BackIndices, 6);

}

void Renderer::Show() {

//    m_VisualCube.exec_perm(
//            "U R2 F B R B2 R U2 L B2 R U' D' R2 F R' L B2 U2 F2");
    glfwSetCursorPos(m_Window, 1024 / 2, 768 / 2);

    bool AnimationNeeded = false;
    GLfloat AnimationScale = 0.99f;
    GLfloat AnimationDelta = -0.03f;
    glm::mat4 Animation = glm::mat4(1.0f);

    FileStatus file_status = FileStatus::DoNothing;

    State FileReader = State::NotWorking;

    VisualCube res_cube;

    CubeStatus CubeCorrect = CubeStatus::Correct;

    while (!glfwWindowShouldClose(m_Window)) {

        if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break;
        }

        if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS) {
            AnimationNeeded = true;
        }

        m_Shader.SetUniform3f("u_CameraPos", 11.0123f,  6.59808f,  7.06434f);
        m_Shader.SetUniform3f("u_LightPos", 6.0f, 6.0f, 2.0f);
        m_Shader.SetUniform4f("u_LightColor", 1.0f, 1.0f, 1.0f, 1.0f);

        m_Shader.SetUniform1i("u_AnimationNeeded", AnimationNeeded);
        if (AnimationNeeded) {
            if (AnimationScale > 0) {
                if (AnimationScale > 0.99f) {
                    AnimationNeeded = false;
                    AnimationScale = 0.99f;
                    AnimationDelta = -0.03f;
                } else {
                    AnimationScale += AnimationDelta;
                }
            } else {
                AnimationDelta *= -1.0f;
                AnimationScale += AnimationDelta;
            }
            Animation = glm::scale(glm::mat4(1.0f), AnimationScale * glm::vec3(1.0f));

            m_Shader.SetUniformMatrix4fv("u_Animation", Animation);
        }

        Renderer::Clear();

        glm::mat4 floating = glm::translate(glm::mat4(1.0f),
                                            glm::vec3(0.0f, glm::sin(-glfwGetTime() * 2.0f) / 10.0f, 0.0f));

        m_Shader.SetUniformMatrix4fv("u_Floating", floating);

        if (FileReader == State::NotWorking){
            m_Controller.ComputeRotations(m_RotationOrder, m_Window);

            m_Controller.ComputeProjectionView(m_Shader, m_Window);

            file_status = m_Controller.FileWorkingCheck(m_Window);

            CubeStatus res = m_Controller.CheckCorrect(m_Window, m_VisualCube);
            if (res != CubeStatus::DontAsked) {
                CubeCorrect = res;
            }
        }

        if (FileReader == State::ReadyToDrop) {
            AnimationNeeded = true;
            if (AnimationDelta > 0.0f && AnimationScale < 0.7f) {
                m_VisualCube = res_cube;
                FileReader = State::NotWorking;
            }
        }

        if (file_status == FileStatus::Read) {
            file_status = FileStatus::DoNothing;
            std::thread thr([&res_cube, &FileReader](){
                FileReader = State::InProcess;
                std::cout << "Reading file...\nEnter filename:" << std::flush;
                std::string filename;
                std::cin >> filename;
                try {
                    res_cube.fromfile(filename.c_str());
                    std::cout << res_cube.gen_cubestring(true) << std::endl;
                    res_cube.ClearRoute();
                    FileReader = State::ReadyToDrop;
                } catch (std::runtime_error& ex) {
                    std::cout << ex.what() << std::endl;
                    FileReader = State::NotWorking;
                }
            });
            thr.detach();
        } else if (file_status == FileStatus::Write) {
            file_status = FileStatus::DoNothing;
            std::thread thr([&FileReader, this](){
                FileReader = State::InProcess;
                std::cout << "Writing file...\nEnter filename:" << std::flush;
                std::string filename;
                std::cin >> filename;
                this->m_VisualCube.tofile(filename.c_str(), true);
                std::cout << "Writing completed" << std::endl;
                FileReader = State::NotWorking;
            });
            thr.detach();
        }

        if (!m_RotationOrder.empty()) {
            auto &rot = m_RotationOrder.front();
            m_Angle += rot.deltaSign * m_Delta;
            if (rot.Move.size() != 1 && rot.Move[1] == '2') {
                m_MaxAngle = 3.14f;
            } else {
                m_MaxAngle = 3.14f / 2.0f;
            }
        }

        DrawCubes();
        Swap();
    }
}

void Renderer::DrawCubes() {
    for (int x = 0; x != 3; ++x) {
        for (int y = 0; y != 3; ++y) {
            for (int z = 0; z != 3; ++z) {
//                std::cerr << "Drawing " << x << " " << y << " " << z << std::endl;
                DrawCube(x, y, z);
            }
        }
    }

}

void Renderer::DrawCube(int x, int y, int z) {
    m_Shader.Bind();
    glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), m_CubeLocations[x][y][z]);

    if (!m_RotationOrder.empty()) {
        auto &rot = m_RotationOrder.front();
        if (fabs(m_Angle) > m_MaxAngle) {
            m_Angle = 0.00000000001f;
            m_VisualCube.exec_move(rot.Move);
            m_RotationOrder.pop_front();
        } else if (rot.CubesToRotateX.count(x) ||
                   rot.CubesToRotateY.count(y) ||
                   rot.CubesToRotateZ.count(z)) {

//            std::cerr << m_Angle << std::endl;

            ModelMatrix = myrotate(rot.axis, rot.point, m_Angle) * ModelMatrix;
        }
    }
    glm::mat4 MVP = ModelMatrix;

    m_Shader.SetUniformMatrix4fv("u_ModelMatrix", MVP);

    std::vector<Color> colors(6, Blue);
    m_VisualCube.GetColor(colors, x, y, z);

    //FIXME these bindings could be excessive
//    m_Color[Green].Bind(0);
//    m_Color[Orange].Bind(1);
//    m_Color[Red].Bind(2);
//    m_Color[White].Bind(3);
//    m_Color[Yellow].Bind(4);
//    m_Color[Blue].Bind(5);

    for (int i = Front; i != Back + 1; ++i) {
        m_Shader.SetUniform1i("u_NumberTexture", colors[i]);
        Renderer::Draw(m_VAO, m_SideIB[i], m_Shader);
    }
}

glm::mat4 Renderer::myrotate(const glm::vec3 &axis, const glm::vec3 &point, GLfloat angle) {
    GLfloat delta = 1.0f;
    if (angle < 0) {
//        angle *= 1.0f;
        delta = -1.0f;
    }
    GLfloat x = axis.x * glm::sin(angle / 2.0f);
    GLfloat y = axis.y * glm::sin(angle / 2.0f);
    GLfloat z = axis.z * glm::sin(angle / 2.0f);
    GLfloat w = glm::cos(angle / 2.0f);
    glm::quat myQuat(w, x, y, z);

    GLfloat xd = axis.x * glm::sin(delta * m_MaxAngle / 2.0f);
    GLfloat yd = axis.y * glm::sin(delta * m_MaxAngle / 2.0f);
    GLfloat zd = axis.z * glm::sin(delta * m_MaxAngle / 2.0f);
    GLfloat wd = glm::cos(delta * m_MaxAngle / 2.0f);
    glm::quat myQuatd(wd, xd, yd, zd);

    glm::quat res = glm::mix(myQuat, myQuatd, angle / m_MaxAngle);
    return glm::translate(glm::mat4(1.0f), point) * glm::toMat4(res) *
           glm::translate(glm::mat4(1.0f), -point);
}
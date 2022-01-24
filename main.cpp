#include <algorithm>
#include <cmath>
#include <list>
#include <set>
#include "dependences.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "VisualCube/VisualCube.h"

GLFWwindow *window;

glm::mat4 translationMatrix = glm::mat4(1.0f);
GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat z = 1.0f * sin(0.0f / 2.0f);
GLfloat w = cos(0.0f / 2.0f);

glm::quat MyQuat(w, x, y, z);
glm::mat4 rotateMatrix = glm::toMat4(MyQuat);
//glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
//glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.33f, 0.33f, 0.33f));
glm::mat4 scaleMatrix = glm::mat4(1.0f);
glm::mat4 modelMatrix = translationMatrix * rotateMatrix * scaleMatrix;

//Bullshit
glm::mat4 viewMatrix = glm::lookAt(
        glm::vec3(2.0f, 2.0f, 4.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
);

glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

// Initial position : on +Z
glm::vec3 position = glm::vec3(14.8f, 7.76f, 5.4f);
// Initial horizontal angle : toward -Z
float horizontalAngle = -2.14f;
// Initial vertical angle : none
float verticalAngle = -0.37f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;

struct Markup {
    IndexBuffer *ibo, *ibo2;
    Shader *shader;
    int number_of_cube;
    VertexArray *va;
    Renderer *rend;
};

struct RotationObject {
    GLfloat angle;
    GLfloat delta;
    glm::vec3 axis;
    glm::vec3 point;

    std::set<int> cubies;

    std::string move;
};

std::list<RotationObject> rotationOrder;
std::set<std::string> forbidden;

glm::quat myrotate(const glm::vec3 &axis, const glm::vec3 &point, GLfloat angle) {
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

    glm::quat res = glm::mix(myQuat, myQuatd, angle * 2.0f / 3.14f);
    return res;
//    return glm::translate(glm::mat4(1.0f), point) * glm::toMat4(res) *
//           glm::translate(glm::mat4(1.0f), -point);
}

void drawCube(const Markup &markup, const glm::vec3 &coordinates) {
//    std::cerr << "Drawing cubes" << std::endl;
    markup.shader->Bind();
//    if (markup.number_of_cube != 8) {
//        markup.shader->SetUniform4f("ourColor",  0.0f, 0.6f, 0.6f, 1.0f);
//    } else {
//        markup.shader->SetUniform4f("ourColor",  0.7f, 0.7f, 0.0f, 1.0f);
//    }
    modelMatrix = glm::translate(glm::mat4(1.0f), coordinates);

    glm::quat q1(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 trans(0.0f, 0.0f, 0.0f);
    int erasecount = 0;
    if (!rotationOrder.empty()) {
        auto &rot = rotationOrder.front();
        if (fabs(rot.angle) > 3.14f / 2.0f) {
            forbidden.erase(rotationOrder.front().move);
            ++erasecount;
        } else if (rot.cubies.count(markup.number_of_cube)) {
            q1 = myrotate(rot.axis, rot.point, rot.angle) * q1;
            trans += rot.point;
        }
    }
    modelMatrix = glm::translate(glm::mat4(1.0f), trans) *
                  glm::toMat4(q1) *
                  glm::translate(glm::mat4(1.0f), -trans) *
                  modelMatrix;
    for (int i = 0; i != erasecount; ++i) {
        rotationOrder.pop_front();
    }
    MVP = projectionMatrix * viewMatrix * modelMatrix;

    markup.shader->SetUniformMatrix4fv("scale", MVP);
    markup.rend->Draw(*(markup.va), *(markup.ibo), *(markup.shader));
//    glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, nullptr);

//    std::cerr << "Drawing lines" << std::endl;
//    markup.shader->SetUniform4f("ourColor",  0.0f, 1.0f, 1.0f, 1.0f);
//    markup.ibo2->Bind();
//    glDrawElements(GL_LINES, markup.ibo2->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void controller() {
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position for next frame
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Compute new orientation
    horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
    verticalAngle += mouseSpeed * float(768 / 2 - ypos);

    if (fabs(horizontalAngle) > 3.14f * 2.0f) {
        if (horizontalAngle > 0) {
            horizontalAngle -= 3.14f * 2.0f;
        } else {
            horizontalAngle += 3.14f * 2.0f;
        }
    }

    if (fabs(verticalAngle) > 3.14f * 2.0f) {
        if (verticalAngle > 0) {
            verticalAngle -= 3.14f * 2.0f;
        } else {
            verticalAngle += 3.14f * 2.0f;
        }
    }


    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
    );

    // Right vector
    glm::vec3 right = glm::vec3(
            sin(horizontalAngle - 3.14f / 2.0f),
            0,
            cos(horizontalAngle - 3.14f / 2.0f)
    );

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;
    }
    // Strafe m_Right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position -= up * deltaTime * speed;
    } else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position += up * deltaTime * speed;
    }


    float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    // Camera matrix
    viewMatrix = glm::lookAt(
            position,           // Camera is here
            position + direction, // and looks here : at the same position, plus "m_Direction"
            up                  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}

int main() {
    bool keyPressed[100]{false};


    //-------------------INITIAL PART-------------------
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL


    window = glfwCreateWindow(1024, 768, "tutor", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error glew init";
        glfwTerminate();
        return -1;
    };

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glEnable(GL_MULTISAMPLE);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    //-------------------DATA PART-------------------
    static const GLfloat g_vertex_buffer_data[] = {
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f
    };

    static const GLfloat texBuff[]{
            1.0f, 1.0f,
            0.0, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,

            1.0f, 1.0f,
            0.0, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f
    };

    static const GLfloat tex_quad[]{
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //DT
            1.0f, 1.0f, -1.0f, 1.0, 0.0f,
            -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //DT
            -1.0f, 1.0f, -1.0f, 0.0f, 0.0f,

            1.0f, -1.0f, 1.0f, 0.0f, 1.0f, //DT
            1.0f, -1.0f, -1.0f, 1.0, 1.0f,
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, //DT
            -1.0f, -1.0f, -1.0f, 1.0f, 0.0f
    };

    static const GLuint g_vertex_indices[]{
//        0, 1, 2,
//        2, 3, 0
            //Top
            0, 1, 3,
            0, 3, 2,

            //Right
            0, 4, 5,
            0, 1, 5,

            //Left
            2, 6, 7,
            2, 3, 7,

            //Front
            0, 2, 6,
            0, 4, 6,

            //Back
            1, 3, 7,
            1, 5, 7,

            //Down
            4, 6, 7,
            4, 5, 7
    };

    static const GLuint g_vertex_indices2[]{
            0, 1,
            1, 5,
            5, 4,
            4, 0,

            0, 1,
            1, 3,
            3, 2,
            2, 0,

            0, 4,
            4, 6,
            6, 2,
            2, 0,

            1, 3, 3, 7, 7, 5, 5, 1,

            2, 3, 3, 7, 7, 6, 6, 2,

            4, 5, 5, 7, 7, 6, 4,
    };

    const std::vector<glm::vec3> CubeLocations{
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 0.0f, 0.0f),
            glm::vec3(4.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 2.0f, 0.0f),
            glm::vec3(2.0f, 2.0f, 0.0f),
            glm::vec3(4.0f, 2.0f, 0.0f),
            glm::vec3(0.0f, 4.0f, 0.0f),
            glm::vec3(2.0f, 4.0f, 0.0f),
            glm::vec3(4.0f, 4.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -2.0f),
            glm::vec3(2.0f, 0.0f, -2.0f),
            glm::vec3(4.0f, 0.0f, -2.0f),
            glm::vec3(0.0f, 2.0f, -2.0f),
            glm::vec3(2.0f, 2.0f, -2.0f),
            glm::vec3(4.0f, 2.0f, -2.0f),
            glm::vec3(0.0f, 4.0f, -2.0f),
            glm::vec3(2.0f, 4.0f, -2.0f),
            glm::vec3(4.0f, 4.0f, -2.0f),
            glm::vec3(0.0f, 0.0f, -4.0f),
            glm::vec3(2.0f, 0.0f, -4.0f),
            glm::vec3(4.0f, 0.0f, -4.0f),
            glm::vec3(0.0f, 2.0f, -4.0f),
            glm::vec3(2.0f, 2.0f, -4.0f),
            glm::vec3(4.0f, 2.0f, -4.0f),
            glm::vec3(0.0f, 4.0f, -4.0f),
            glm::vec3(2.0f, 4.0f, -4.0f),
            glm::vec3(4.0f, 4.0f, -4.0f)
    };

    GLuint g_FrontIndices[6] = {
            1, 4, 17,
            1, 17, 13
    };

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

    //-------------------GEN BUFFERS-------------------

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    VertexArray vao;
////    VertexBuffer vb(g_vertex_buffer_data, sizeof(g_vertex_buffer_data));
//
//    VertexBuffer vb(tex_quad, sizeof(tex_quad));
//    VertexBufferLayout layout;
//    layout.Push<GLfloat>(3);
//    layout.Push<GLfloat>(2);
//    vao.AddBuffer(vb, layout);
////
//    IndexBuffer ibo(g_vertex_indices, 36);
//    IndexBuffer ibo2(g_vertex_indices2, 48);
    //-------------------CREATE SHADER-------------------
//    Shader shader("vertex.glsl",
//                               "fragment.glsl");
//    shader.Bind();
    Renderer renderer;
//    IndexBuffer index_buffer(g_FrontIndices, 6);

//    VertexArray vao;
//    VertexBuffer vb(g_RubiksCubeVertices, sizeof(g_RubiksCubeVertices));
//    VertexBufferLayout layout;
//    layout.Push<GLfloat>(3);
//    layout.Push<GLfloat>(2);
//    vao.AddBuffer(vb, layout);
//    layout.~VertexBufferLayout();
//
//    Texture texture("VisualCube\\orange.png");
//    shader.SetUniform1i("u_OrangeTex", 1);
//
//    Texture texture1("VisualCube\\green.png");
//    shader.SetUniform1i("u_GreenTex", 2);

//    texture.Bind(1);
//    texture1.Bind(2);
////    std::cerr << Red << std::endl;
//    shader.SetUniform1i("u_NumberTexture", 0);

    glfwSetCursorPos(window, 1024 / 2, 768 / 2);
//
//    Markup kek{&ibo, &ibo2, &shader, -1, &vao, &renderer};

//    GLPrintError();
//    texture.Bind(1);
//    shader.SetUniform1i("tex", 0);
    GLPrintError();
    VisualCube tttt("C:\\my stuff\\itmo\\cpp bullshit\\glut_test\\vertex.glsl",
                    "C:\\my stuff\\itmo\\cpp bullshit\\glut_test\\fragment.glsl");
    controller();
    tttt.exec_perm("R U R' U'");
    std::cerr << tttt.gen_cubestring(true) << std::endl;
    std::string input;
//    return 0;
    while (!glfwWindowShouldClose(window)) {

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS &&
            !keyPressed[GLFW_KEY_R]) {
            tttt.exec_move("R'");
            keyPressed[GLFW_KEY_R] = true;
        } else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !keyPressed[GLFW_KEY_R]) {
            tttt.exec_perm("R");
//            RotationObject rot{0.0f, 0.05f, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(2.0f, 2.0f, 0.0f),
//                               {0, 1, 2, 3, 4, 5, 6, 7, 8}, "F"};
//            rotationOrder.push_back(rot);
////            forbidden.insert("F");
//          std::cerr << "R pressed" << std::endl;
            keyPressed[GLFW_KEY_R] = true;
        }

        if (keyPressed[GLFW_KEY_R] && glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
            std::cerr << "R release" << std::endl;
            keyPressed[GLFW_KEY_R] = false;
        }
//
//        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !forbidden.count("R'")) {
//            RotationObject rot{0.0f, 0.05f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(4.0f, 2.0f, -2.0f),
//                               {2, 5, 8, 11, 14, 17, 20, 23, 26}, "R'"};
//            rotationOrder.push_back(rot);
//            forbidden.insert("R'");
//        }
//
//        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
//            shader.SetUniform1i("u_NumberTexture", 1);
//            std::cout << "Your current pos: " << position.x << " " << position.y << " " << position.z << std::endl;
//            std::cout << "Your angles: " << m_HorizontalAngle << " " << m_VerticalAngle << std::endl;
//        }


        renderer.Clear();
        glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;
//
//        tttt.m_Shader.SetUniformMatrix4fv("scale", MVP);
        tttt.DrawCubes();
//        if (rotationOrder.empty()) {
//            forbidden.clear();
//        }
//        if (!rotationOrder.empty()) {
//            auto &rot = rotationOrder.front();
//            rot.angle += rot.delta;
//        }

//        Renderer::Draw(vao, ibo, shader);
        //Blue cube

//        for (int i = 0; i != CubeLocations.size(); ++i) {
//            kek.number_of_cube = i;
//            const auto &elem = CubeLocations[i];
//            drawCube(kek, elem);
//        }
//        for (auto &elem: forbidden) {
//            std::cerr << elem << std::endl;
//        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}

//    std::cerr << "qqq";

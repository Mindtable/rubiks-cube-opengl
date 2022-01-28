//
// Created by MTBLE on 22.01.2022.
//

#include "VisualCube.h"

VisualCube::VisualCube()
        : Cube() {
    ;
}

//void VisualCube::DrawCube(int x, int y, int z) {
////    std::cerr << gen_cubestring(true) << std::endl;
////    m_VAO.Bind();
//    m_Shader.Bind();
//    glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), m_CubeLocations[x][y][z]);
//
//    if (!m_RotationOrder.empty()) {
//        auto &rot = m_RotationOrder.front();
//        if (fabs(m_Angle) > 3.14f / 2.0f) {
//            m_Angle = 0.0f;
//            m_RotationOrder.pop_front();
//        } else if (x == rot.cubesToRotate.x ||
//                   y == rot.cubesToRotate.y ||
//                   z == rot.cubesToRotate.z) {
//            ModelMatrix = myrotate(rot.axis, rot.point, m_Angle) * ModelMatrix;
//        }
//    }
//    glm::mat4 MVP = m_ProjectionMatrix * viewMatrix * ModelMatrix;
////
//    m_Shader.SetUniformMatrix4fv("u_ModelMatrix", MVP);
//
//    //-----------------------------Bullshit start---------------------------
//    if (z == 0) {
//        DrawSide(Front, faces[Front][2 - y][x]);
//
//        //Invisible
//        DrawSide(Back, Blue);
//    } else if (z == 2) {
//        DrawSide(Back, faces[Back][x][y]);
//        //Invisible
//        DrawSide(Front, Blue);
//    } else {
//        //Invisible
//        DrawSide(Front, Red);
//        //Invisible
//        DrawSide(Back, Red);
//    }
//
//    if (x == 0) {
//        DrawSide(Left, faces[Left][y][z]);
//        //Invisible
//        DrawSide(Right, White);
//    } else if (x == 2) {
//        DrawSide(Right, faces[Right][2 - y][z]);
//        //Invisible
//        DrawSide(Left, White);
//    } else {
//        //Invisible
//        DrawSide(Right, White);
//        //Invisible
//        DrawSide(Right, White);
//    }
//
//    if (y == 0) {
//        DrawSide(Bottom, faces[Bottom][z][x]);
//        //Invisible
//        DrawSide(Top, Red);
//    } else if (y == 2) {
//        DrawSide(Top, faces[Top][2 - z][x]);
//        //Invisible
//        DrawSide(Bottom, Orange);
//    } else {
//        //Invisible
//        DrawSide(Top, White);
//        //Invisible
//        DrawSide(Bottom, White);
//    }
//
//    //-----------------------------Bullshit end---------------------------
//}

void VisualCube::GetColor(std::vector<Color> &colors, int x, int y, int z) {
    if (z == 0) {
        colors[Front] = faces[Front][2 - y][x];

        //Invisible
        colors[Back] = Blue;
    } else if (z == 2) {
        colors[Back] = faces[Back][2 - y][2 - x];
        //Invisible
        colors[Front] = Blue;
    } else {
        //Invisible
        colors[Front] = Blue;
        //Invisible
        colors[Back] = Blue;
    }

    if (x == 0) {
        colors[Left] = faces[Left][2 - y][2 - z];
        //Invisible
        colors[Right] = Blue;
    } else if (x == 2) {
        colors[Right] = faces[Right][2 - y][z];
        //Invisible
        colors[Left] = Blue;
    } else {
        //Invisible
        colors[Left] = Blue;
        //Invisible
        colors[Right] = Blue;
    }

    if (y == 0) {
        colors[Bottom] = faces[Bottom][z][x];
        //Invisible
        colors[Top] = Blue;
    } else if (y == 2) {
        colors[Top] = faces[Top][2 - z][x];
        //Invisible
        colors[Bottom] = Blue;
    } else {
        //Invisible
        colors[Top] = Blue;
        //Invisible
        colors[Bottom] = Blue;
    }
}

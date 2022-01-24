cmake_minimum_required(VERSION 3.20)
project(glut_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS -static)

add_subdirectory(glfw)
add_subdirectory(glew)
add_subdirectory(glm)
add_subdirectory(stb-cmake)
add_executable(glut_test
        main.cpp
        Renderer.cpp Renderer.h
        VertexBuffer.cpp VertexBuffer.h
        IndexBuffer.cpp IndexBuffer.h
        VertexArray.cpp VertexArray.h
        VertexBufferLayout.cpp VertexBufferLayout.h
        Shader.cpp Shader.h
        stb_image.cpp
        Texture.cpp Texture.h
        VisualCube/VisualCube.cpp VisualCube/VisualCube.h
        VisualCube/Cube.h VisualCube/dependences.h
        VisualCube/Solver.h
        VisualCube/Cube.cpp)

target_include_directories(glut_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)
target_include_directories(glut_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glew/include)
target_link_libraries(glut_test glfw libglew_static glm stb::image)
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
        src/BasicOpenglObj/VertexBuffer.cpp src/BasicOpenglObj/VertexBuffer.h
        src/BasicOpenglObj/IndexBuffer.cpp src/BasicOpenglObj/IndexBuffer.h
        src/BasicOpenglObj/VertexArray.cpp src/BasicOpenglObj/VertexArray.h
        src/BasicOpenglObj/VertexBufferLayout.cpp src/BasicOpenglObj/VertexBufferLayout.h
        src/BasicOpenglObj/Shader.cpp src/BasicOpenglObj/Shader.h
        stb_image.cpp
        src/BasicOpenglObj/Texture.cpp src/BasicOpenglObj/Texture.h
        VisualCube/VisualCube.cpp VisualCube/VisualCube.h
        VisualCube/Cube.h VisualCube/dependences.h
        VisualCube/Solver.h
        Controller.h Controller.cpp
        VisualCube/Cube.cpp src/BasicOpenglObj/BasicRenderer.cpp src/BasicOpenglObj/BasicRenderer.h)

target_include_directories(glut_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)
target_include_directories(glut_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glew/include)
target_link_libraries(glut_test glfw libglew_static glm stb::image )

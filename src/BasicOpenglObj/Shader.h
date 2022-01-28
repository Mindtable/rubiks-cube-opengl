//
// Created by MTBLE on 22.01.2022.
//

#include "dependences.h"

#ifndef GLUT_TEST_SHADER_H
#define GLUT_TEST_SHADER_H

class Shader {
private:
    GLuint m_RendererID;
    std::string m_VertexFilepath;
    std::string m_FragmentFilepath;

    //Cache system for uniforms
    std::unordered_map<std::string, GLint> m_UniformLocationCache;

    static GLuint ReadShader(const std::string &vertex, const std::string& fragment);

    static GLuint CompileShader(const std::string &source, GLenum type);

    static GLuint CreateShader(const std::string &vertexShader, const std::string &fragmentShader);

    GLint GetUniformLocation(const std::string &name);
public:
    Shader(const std::string &vertexshader, const std::string &fragmentshader);

    ~Shader();

    void Bind() const;
    void Unbind() const;

    //Set uniforms
    void SetUniform4f(const std::string &name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    void SetUniform3f(const std::string &name, GLfloat v0, GLfloat v1, GLfloat v2);
    void SetUniformMatrix4fv(const std::string &name, glm::mat4 &matrix);
    void SetUniform1i(const std::string &name, GLint value);
};

#endif //GLUT_TEST_SHADER_H

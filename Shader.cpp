//
// Created by MTBLE on 22.01.2022.
//

#include "Shader.h"

Shader::Shader(const std::string &vertexshader, const std::string &fragmentshader) : m_RendererID(0),
                                                                                     m_VertexFilepath(vertexshader),
                                                                                     m_FragmentFilepath(
                                                                                             fragmentshader) {
    m_RendererID = ReadShader(vertexshader, fragmentshader);
}


Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const {
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

GLint Shader::GetUniformLocation(const std::string &name) {
    GLint location;
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        location = m_UniformLocationCache[name];
    } else {
        location = glGetUniformLocation(m_RendererID, name.c_str());
        m_UniformLocationCache[name] = location;
    }
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' does not exist" << std::endl;
    }
    return location;
}

void Shader::SetUniform4f(const std::string &name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMatrix4fv(const std::string &name, glm::mat4 &matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

GLuint Shader::ReadShader(const std::string &vertex, const std::string &fragment) {
    std::string VertexCode, FragmentCode;

    {
        std::ifstream VertexFile(vertex, std::ios::in);

        if (!VertexFile.is_open()) {
            std::cerr << "Unable to find file " << vertex << std::endl;
            return 0;
        }

        std::stringstream sstr;
        sstr << VertexFile.rdbuf();
        VertexCode = sstr.str();
        VertexFile.close();
    }

    {
        std::ifstream FragmentFile(fragment, std::ios::in);
        std::stringstream sstr;
        sstr << FragmentFile.rdbuf();
        FragmentCode = sstr.str();
        FragmentFile.close();
    }

    return CreateShader(VertexCode, FragmentCode);
}

GLuint Shader::CompileShader(const std::string &source, GLenum type) {
    GLuint result = glCreateShader(type);

    const char *src = source.c_str();

    glShaderSource(result, 1, &src, nullptr);
    glCompileShader(result);

    int status;
    glGetShaderiv(result, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        int length;
        glGetShaderiv(result, GL_INFO_LOG_LENGTH, &length);

        char *message = (char *) alloca(length);
        glGetShaderInfoLog(result, length, &length, message);

        std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex\n" : "fragment\n");
        std::cerr << source << std::endl;
        std::cerr << message << std::endl;
        glDeleteShader(result);
        return 0;
    }

    return result;
}

GLuint Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
    GLuint ProgramID = glCreateProgram();

    GLuint vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    GLuint fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(ProgramID, vs);
    glAttachShader(ProgramID, fs);
    glLinkProgram(ProgramID);
    glValidateProgram(ProgramID);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return ProgramID;
}

void Shader::SetUniform1i(const std::string &name, GLint value) {
    glUniform1i(GetUniformLocation(name), value);
}

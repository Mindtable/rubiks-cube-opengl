//
// Created by MTBLE on 22.01.2022.
//

#include "Renderer.h"

#ifndef GLUT_TEST_TEXTURE_H
#define GLUT_TEST_TEXTURE_H


class Texture {
private:
    GLuint m_RendererID;
    std::string m_Filepath;
    GLubyte *m_LocalBuffer{};

    GLint m_Width{}, m_Height{}, m_BPP{};
public:
    Texture();
    Texture(const std::string &m_filepath);

    virtual ~Texture();

    void Bind(GLuint slot = 0) const;

    void Unbind() const;

    inline GLint GetWidth() const {return m_Width;}
    inline GLint GetHeight() const {return m_Height;}

};


#endif //GLUT_TEST_TEXTURE_H

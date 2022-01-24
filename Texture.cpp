//
// Created by MTBLE on 22.01.2022.
//

#include "Texture.h"

Texture::Texture(const std::string &m_filepath) : m_Filepath(m_filepath), m_LocalBuffer(nullptr), m_Width(0),
                                                  m_Height(0), m_BPP(0), m_RendererID(0) {
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(m_filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);


    //FIXME could be broken GL_TEXTURE_2D_MULTISAMPLE
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, m_Width, m_Height, GL_TRUE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (m_LocalBuffer) {
        stbi_image_free(m_LocalBuffer);
    }
}

Texture::~Texture() {
//    std::cerr << "Destructor " << m_RendererID << std::endl;
    glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(GLuint slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture() {
    m_RendererID = 0;
}

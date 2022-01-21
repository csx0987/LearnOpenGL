#include "texture.h"
#include "glad/glad.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

// TODO mipmap
Texture::Texture(const std::string &path)
    : mRenderID(0), mWidth(0), mHeight(0), mBPP(0), mFilePath(path), mLocalBuffer(nullptr)
{
    stbi_set_flip_vertically_on_load(1); // TODO 需要解释
    mLocalBuffer = stbi_load(mFilePath.c_str(), &mWidth, &mHeight, &mBPP, 0);
    if (mLocalBuffer)
    {
        glGenTextures(1, &mRenderID);
        glBindTexture(GL_TEXTURE_2D, mRenderID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format = Texture::GetBPPEnum(mBPP);
        glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, mLocalBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(mLocalBuffer);
        mLocalBuffer = nullptr;
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &mRenderID);
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, mRenderID);
}

void Texture::UnBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::GetBPPEnum(unsigned int v)
{
    GLenum format;
    if (v == 1)
        format = GL_RED;
    else if (v == 3)
        format = GL_RGB;
    else if (v == 4)
        format = GL_RGBA;
    return format;
}

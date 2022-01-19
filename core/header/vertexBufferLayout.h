#pragma once

#include "glad/glad.h"
#include <vector>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    bool normalized;

    static unsigned int GetTypeSize(unsigned int type)
    {
        unsigned int size = 0;
        switch (type)
        {
        case GL_FLOAT:
            size = sizeof(GLfloat);
            break;

        case GL_UNSIGNED_INT:
            size = sizeof(GLuint);
            break;

        default:
            break;
        }
        return size;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> mElements;
    unsigned int mStride;

public:
    VertexBufferLayout();
    ~VertexBufferLayout();

    void Push(unsigned int type, unsigned int count, bool normalized);
    inline unsigned int GetStride() const { return mStride; }
    inline const std::vector<VertexBufferElement> &GetElements() const { return mElements; }
};

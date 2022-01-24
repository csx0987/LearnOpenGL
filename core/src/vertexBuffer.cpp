#include "vertexBuffer.h"
#include "glad/glad.h"
#include "gFunction.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    GLCall(glGenBuffers(1, &mRenderID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRenderID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &mRenderID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRenderID));
}

void VertexBuffer::UnBind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

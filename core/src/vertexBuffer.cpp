#include "vertexBuffer.h"
#include "glad/glad.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    glGenBuffers(1, &mRenderID);
    glBindBuffer(GL_ARRAY_BUFFER, mRenderID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &mRenderID);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, mRenderID);
}

void VertexBuffer::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

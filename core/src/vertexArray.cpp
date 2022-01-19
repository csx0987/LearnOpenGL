#include "vertexArray.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &mRenderID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &mRenderID);
}

void VertexArray::Bind() const
{
    glBindVertexArray(mRenderID);
}

void VertexArray::UnBind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &vbl)
{
    Bind();
    vb.Bind();
    const auto &elements = vbl.GetElements();
    unsigned int stride = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const VertexBufferElement &element = elements[i];
        glVertexAttribPointer(i, element.count, element.type, element.normalized, vbl.GetStride(), (void *)(stride));
        glEnableVertexAttribArray(i);

        stride += element.count * VertexBufferElement::GetTypeSize(element.type);
    }
}

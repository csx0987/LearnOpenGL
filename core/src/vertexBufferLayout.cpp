#include "vertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout()
    : mStride(0)
{
}

VertexBufferLayout::~VertexBufferLayout()
{
}

void VertexBufferLayout::Push(unsigned int type, unsigned int count, bool normalized)
{
    mElements.push_back({type, count, normalized});
    mStride += count * VertexBufferElement::GetTypeSize(type);
}
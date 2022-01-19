#pragma once

class VertexArray
{
private:
    unsigned int mRenderID;

public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void UnBind() const;

    void AddBuffer(const class VertexBuffer &vb, const class VertexBufferLayout &vbl);
};

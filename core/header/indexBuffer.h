#pragma once

#pragma once

class IndexBuffer
{
private:
    unsigned int mRenderID;
    unsigned int mCount;

public:
    IndexBuffer(const unsigned int *data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void UnBind() const;

    inline unsigned int GetCount() const { return mCount; }
};

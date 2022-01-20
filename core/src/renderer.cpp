#include "renderer.h"
#include "glad/glad.h"
#include "vertexArray.h"
#include "indexBuffer.h"
#include "shader.h"

Renderer::Renderer()
	: mBgColor(0.0f, 0.0f, 0.0f, 0.0f)
{
}

Renderer::~Renderer()
{
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
	va.Bind();
	ib.Bind();
	shader.use();
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::Clear() const
{
	glClearColor(mBgColor.x * mBgColor.w, mBgColor.y * mBgColor.w, mBgColor.z * mBgColor.w, mBgColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

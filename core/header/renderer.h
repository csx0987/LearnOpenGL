#pragma once
#include <glm/glm.hpp>

class VertexArray;
class IndexBuffer;
class Shader;

class Renderer
{
private:
	glm::vec4 mBgColor;

public:
	Renderer();
	~Renderer();

	void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader);
	void Clear() const;

	// get set
	inline const glm::vec4& GetBgColor() const { return mBgColor; }
	inline void SetBgColor(const glm::vec4& color) { mBgColor = color; }
};
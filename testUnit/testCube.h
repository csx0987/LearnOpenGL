#pragma once
#include "baseTest.h"
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "renderer.h"

namespace Test
{
	class TestCube : public BaseTest
	{
	public:
		TestCube();
		~TestCube() override;

        void OnRender() override;
        void OnImGuiRender() override;

	private:
        Shader* pCubeShader;
        Shader* pLightCubeShader;
        VertexArray* pCubeVertexArray;
        VertexBuffer* pCubeVertexBuffer;
        IndexBuffer* pCubeIndex;
        VertexArray* pLightVertexArray;
        Texture* pDiffuseMap;
		Texture* pSpecularMap;
		Texture* pEmissiveMap;
        Camera* pCamera;
        Renderer* pRenderer;
    };
}
#pragma once
#include "baseTest.h"
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "constant.h"
#include "renderer.h"

namespace Test
{
	class TestCube : public BaseTest
	{
	public:
		TestCube();
		~TestCube() override;

        void ProcessInput(GLFWwindow* window, float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

        void MouseCallback(GLFWwindow* window, double xpos, double ypos);

        void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

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

        bool firstMouse = true;
        float lastX = Constant::SCR_WIDTH / 2.0f;
        float lastY = Constant::SCR_HEIGHT / 2.0f;
    };
}
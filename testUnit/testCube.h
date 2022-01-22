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
		~TestCube();

        void OnRender() override;
        void OnImGuiRender();

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


#if defined(__APPLE__)
        const char* cubeVsPath = "../shaders/cube.vs";
        const char* cubeFsPath = "../shaders/cube.fs";
        const char* lightCubeVsPath = "../shaders/light_cube.vs";
        const char* lightCubeFsPath = "../shaders/light_cube.fs";
        const char* diffuseMapPath = "../resources/textures/container2.png";
        const char* specularMapPath = "../resources/textures/container2_specular.png";
        const char* emissiveMapPath = "../resources/textures/matrix.jpeg";
#else
        const char* cubeVsPath = "../../../shaders/cube.vs";
        const char* cubeFsPath = "../../../shaders/cube.fs";
        const char* lightCubeVsPath = "../../../shaders/light_cube.vs";
        const char* lightCubeFsPath = "../../../shaders/light_cube.fs";
        const char* diffuseMapPath = "../../../resources/textures/container2.png";
        const char* specularMapPath = "../../../resources/textures/container2_specular.png";
        const char* emissiveMapPath = "../../../resources/textures/matrix.jpeg";
#endif
    };
}
#pragma once
#include "baseTest.h"
#include "shader.h"
#include "model.h"
#include "camera.h"

namespace Test
{
	class TestModel : public BaseTest
	{
	public:
		TestModel();
		~TestModel() override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Shader* pShader;
		Model* pModel;
		Camera* pCamera;

		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model;
	};
}
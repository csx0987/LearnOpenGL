#include "testModel.h"
#include "constant.h"

namespace Test
{
	Test::TestModel::TestModel()
		: pShader(nullptr), pModel(nullptr), pCamera(nullptr)
	{
		pShader = new Shader(Constant::modelVsPath, Constant::modelFsPath);
		pModel = new Model(Constant::objPath);
		pCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	}

	Test::TestModel::~TestModel()
	{
		if (pShader)
		{
			printf("delete pShader\n");
			delete pShader;
			pShader = nullptr;
		}
		
		if (pModel)
		{
			printf("delete pModel\n");
			delete pModel;
			pModel = nullptr;
		}

		if (pCamera)
		{
			printf("delete pCamera\n");
			delete pCamera;
			pCamera = nullptr;
		}
	}

	void Test::TestModel::OnUpdate(float deltaTime)
	{
		
	}

	void Test::TestModel::OnRender()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		pShader->use();
		projection = glm::perspective(glm::radians(pCamera->Zoom), (float)Constant::SCR_WIDTH / (float)Constant::SCR_HEIGHT, 0.1f, 100.0f);
		view = pCamera->GetViewMatrix();
		pShader->setMat4("projection", projection);
		pShader->setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.1f));     // it's a bit too big for our scene, so scale it down
		pShader->setMat4("model", model);
		pModel->Draw(*pShader);
	}

	void Test::TestModel::OnImGuiRender()
	{
		//BaseTest::OnImGuiRender();
	}
}

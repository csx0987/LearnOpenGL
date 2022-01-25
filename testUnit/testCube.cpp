#include "testCube.h"
#include "vertexBufferLayout.h"
#include "constant.h"

namespace Test
{
	float shininessFact = 36.0f;
	// lighting
	struct Light
	{
		glm::vec3 color;
		glm::vec3 position;
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float constant;
		float linear;
		float quadratic;
		float cutOff;
		float outerCutOff;
	};

	struct BaseLight
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	struct DirLight : BaseLight // 平行光，暂只有一个
	{
		glm::vec3 direction;
	};

	struct PointLight : BaseLight
	{
		glm::vec3 position;

		float constant;
		float linear;
		float quadratic;
	};

	struct SpotLight : BaseLight
	{
		glm::vec3 position;
		glm::vec3 direction;

		float cutOff;
		float outerCutOff;

		float constant;
		float linear;
		float quadratic;
	};

	// box
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f) };

	glm::vec3 pointLightPosition[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f) };

	glm::vec3 pointLightAmbient[] = {
		glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3(0.05f, 0.05f, 0.05f),
	};

	glm::vec3 pointLightDiffuse[] = {
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(0.8f, 0.8f, 0.8f),
	};

	glm::vec3 pointLightSpecular[] = {
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
	};

	float pointLightConstant[] = {
		1.0f, 1.0f, 1.0f };

	float pointLightLinear[] = {
		0.09f, 0.09f, 0.09f };

	float pointLightQuadratic[] = {
		0.032f, 0.032f, 0.032f };

	Light light;
	DirLight dirLight;
	SpotLight spotLight;
	PointLight pointLights[3];

	Test::TestCube::TestCube()
        : pCubeShader(nullptr), pLightCubeShader(nullptr), pCubeVertexArray(nullptr), pCubeVertexBuffer(nullptr), pCubeIndex(nullptr)
	{
        float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
        };
        unsigned int indices[] = {
            0, 1, 2,
            3, 4, 5,

            6, 7, 8,
            9, 10, 11,

            12, 13, 14,
            15, 16, 17,

            18, 19, 20,
            21, 22, 23,

            24, 25, 26,
            27, 28, 29,

            30, 31, 32,
            33, 34, 35 };
	    
        pCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
        // init shader
        pCubeShader = new Shader(Constant::cubeVsPath, Constant::cubeFsPath);
        pLightCubeShader = new Shader(Constant::lightCubeVsPath, Constant::lightCubeFsPath);

        // cube
        pCubeVertexArray = new VertexArray();
        pCubeVertexBuffer = new VertexBuffer(vertices, sizeof(vertices));
        pCubeIndex = new IndexBuffer(indices, sizeof(indices) / sizeof(unsigned int));
		VertexBufferLayout cubeVertexBufferLayout;

		cubeVertexBufferLayout.Push(GL_FLOAT, 3, false); // 顶点位置
		cubeVertexBufferLayout.Push(GL_FLOAT, 3, true);  // 法线
		cubeVertexBufferLayout.Push(GL_FLOAT, 2, false); // uv

        pCubeVertexArray->AddBuffer(*pCubeVertexBuffer, cubeVertexBufferLayout);
        pCubeIndex->Bind();

        // light
        pLightVertexArray = new VertexArray();
        VertexBufferLayout lightVertexBufferLayout;
		lightVertexBufferLayout.Push(GL_FLOAT, 3, false); // 顶点位置
		lightVertexBufferLayout.Push(GL_FLOAT, 3, false);
		lightVertexBufferLayout.Push(GL_FLOAT, 2, false);

        pLightVertexArray->AddBuffer(*pCubeVertexBuffer, lightVertexBufferLayout);
        pCubeIndex->Bind();

        glBindVertexArray(0);

        // Texture
		pDiffuseMap = new Texture(Constant::diffuseMapPath);
		pSpecularMap = new Texture(Constant::specularMapPath);
		pEmissiveMap = new Texture(Constant::emissiveMapPath);

		
		light.color = glm::vec3(1.0f, 1.0f, 1.0f);
		light.position = glm::vec3(1.2f, 1.0f, 2.0f);
		light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		light.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
		light.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
		light.constant = 1.0f;
		light.linear = 0.09f;
		light.quadratic = 0.032f;
		light.cutOff = glm::cos(glm::radians(12.5f));
		light.outerCutOff = glm::cos(glm::radians(17.5f));

		
		dirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
		dirLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
		dirLight.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
		dirLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);

		
		for (unsigned int i = 0; i < 3; i++)
		{
			pointLights[i].position = pointLightPosition[i];
			pointLights[i].ambient = pointLightAmbient[i];
			pointLights[i].diffuse = pointLightDiffuse[i];
			pointLights[i].specular = pointLightSpecular[i];
			pointLights[i].constant = pointLightConstant[i];
			pointLights[i].linear = pointLightLinear[i];
			pointLights[i].quadratic = pointLightQuadratic[i];
		}

		
		spotLight.position = pCamera->Position;
		spotLight.direction = pCamera->Front;
		spotLight.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		spotLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		spotLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
		spotLight.constant = 1.0f;
		spotLight.linear = 0.09f;
		spotLight.quadratic = 0.032f;
		spotLight.cutOff = glm::cos(glm::radians(12.5f));
		spotLight.outerCutOff = glm::cos(glm::radians(15.0f));

		pRenderer = new Renderer();
    }

	Test::TestCube::~TestCube()
	{
		delete pCubeShader;
		delete pLightCubeShader;
		delete pCubeVertexArray;
		delete pCubeVertexBuffer;
		delete pCubeIndex;
		delete pLightVertexArray;
		delete pDiffuseMap;
		delete pSpecularMap;
		delete pEmissiveMap;
		delete pCamera;
		delete pRenderer;
	}

	void Test::TestCube::ProcessInput(GLFWwindow* window, float deltaTime)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			pCamera->ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			pCamera->ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			pCamera->ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			pCamera->ProcessKeyboard(RIGHT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{
			if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				glfwSetCursorPosCallback(window, nullptr);
				glfwSetScrollCallback(window, nullptr);
				firstMouse = true;
			}
			else
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				firstMouse = true;
			}
		}
	}

	void Test::TestCube::OnRender()
	{
		pRenderer->Clear();

		// Rendering
		pCubeShader->use();

		glm::mat4 projection = glm::perspective(glm::radians(pCamera->Zoom), (float)Constant::SCR_WIDTH / (float)Constant::SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = pCamera->GetViewMatrix();
		pCubeShader->setMat4("projection", projection);
		pCubeShader->setMat4("view", view);

		pCubeShader->setVec3("viewPos", pCamera->Position);

		// material
		pCubeShader->setInt("material.diffuse", 0);
		pCubeShader->setInt("material.specular", 1);
		pCubeShader->setInt("material.emissive", 2);
		pCubeShader->setFloat("material.shininess", shininessFact);

		spotLight.position = pCamera->Position;
		spotLight.direction = pCamera->Front;

		// light
		// Direction Light
		pCubeShader->setVec3("dirLight.direction", dirLight.direction);
		pCubeShader->setVec3("dirLight.ambient", dirLight.ambient);
		pCubeShader->setVec3("dirLight.diffuse", dirLight.diffuse);
		pCubeShader->setVec3("dirLight.specular", dirLight.specular);

		// Point Light
		const char *attr1 = "position";
		const char *attr2 = "ambient";
		const char *attr3 = "diffuse";
		const char *attr4 = "specular";
		const char *attr5 = "constant";
		const char *attr6 = "linear";
		const char *attr7 = "quadratic";
		char prefix[100]; // temp;
		for (unsigned int i = 0; i < 3; i++)
		{
		    sprintf(prefix, "%s%d%s", "pointLights[", i, "].");
		    char temp[100];

		    sprintf(temp, "%s%s", prefix, attr1);
			pCubeShader->setVec3(temp, pointLights[i].position);
		    sprintf(temp, "%s%s", prefix, attr2);
			pCubeShader->setVec3(temp, pointLights[i].ambient);
		    sprintf(temp, "%s%s", prefix, attr3);
			pCubeShader->setVec3(temp, pointLights[i].diffuse);
		    sprintf(temp, "%s%s", prefix, attr4);
			pCubeShader->setVec3(temp, pointLights[i].specular);
		    sprintf(temp, "%s%s", prefix, attr5);
			pCubeShader->setFloat(temp, pointLights[i].constant);
		    sprintf(temp, "%s%s", prefix, attr6);
			pCubeShader->setFloat(temp, pointLights[i].linear);
		    sprintf(temp, "%s%s", prefix, attr7);
			pCubeShader->setFloat(temp, pointLights[i].quadratic);
		}

		// Spot Light
		pCubeShader->setVec3("spotLight.position", spotLight.position);
		pCubeShader->setVec3("spotLight.direction", spotLight.direction);
		pCubeShader->setVec3("spotLight.ambient", spotLight.ambient);
		pCubeShader->setVec3("spotLight.diffuse", spotLight.diffuse);
		pCubeShader->setVec3("spotLight.specular", spotLight.specular);
		pCubeShader->setFloat("spotLight.constant", spotLight.constant);
		pCubeShader->setFloat("spotLight.linear", spotLight.linear);
		pCubeShader->setFloat("spotLight.quadratic", spotLight.quadratic);
		pCubeShader->setFloat("spotLight.cutOff", spotLight.cutOff);
		pCubeShader->setFloat("spotLight.outerCutOff", spotLight.outerCutOff);

		// texture
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, diffuseMap);
		pDiffuseMap->Bind(0);

		// glActiveTexture(GL_TEXTURE1);
		// glBindTexture(GL_TEXTURE_2D, specularMap);
		pSpecularMap->Bind(1);

		// glActiveTexture(GL_TEXTURE2);
		// glBindTexture(GL_TEXTURE_2D, emissiveMap);
		pEmissiveMap->Bind(2);

		// cubeVertexArray.Bind();

		for (unsigned int i = 0; i < 10; i++)
		{
		    glm::mat4 model = glm::mat4(1.0f);
		    model = glm::translate(model, cubePositions[i]);
		    float angle = 20.0f * i;
		    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			pCubeShader->setMat4("model", model);
		    glm::mat4 invModel = glm::inverse(model);
			pCubeShader->setMat4("invModel", invModel);

		    // glDrawArrays(GL_TRIANGLES, 0, 36);
		    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		    pRenderer->Draw(*pCubeVertexArray, *pCubeIndex, *pCubeShader);
		}

		// glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		pLightCubeShader->use();
		pLightCubeShader->setMat4("projection", projection);
		pLightCubeShader->setMat4("view", view);

		// lightVertexArray.Bind();
		for (unsigned int i = 0; i < 3; i++)
		{
		    glm::mat4 model = glm::mat4(1.0f);
		    model = glm::translate(model, pointLights[i].position);
		    model = glm::scale(model, glm::vec3(0.2f));
			pLightCubeShader->setMat4("model", model);
		    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		    pRenderer->Draw(*pLightVertexArray, *pCubeIndex, *pLightCubeShader);
		}
	}

	void Test::TestCube::OnImGuiRender()
	{
	}

	void Test::TestCube::MouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		lastX = xpos;
		lastY = ypos;

		pCamera->ProcessMouseMovement(xoffset, yoffset);
	}

	void Test::TestCube::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		pCamera->ProcessMouseScroll(yoffset);
	}
}

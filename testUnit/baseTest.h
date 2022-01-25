#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Test
{
	class BaseTest
	{
	public:
		BaseTest() {}
		virtual ~BaseTest() {}

		virtual void ProcessInput(GLFWwindow* window, float deltaTime) {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		virtual void OnImGuiRender() {}
	};
}
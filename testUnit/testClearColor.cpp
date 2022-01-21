#include "testClearColor.h"
#include "glad/glad.h"
#include "imgui.h"

Test::TestClearColor::TestClearColor()
	: mClearColor{0.2f, 0.3f, 0.8f, 1.0f}
{
}

Test::TestClearColor::~TestClearColor()
{
}

void Test::TestClearColor::OnUpdate(float deltaTime)
{
}

void Test::TestClearColor::OnRender()
{
	glClearColor(mClearColor[0], mClearColor[1], mClearColor[2], mClearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Test::TestClearColor::OnImGuiRender()
{
	ImGui::ColorEdit4("clear color", mClearColor);
}

#include "testMenu.h"
#include "imgui.h"

Test::TestMenu::TestMenu(BaseTest* &current)
	: mCurrent(current)
{
}

Test::TestMenu::~TestMenu()
{
	if (mCurrent && mCurrent != this)
	{
		delete mCurrent;
		mCurrent = nullptr;
	}
}

void Test::TestMenu::OnImGuiRender()
{
	for (auto test : mTests)
	{
		if (ImGui::Button(test.first.c_str()))
			mCurrent = test.second();
	}
}

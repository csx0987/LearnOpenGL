#pragma once
#include "baseTest.h"

namespace Test
{
	class TestClearColor : public BaseTest
	{
	public:
		TestClearColor();
		~TestClearColor() override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float mClearColor[4];
	};
}
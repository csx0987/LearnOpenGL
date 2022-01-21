#pragma once
#include "baseTest.h"

#include <vector>
#include <functional>
#include <string>


namespace Test
{
	class TestMenu : public BaseTest
	{
	public:
		TestMenu(BaseTest* &current);
		~TestMenu();

		void OnImGuiRender() override;

		template <typename T>
		void RegistTest(const std::string& name)
		{
			mTests.push_back(std::make_pair(name, []() { return new T(); }));
		}

		inline BaseTest* GetCurrent() { return mCurrent; }
		inline void SetCurrent(BaseTest* current) { mCurrent = current; }

	private:
		BaseTest* &mCurrent;
		std::vector<std::pair<std::string, std::function<BaseTest* ()>>> mTests;
	};
}
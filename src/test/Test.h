#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>

namespace test {

	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);
		~TestMenu();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		
		template<typename T, typename... Params>
		void RegisterTest(const std::string& name, Params... params)
		{
			std::cout << "Register test " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, [=]() { return new T(params...); }));
		}

		/* // no need to use explicit specialization
		template<>
		void RegisterTest<class TestDrawTexture>(const std::string& name, std::string& path, int sizeX, int sizeY);*/

	private:

		/* reference to external translation unit pointer on current test */
		Test*& m_CurrentTest;

		/* store lambdas which creates test instances*/
		std::vector < std::pair<std::string, std::function<Test* ()>>> m_Tests;  

	};


}
#include "Test.h"
#include "imgui/imgui.h"

namespace test {

	TestMenu::TestMenu(Test*& currentTestPointer)
		: m_CurrentTest(currentTestPointer)
	{

	}

	TestMenu::~TestMenu()
	{
	}

	void TestMenu::OnUpdate(float deltaTime)
	{
	}

	void TestMenu::OnRender()
	{
	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				m_CurrentTest = test.second();
			}
		}
	}
	
	/*  // no need to explicit use specialization
	template<>
	void TestMenu::RegisterTest<TestDrawTexture>(const std::string& name, std::string& path, int sizeX, int sizeY)
	{
		std::cout << "Register test " << name << std::endl;

		m_Tests.push_back(std::make_pair(name, [&]() { return new TestDrawTexture(path.c_str(), sizeX, sizeY); }));
	}
	*/
}
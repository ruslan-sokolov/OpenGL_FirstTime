#pragma once

#include "Test.h"

#include "glm/glm.hpp"

#include <memory>

class IndexBuffer;
class VertexBuffer;
class VertexArray;
class Shader;
class Texture;

namespace test {

	class TestDrawTexture : public Test
	{
	public:
		TestDrawTexture(const char* path = "res/textures/UE_Logo.png", unsigned int sizeX = 100, unsigned int sizeY = 100);
		~TestDrawTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		
	private:
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<VertexArray> m_VAO;

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj;
		glm::mat4 m_View;
		glm::vec3 m_Translation;
		glm::vec4 m_Color;
	};
}


#pragma once

#include "Test.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

#include "glm/glm.hpp"

namespace test {

	constexpr const char* defaultShader = "res/shaders/Basic.shader";
	constexpr float whiteColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	constexpr unsigned int indicies[6] = { 0, 1, 2, 2, 3, 0 };

	class TestDrawTexture : public Test
	{
	public:
		TestDrawTexture(const char* path = "res/textures/UE_Logo.png", unsigned int sizeX = 100, unsigned int sizeY = 100);
		~TestDrawTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		
	private:
		float m_Positions[16];
		Shader shader;
		Texture texture;

		IndexBuffer ib;
		VertexBufferLayout vb_layout;
		VertexBuffer vb;
		VertexArray va;

		glm::vec4 m_Color;
		glm::vec3 m_Translation;
	};
}


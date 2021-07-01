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

	class TestBatchRender : public Test
	{
	public:
		TestBatchRender(unsigned int posX_1 = 200, unsigned int posY_1 = 200, unsigned int size_1 = 100, 
			            unsigned int posX_2 = 400, unsigned int posY_2 = 200, unsigned int size_2 = 50);
		~TestBatchRender();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		
	private:
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<VertexArray> m_VAO;

		std::unique_ptr<Shader> m_Shader;

		glm::mat4 m_Proj;
		glm::mat4 m_View;
		glm::vec3 m_Translation;
		glm::vec4 m_Color;
	};
}


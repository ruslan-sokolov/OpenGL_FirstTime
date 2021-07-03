#pragma once

#include "Test.h"

#include "glm/glm.hpp"

#include <memory>

class Shader;
class Texture;

namespace test {

	class TestBatchTexture : public Test
	{
	public:
		TestBatchTexture();
		~TestBatchTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture_0;
		std::unique_ptr<Texture> m_Texture_1;

		glm::vec3 m_Translation;
		glm::vec3 m_Scale;
		float m_Rotation;
		glm::vec4 m_Color;

		unsigned int m_QuadVA;
		unsigned int m_QuadVB;
		unsigned int m_QuadIB;

		static const glm::vec3 rotateXYPlane;
		static const glm::mat4 identityMatrix;

	};
}


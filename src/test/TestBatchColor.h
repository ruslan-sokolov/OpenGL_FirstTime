#pragma once

#include "Test.h"

#include "glm/glm.hpp"

#include <memory>

class Shader;

namespace test {

	class TestBatchColor : public Test
	{
	public:
		TestBatchColor();
		~TestBatchColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		
	private:
		std::unique_ptr<Shader> m_Shader;
		glm::vec3 m_Translation;
		glm::vec3 m_Scale;
		float m_Rotation;

		unsigned int m_QuadVA;
		unsigned int m_QuadVB;
		unsigned int m_QuadIB;

		static const glm::vec3 rotateXYPlane;
		static const glm::mat4 identityMatrix;

	};
}


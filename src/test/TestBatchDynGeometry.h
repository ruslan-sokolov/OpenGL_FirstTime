#pragma once

#include "Test.h"

#include "glm/glm.hpp"

#include <memory>
#include <array>
#include <vector>

class Shader;
class Texture;

namespace test {

	struct Vec3
	{
		float x, y, z;
	};

	struct Vec2
	{
		float x, y;
	};

	struct Vertex
	{
		Vec3 Position;
		Vec2 TexCoords;
		float TexId;
	};

	constexpr int MAX_VERT_NUM = 100;

	class TestBatchDynGeometry : public Test
	{
	public:
		TestBatchDynGeometry();
		~TestBatchDynGeometry();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::vector<Vertex> m_Vertices;
		static std::array<Vertex, 4> CreateQuad(float x, float size_x, float y, float size_y, uint32_t textureId);

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


#include "TestBatchDynGeometry.h"

#include "GL/glew.h"

#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Renderer.h"

#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"

#include <string>

using namespace test;

const glm::vec3 TestBatchDynGeometry::rotateXYPlane = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::mat4 TestBatchDynGeometry::identityMatrix = glm::mat4(1.0f);


TestBatchDynGeometry::TestBatchDynGeometry()
	: m_Shader(std::make_unique<Shader>("res/shaders/BatchTexture.shader")),
	m_Texture_0(std::make_unique<Texture>("res/textures/UE_Logo.png")),
	m_Texture_1(std::make_unique<Texture>("res/textures/sun_pic.png")),
	m_Translation(glm::vec3(400.0f, 200.0f, 0.0f)),
	m_Scale(glm::vec3(100.0f, 100.0f, 1.0f)),
	m_Rotation(0.0f),
	m_Color(1.0f, 1.0f, 1.0f, 1.0f)
{
	glGenVertexArrays(1, &m_QuadVA);  // generate vertex array
	glBindVertexArray(m_QuadVA);

	glGenBuffers(1, &m_QuadVB);  // generate dynamic vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);  // use
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MAX_VERT_NUM, nullptr, GL_DYNAMIC_DRAW);  // alloc

	glEnableVertexAttribArray(0);  // vertex layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexId));

	uint32_t indicies[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};

	glGenBuffers(1, &m_QuadIB);  // generate index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);  // use
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW); // alloc

	// bind shader for vertex color draw (we need to bind it here to set uniform)
	m_Shader->Bind();
	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
	m_Shader->SetUniformMat4f("u_ViewProj", proj * view);
	glm::mat4 scaleMatrix = glm::scale(identityMatrix, m_Scale);
	glm::mat4 translationMatrix = glm::translate(identityMatrix, m_Translation);
	glm::mat4 model = translationMatrix * scaleMatrix;
	m_Shader->SetUniformMat4f("u_Transform", model);
	m_Shader->SetUniform4f("u_Color", (float*)&m_Color);

	// create texture and set shader u_Texture uniform value
	m_Texture_0->Bind(0);
	m_Texture_1->Bind(1);
	int textures[2]{ 0, 1 };
	m_Shader->SetUniform1iv("u_Textures", 2, textures);  // specify textures location

	// create two quads
	m_Vertices.reserve(100);
	auto q0 = CreateQuad(-1.5f, 1.0f, -0.5f, 1.0f, 0);
	auto q1 = CreateQuad(0.5f, 1.0f, -0.5f, 1.0f, 1);
	
	m_Vertices.insert(std::end(m_Vertices), std::begin(q0), std::end(q0));
	m_Vertices.insert(std::end(m_Vertices), std::begin(q1), std::end(q1));
	
	//memcpy(m_Vertices.data(), q0.data(), q0.size() * sizeof(Vertex));
	//memcpy(m_Vertices.data() + q0.size(), q1.data(), q1.size() * sizeof(Vertex));
}

TestBatchDynGeometry::~TestBatchDynGeometry()
{
	glDeleteBuffers(1, &m_QuadVB);
	glDeleteBuffers(1, &m_QuadIB);
	glDeleteVertexArrays(1, &m_QuadVA);
	glBindVertexArray(0);
}

void TestBatchDynGeometry::OnUpdate(float deltaTime)
{
	// Set dynamic vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_Vertices.size() * sizeof(Vertex), (const void*)m_Vertices.data());
}

void TestBatchDynGeometry::OnRender()
{
	glDrawElements(GL_TRIANGLES, 2 * 6, GL_UNSIGNED_INT, NULL);
}

void TestBatchDynGeometry::OnImGuiRender()
{
	ImGui::SliderFloat3("Translation", &m_Translation.x, 0.f, 960.f);
	ImGui::SliderFloat3("Scale", &m_Scale.x, 1.0f, 1000.f);
	ImGui::SliderFloat("Rotate", &m_Rotation, -180.0f, 180.0f);
	ImGui::ColorEdit4("Color", (float*)&m_Color);

	for (int i = 0; i < m_Vertices.size(); ++i)
	{
		std::string name = "Vertex" + std::to_string(i);
		Vertex& vert = m_Vertices[i];
		ImGui::SliderFloat2(name.c_str(), &vert.Position.x, -5.0f, 5.0f);
	}

	glm::mat4 scaleMatrix = glm::scale(identityMatrix, m_Scale);
	glm::mat4 translationMatrix = glm::translate(identityMatrix, m_Translation);
	glm::mat4 rotationMatrix = glm::rotate(identityMatrix, glm::radians(m_Rotation), rotateXYPlane);

	glm::mat4 model = translationMatrix * rotationMatrix * scaleMatrix;

	m_Shader->SetUniform4f("u_Color", (float*)&m_Color);
	m_Shader->SetUniformMat4f("u_Transform", model);
}

std::array<Vertex, 4> TestBatchDynGeometry::CreateQuad(float x, float size_x, float y, float size_y, uint32_t textureId)
{
	Vertex v0;
	v0.Position = { x, y,  0.0f };
	v0.TexCoords = { 0.0f, 0.0f };
	v0.TexId = textureId;

	Vertex v1;
	v1.Position = { x + size_x, y,  0.0f };
	v1.TexCoords = { 1.0f, 0.0f };
	v1.TexId = textureId;

	Vertex v2;
	v2.Position = { x + size_x,  y + size_y,  0.0f };
	v2.TexCoords = { 1.0f, 1.0f };
	v2.TexId = textureId;

	Vertex v3;
	v3.Position = { x,  y + size_y,  0.0f };
	v3.TexCoords = { 0.0f, 1.0f };
	v3.TexId = textureId;

	return { v0, v1, v2, v3 };
}

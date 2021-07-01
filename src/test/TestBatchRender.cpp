#include "TestBatchRender.h"

#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Renderer.h"

#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"

test::TestBatchRender::TestBatchRender(unsigned int posX_1, unsigned int posY_1, unsigned int size_1,
	                                   unsigned int posX_2, unsigned int posY_2, unsigned int size_2)
	: m_Color(1.0f, 1.0f, 1.0f, 1.0f),
	m_Translation(0.0f, 0.0f, 0.0f),
	m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
	m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))
{
	// vertex array
	m_VAO = std::make_unique<VertexArray>();

	// vertex buffer
	
	float positions[] = {
		posX_1,          posY_1,
		posX_1 + size_1, posY_1,
		posX_1 + size_1, posY_1 + size_1,
		posX_1,          posY_1 + size_1,

		posX_2,          posY_2,
		posX_2 + size_2, posY_2,
		posX_2 + size_2, posY_2 + size_2,
		posX_2,          posY_2 + size_2
	};

	unsigned int vertexBufferSize = sizeof(positions) / sizeof(float) * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	m_VertexBuffer = std::make_unique<VertexBuffer>(positions, vertexBufferSize);

	// vertex array bind  buffer with proper layout
	VertexBufferLayout layout;
	layout.Push<float>(2);
	m_VAO->AddBuffer(*m_VertexBuffer, layout);

	// declare index buffer
	unsigned int indicies[] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4
	};
	m_IndexBuffer = std::make_unique<IndexBuffer>(indicies, sizeof(indicies) / sizeof(unsigned int));

	// create and bind default shader for texture draw (we need to bind it here to set uniform)
	m_Shader = std::make_unique<Shader>("res/shaders/BasicColor.shader");
	m_Shader->Bind();
	m_Shader->SetUniform4f("u_Color", (float*)&m_Color);

	// 16:9 screen format projection mvp
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
	m_Shader->SetUniformMat4f("u_MVP", m_Proj * m_View * model);
}

test::TestBatchRender::~TestBatchRender()
{
}

void test::TestBatchRender::OnUpdate(float deltaTime)
{
}

void test::TestBatchRender::OnRender()
{
	Renderer::Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
}

void test::TestBatchRender::OnImGuiRender()
{
	ImGui::ColorEdit4("Texture Color", (float*)&m_Color);
	m_Shader->SetUniform4f("u_Color", (float*)&m_Color);

	ImGui::SliderFloat3("Texture Translation", &m_Translation.x, 0.f, 960.f);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
	m_Shader->SetUniformMat4f("u_MVP", m_Proj * m_View * model);
}

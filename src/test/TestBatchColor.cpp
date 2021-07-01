#include "TestBatchColor.h"

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

test::TestBatchColor::TestBatchColor()
	: m_Shader(std::make_unique<Shader>("res/shaders/VertexColor.shader"))
{

	float vertices[] = {
		-1.5f, -0.5f,  0.0f,   0.18f, 0.6f, 0.96f, 1.0f,
		-0.5f, -0.5f,  0.0f,   0.18f, 0.6f, 0.96f, 1.0f,
		-0.5f, -0.5f,  0.0f,   0.18f, 0.6f, 0.96f, 1.0f,
		-0.5f, -0.5f,  0.0f,   0.18f, 0.6f, 0.96f, 1.0f,

		 0.5f, -0.5f,  0.0f,   1.0f, 0.93f, 0.24f, 1.0f,
		 1.5f, -0.5f,  0.0f,   1.0f, 0.93f, 0.24f, 1.0f,
		 1.5f,  0.5f,  0.0f,   1.0f, 0.93f, 0.24f, 1.0f,
		 0.5f,  0.5f,  0.0f,   1.0f, 0.93f, 0.24f, 1.0f,
	};

	glGenVertexArrays(1, &m_QuadVA);  // generate vertex array
	glBindVertexArray(m_QuadVA);

	glGenBuffers(1, &m_QuadVB);  // generate vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void*)(sizeof(float) * 3));

	uint32_t indicies[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};

	glGenBuffers(1, &m_QuadIB);  // generate index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	// bind shader for vertex color draw (we need to bind it here to set uniform)
	m_Shader->Bind();
	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	m_Shader->SetUniformMat4f("u_ViewProj", proj * view);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
	m_Shader->SetUniformMat4f("u_Transform", model);
}

test::TestBatchColor::~TestBatchColor()
{
	glDeleteBuffers(1, &m_QuadVB);
	glDeleteBuffers(1, &m_QuadIB);
	glDeleteVertexArrays(1, &m_QuadVA);
	glBindVertexArray(0);
}

void test::TestBatchColor::OnUpdate(float deltaTime)
{
}

void test::TestBatchColor::OnRender()
{
	glDrawElements(GL_TRIANGLES, 2 * 6, GL_UNSIGNED_INT, NULL);
}

void test::TestBatchColor::OnImGuiRender()
{
	ImGui::SliderFloat3("Translation", &m_Translation.x, 0.f, 960.f);
	glm::mat4 model = glm::translate(glm::mat4(2323.f), m_Translation);
	m_Shader->SetUniformMat4f("u_Transform", model);
}

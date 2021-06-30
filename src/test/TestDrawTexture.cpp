#include "TestDrawTexture.h"

#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Renderer.h"

#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"

test::TestDrawTexture::TestDrawTexture(const char* path, unsigned int sizeX, unsigned int sizeY)
	: m_Color(1.0f, 1.0f, 1.0f, 1.0f),
	m_Translation(200.0f, 200.0f, 0.0f),
	m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
	m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))
{
	// vertex array
	m_VAO = std::make_unique<VertexArray>();

	// vertex buffer
	float halfSizeX = float(sizeX / 2);
	float halfSizeY = float(sizeY / 2);
	float positions[] = {
	-halfSizeX,  -halfSizeY,  0.0f, 0.0f, // 0  first two are Position, next two are texCoord
	 halfSizeX,  -halfSizeY,  1.0f, 0.0f, // 1
	 halfSizeX,   halfSizeY,  1.0f, 1.0f, // 2
	-halfSizeX,   halfSizeY,  0.0f, 1.0f  // 3
	};
	unsigned int vertexBufferSize = sizeof(positions) / sizeof(float) * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	m_VertexBuffer = std::make_unique<VertexBuffer>(positions, vertexBufferSize);

	// vertex array bind to buffer with proper layout
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	m_VAO->AddBuffer(*m_VertexBuffer, layout);

	// declare index buffer
	unsigned int indicies[] = {
		0, 1, 2,
		2, 3, 0
	};
	m_IndexBuffer = std::make_unique<IndexBuffer>(indicies, 6);

	// create and bind default shader for texture draw (we need to bind it here to set uniform)
	m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
	m_Shader->Bind();
	m_Shader->SetUniform4f("u_Color", (float*)&m_Color);

	// create texture and set shader u_Texture uniform value
	m_Texture = std::make_unique<Texture>(path);
	m_Texture->Bind(0);
	m_Shader->SetUniform1i("u_Texture", 0);  // specify texture location

	// 16:9 screen format projection mvp
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
	m_Shader->SetUniformMat4f("u_MVP", m_Proj * m_View * model);
}

test::TestDrawTexture::~TestDrawTexture()
{
}

void test::TestDrawTexture::OnUpdate(float deltaTime)
{
}

void test::TestDrawTexture::OnRender()
{
	Renderer::Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
}

void test::TestDrawTexture::OnImGuiRender()
{
	ImGui::ColorEdit4("Texture Color", (float*)&m_Color);
	m_Shader->SetUniform4f("u_Color", (float*)&m_Color);

	ImGui::SliderFloat3("Texture Translation", &m_Translation.x, 0.f, 960.f);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
	m_Shader->SetUniformMat4f("u_MVP", m_Proj * m_View * model);
}

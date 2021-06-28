#include "TestDrawTexture.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Renderer.h"
#include "imgui/imgui.h"

test::TestDrawTexture::TestDrawTexture(const char* path, unsigned int sizeX, unsigned int sizeY)
    : shader(defaultShader), texture(path), ib(indicies, 6)
{
    float sizeXHalf = float(sizeX) * 0.5f;
    float sizeYHalf = float(sizeY) * 0.5f;

    float positions[] = {  // todo: change to actual texture size
        -sizeXHalf,  -sizeYHalf,  0.0f, 0.0f, // 0  first two are Position, next two are texCoord
         sizeXHalf,  -sizeYHalf,  1.0f, 0.0f, // 1
         sizeXHalf,   sizeYHalf,  1.0f, 1.0f, // 2
        -sizeXHalf,   sizeYHalf,  0.0f, 1.0f  // 3
    };

    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0
    };

    for (int i = 0; i < 16; ++i)
    {
        m_Positions[i] = positions[i];
    }

    // vertex buffer provide triangle points positions as data
    vb.GenerateBuffer(positions, 4 * 4 * sizeof(float));

    vb_layout.Push<float>(2);
    vb_layout.Push<float>(2);
    va.AddBuffer(vb, vb_layout);

    // create default shader for texture draw
    shader.Bind();
    m_Color = glm::vec4(whiteColor[0], whiteColor[1], whiteColor[2], whiteColor[3]);
    shader.SetUniform4f("u_Color", (float*)&m_Color);

    texture.Bind(0);
    shader.SetUniform1i("u_Texture", 0);  // specify texture location

    // 16:9 screen format projection mvp
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    m_Translation = glm::vec3(200.0f, 200.0f, 0.0f);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
    glm::mat4 mvp = proj * view * model;

    shader.SetUniformMat4f("u_MVP", mvp);

    va.Unbind();      // clear vertex array buffer 
    shader.Unbind();  // clear program shader
    vb.Unbind();      // clear vertex buffer
    ib.Unbind();      // clear index buffer
}

test::TestDrawTexture::~TestDrawTexture()
{
}

void test::TestDrawTexture::OnUpdate(float deltaTime)
{
}

void test::TestDrawTexture::OnRender()
{
    Renderer::Draw(va, ib, shader);
}

void test::TestDrawTexture::OnImGuiRender()
{
    ImGui::ColorEdit4("Texture Color", (float*)&m_Color);
    shader.SetUniform4f("u_Color", (float*)&m_Color);

    ImGui::SliderFloat3("Texture Translation", &m_Translation.x, 0.f, 960.f);
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
    glm::mat4 mvp = proj * view * model;
    shader.SetUniformMat4f("u_MVP", mvp);
}

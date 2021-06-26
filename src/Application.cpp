#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define GLFW_INCLUDE_NONE

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 960;
    int height = 540;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // fps limit to monitor refresh rate
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float positions[] = {
            -50.0f,  -50.0f,  0.0f, 0.0f, // 0  first two are Position, next two are texCoord
             50.0f,  -50.0f,  1.0f, 0.0f, // 1
             50.0f,   50.0f,  1.0f, 1.0f, // 2
            -50.0f,   50.0f,  0.0f, 1.0f  // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // Open GL Blending Alpha
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // declare vertex array buffer and vertex then provide triangle points positions as data
        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // declare index buffer
        IndexBuffer ib(indices, 6);

        // 16:9 screen format projection
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

        // create and run shader program on gpu
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

        Texture texture("res/textures/UE_Logo.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();  // clear vertex array buffer 
        shader.Unbind(); // clear program shader
        vb.Unbind();  // clear vertex buffer
        ib.Unbind();  // clear index buffer

        Renderer renderer;

        // // Imgui init
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.DisplaySize.x = float(width);
        io.DisplaySize.y = float(height);

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        // logo translation
        glm::vec3 translationA = glm::vec3(200.0f, 200.0f, 0.0f);
        // second logo translation
        glm::vec3 translationB = glm::vec3(400.0f, 400.0f, 0.0f);
        
        // logo color
        ImVec4 colorA = { 1.0f, 1.0f, 1.0f, 1.0f };
        // second logo color
        ImVec4 colorB = { 1.0f, 0.25f, 0.0f, 1.0f };

        shader.Bind();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */

            renderer.Clear();

            // Poll and handle events (inputs, window resize, etc.)
            // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
            // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
            // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
            // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
            /* Poll for and process events */
            GLCall(glfwPollEvents());

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
            {
                ImGui::Begin("Test");  // Create a window called "Test" and append into it.
                
                ImGui::ColorEdit3("ColorA", (float*)&colorA);                      // Edit our shader color
                ImGui::SliderFloat2("TranslationA", &translationA.x, 0.f, 960.f);  // Set Translation
                ImGui::ColorEdit3("ColorB", (float*)&colorB);                      // Edit our shader color
                ImGui::SliderFloat2("TranslationB", &translationB.x, 0.f, 960.f);  // Set Translation

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            /* Imgui Rendering */
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* My Rendering */
            shader.SetUniform4f("u_Color", (float*)&colorA);                  // set first unreal logo color
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);  // unreal first logo texture translation
            glm::mat4 mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);                                    // draw first logo

            shader.SetUniform4f("u_Color", (float*)&colorB);                  // unreal second logo color
            model = glm::translate(glm::mat4(1.0f), translationB);            // unreal second logo translation
            mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);                                    // draw second logo

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));
        }
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
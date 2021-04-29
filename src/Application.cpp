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

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
             100.0f,  100.0f,  0.0f, 0.0f, // 0  first two are Position, next two are texCoord
             200.0f,  100.0f,  1.0f, 0.0f, // 1
             200.0f,  200.0f,  1.0f, 1.0f, // 2
             100.0f,  200.0f,  0.0f, 1.0f  // 3
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

        // 4:3 screen format projection
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 0.0f));
        glm::mat4 mvp = proj * view * model;

        // to see in dbg mode what is going on with proj * float positions
        //glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);
        //glm::vec4 result = proj * vp;

        // create and run shader program on gpu
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);

        Texture texture("res/textures/UE_Logo.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();  // clear vertex array buffer 
        shader.Unbind(); // clear program shader
        vb.Unbind();  // clear vertex buffer
        ib.Unbind();  // clear index buffer

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            //shader.SetUniform4f("u_Color", r, 0.3f, 0.5f, 1.0f);

            renderer.Draw(va, ib, shader);

            // little focus to change color
            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;
            //

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }

    glfwTerminate();
    return 0;
}
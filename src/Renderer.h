#pragma once

#include <string>

class VertexArray;
class IndexBuffer;
class Shader;

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
public:
    static void Clear();
    static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
};
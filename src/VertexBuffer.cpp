#include "VertexBuffer.h"

#include "GL/glew.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer()
	: m_RenderedID(0)
{
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GenerateBuffer(data, size);
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RenderedID));
}

void VertexBuffer::GenerateBuffer(const void* data, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_RenderedID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderedID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderedID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

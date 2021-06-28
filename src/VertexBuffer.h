#pragma once


class VertexBuffer
{
private:
	unsigned int m_RenderedID;

public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void GenerateBuffer(const void* data, unsigned int size);

	void Bind() const;
	void Unbind() const;
};
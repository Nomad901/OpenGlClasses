#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const uint32_t* pData, uint32_t pCount)
{
	ASSERT(sizeof(uint32_t) == sizeof(GLuint));

	mCount = pCount;

	GLCall(glGenBuffers(1, &mRendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, pCount * sizeof(uint32_t), pData, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &mRendererID));
}

int32_t IndexBuffer::getCount() const noexcept
{
	return mCount;
}

void IndexBuffer::bind() const 
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID));
}

void IndexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
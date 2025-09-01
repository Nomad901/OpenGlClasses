#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const std::vector<GLuint>& pIndicies)
{
	ASSERT(sizeof(uint32_t) == sizeof(GLuint));

	mCount = pIndicies.size();

	GLCall(glGenBuffers(1, &mRendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, pIndicies.size() * sizeof(uint32_t), pIndicies.data(), GL_STATIC_DRAW));
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
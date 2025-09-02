#include "VertexBufferOrig.h"
#include "Renderer.h"

VertexBufferOrig::VertexBufferOrig(const std::vector<Vertex>& pVertex, GLenum pUsage)
{
	init(pVertex, pUsage);
}

VertexBufferOrig::VertexBufferOrig(const void* pData, uint32_t pSize, GLenum pUsage)
{
	init(pData, pSize, pUsage);
}

VertexBufferOrig::~VertexBufferOrig()
{
	GLCall(glDeleteBuffers(1, &mRendererID));
}

void VertexBufferOrig::init(const std::vector<Vertex>& pVertex, GLenum pUsage)
{
	GLCall(glGenBuffers(1, &mRendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, pVertex.size() * sizeof(float), pVertex.data(), pUsage));
}

void VertexBufferOrig::init(const void* pData, uint32_t pSize, GLenum pUsage)
{
	GLCall(glGenBuffers(1, &mRendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, pSize, pData, pUsage));
}

void VertexBufferOrig::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
}

void VertexBufferOrig::unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


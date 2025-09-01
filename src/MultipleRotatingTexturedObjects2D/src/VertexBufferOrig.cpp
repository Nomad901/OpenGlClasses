#include "VertexBufferOrig.h"
#include "Renderer.h"

VertexBufferOrig::VertexBufferOrig(const std::vector<Vertex>& pVertex)
{
	init(pVertex);
}

VertexBufferOrig::VertexBufferOrig(const void* pData, uint32_t pSize)
{
	init(pData, pSize);
}

VertexBufferOrig::~VertexBufferOrig()
{
	GLCall(glDeleteBuffers(1, &mRendererID));
}

void VertexBufferOrig::init(const std::vector<Vertex>& pVertex)
{
	GLCall(glGenBuffers(1, &mRendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, pVertex.size() * sizeof(float), pVertex.data(), GL_STATIC_DRAW));
}

void VertexBufferOrig::init(const void* pData, uint32_t pSize)
{
	GLCall(glGenBuffers(1, &mRendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, pSize, pData, GL_STATIC_DRAW));
}

void VertexBufferOrig::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererID));
}

void VertexBufferOrig::unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


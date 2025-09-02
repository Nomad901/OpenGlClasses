#pragma once
#include <iostream>

#include "glm/glm.hpp"

class Renderer;

struct Vertex
{
	glm::vec3 mPos;
	glm::vec3 mNormals;
	glm::vec2 TexCoords;
};

class VertexBufferOrig
{
public:
	VertexBufferOrig() = default;
	VertexBufferOrig(const std::vector<Vertex>& pVertex);
	VertexBufferOrig(const void* pData, uint32_t pSize);
	~VertexBufferOrig();

	void init(const std::vector<Vertex>& pVertex);
	void init(const void* pData, uint32_t pSize);

	void bind() const;
	void unbind() const;

private:
	//
	// each element has its own renderer id - like texture, a mesh3D and so on
	//
	uint32_t mRendererID{};

};


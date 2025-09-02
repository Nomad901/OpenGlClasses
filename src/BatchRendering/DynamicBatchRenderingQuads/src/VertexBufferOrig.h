#pragma once
#include <iostream>

#include "glm/glm.hpp"
#include "glad/glad.h"

class Renderer;

struct Vertex
{
	glm::vec3 mPos;
	glm::vec4 mColors;
	glm::vec2 mTexCoords;
	float mTexId;
};

class VertexBufferOrig
{
public:
	VertexBufferOrig() = default;
	VertexBufferOrig(const std::vector<Vertex>& pVertex, GLenum pUsage);
	VertexBufferOrig(const void* pData, uint32_t pSize, GLenum pUsage);
	~VertexBufferOrig();

	void init(const std::vector<Vertex>& pVertex, GLenum pUsage);
	void init(const void* pData, uint32_t pSize, GLenum pUsage);

	void bind() const;
	void unbind() const;

private:
	//
	// each element has its own renderer id - like texture, a mesh3D and so on
	//
	uint32_t mRendererID{};

};


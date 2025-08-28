#pragma once
#include <iostream>
#include "glm.hpp"

class Renderer;

struct Vertex
{
	glm::vec3 mPosition;
	glm::vec3 mNormal;
	glm::vec3 mColor;
	glm::vec2 mTexUV;
};

class VertexBuffer
{
public:
	VertexBuffer(const std::vector<Vertex>& pVertexes);
	~VertexBuffer();

	void bind() const;
	void unbind() const;

private:
	//
	// each element has its own renderer id - like texture, a mesh3D and so on
	//
	uint32_t mRendererID{};

};


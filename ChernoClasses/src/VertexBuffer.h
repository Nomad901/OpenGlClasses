#pragma once
#include <iostream>

class Renderer;

class VertexBuffer
{
public:
	VertexBuffer(const void* pData, uint32_t pSize);
	~VertexBuffer();

	void bind() const;
	void unbind() const;

private:
	//
	// each element has its own renderer id - like texture, a mesh3D and so on
	//
	uint32_t mRendererID{};

};


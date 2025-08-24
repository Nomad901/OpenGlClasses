#pragma once
#include <iostream>

#include "Renderer.h"

class IndexBuffer
{
public:
	IndexBuffer(const uint32_t* pData, uint32_t pCount);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	int32_t getCount() const noexcept;

private:
	//
	// each element has its own renderer id - like texture, a mesh3D and so on
	//
	uint32_t mRendererID{};
	uint32_t mCount{};
};


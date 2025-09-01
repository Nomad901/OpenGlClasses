#pragma once
#include <iostream>
#include <vector>

#include "glad/glad.h"

class Renderer;

class IndexBuffer
{
public:
	IndexBuffer(const std::vector<GLuint>& pIndicies);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	int32_t getCount() const noexcept;

private:
	//
	// each element has its own renderer id - like texture, a mesh3D and so on
	//
	uint32_t mRendererID;
	uint32_t mCount;
};


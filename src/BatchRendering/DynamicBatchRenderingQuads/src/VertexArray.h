#pragma once

#include "VertexBufferOrig.h"

class VertexBufferLayout;

class VertexArray
{
public:
	VertexArray() = default;
	~VertexArray();
	
	void init();

	void addBuffer(const VertexBufferOrig& pVB, const VertexBufferLayout& pVBL);

	void bind() const;
	void unbind() const;

private:
	uint32_t mRendererID{};

};


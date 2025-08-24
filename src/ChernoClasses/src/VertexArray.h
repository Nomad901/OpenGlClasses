#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& pVB, const VertexBufferLayout& pVBL);

	void bind() const;
	void unbind() const;

private:
	uint32_t mRendererID{};

};


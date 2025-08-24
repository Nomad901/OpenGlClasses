#pragma once
#include <iostream>
#include <vector>

#include <glad/glad.h>

struct VertexBufferElement
{
	uint8_t mNorlmalized{};
	uint32_t mType{};
	uint32_t mCount{};

	static auto sizeOf(uint32_t pType) -> uint32_t
	{
		switch (pType)
		{
		case GL_UNSIGNED_BYTE:
			return sizeof(GLubyte);
		case GL_UNSIGNED_INT:
			return sizeof(GLuint);
		case GL_FLOAT:
			return sizeof(GLfloat);
		}
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout(uint32_t pStride)
		: mStride( pStride )
	{ }
	~VertexBufferLayout();

	auto push(uint32_t pType, uint32_t pCount) -> void
	{
		mElements.push_back({ GL_FALSE, pType, pCount });
		mStride += pCount * VertexBufferElement::sizeOf(pType);
	}

	inline auto getElements() const -> const std::vector<VertexBufferElement>&
	{
		return mElements;
	}
	inline auto getStride() const -> uint32_t
	{
		return mStride;
	}

private:
	std::vector<VertexBufferElement> mElements;
	uint32_t mStride{};
};


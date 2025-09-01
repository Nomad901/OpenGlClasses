#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &mRendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &mRendererID))
}

void VertexArray::addBuffer(const VertexBuffer& pVB, const VertexBufferLayout& pVBL)
{
	bind();
	pVB.bind();
	const auto& elements = pVBL.getElements();
	uint32_t offset = 0;
	for (size_t i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.mCount, element.mType, 
									    element.mNorlmalized, pVBL.getStride(), (const void*)offset));
		offset += element.mCount * VertexBufferElement::sizeOf(element.mType);
	}
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(mRendererID));
}

void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}

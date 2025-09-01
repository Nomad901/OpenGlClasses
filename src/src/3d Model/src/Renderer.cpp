#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* pFunction, const char* pFile, int32_t pLine)
{
	while (GLenum error = glGetError())
	{
		std::cout << std::format("[OPENGL ERROR] {} [FUNCTION] {} [FILE] {} [LINE] {}\n",
			error, pFunction, pFile, pLine);
		return false;
	}
	return true;
}

void Renderer::clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& pVa, const IndexBuffer& pIB, const Shader& pShader) const
{
	pShader.bind();
	pVa.bind();
	pIB.bind();
	GLCall(glDrawElements(GL_TRIANGLES, pIB.getCount(), GL_UNSIGNED_INT, nullptr));

}

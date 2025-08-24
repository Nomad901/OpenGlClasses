#include "Renderer.h"

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
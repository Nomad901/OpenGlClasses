#pragma once
#include <iostream>
#include <format>

#include "glad/glad.h"

#ifdef WIN32
	#define DEBUG_BREAK() __debugbreak()
#else 
	#define DEBUG_BREAK() __builtin_trap()
#endif // WIN32

#define ASSERT(x) if (!(x)) DEBUG_BREAK();
#define GLCall(x) \
    GLClearError(); \
    x; \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* pFunction, const char* pFile, int32_t pLine);

class Renderer
{
};


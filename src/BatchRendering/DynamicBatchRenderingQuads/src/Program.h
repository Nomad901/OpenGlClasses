#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <array>

#include "SDL3/SDL.h"
#include "glad/glad.h"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexBufferOrig.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include "Camera.h"

class Program
{
public:
	Program(int32_t pWinWidth = 1280, int32_t pWinHeigth = 720);
	~Program();

	void run();

private:
	static void glDebugOutput(GLenum source,
					  GLenum type,
					  GLuint id,
					  GLenum severity,
					  GLsizei length,
					  const GLchar* message,
					  const void* userParam);
	std::array<Vertex, 4> createQuad(float x, float y, float z, float pTetureID);

private:
	int32_t mWinWidth, mWinHeight;

	SDL_Window* mWindow;
	SDL_GLContext mContext;

	Camera mCamera;
	VertexArray mVAO;
	VertexBufferOrig mVBO;
	VertexBufferLayout mVBOLayout;
	IndexBuffer mEBO;
	Shader mShader;
	Renderer mRenderer;

	glm::vec3 mSizeQuad1{ -6.0f, -6.0f, -9.0f };
	glm::vec3 mSizeQuad2{ -3.0f, -3.0f, -10.0f };
};


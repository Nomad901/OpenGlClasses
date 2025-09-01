#pragma once
#include "ShadersSys.h"
#include "ErrorHandler.h"
#include "Mesh3D.h"
#include "Camera.h"
#include "UI.h"
#include "Texture.h"

#include "SDL3/SDL.h"

class Program
{
public:
	enum class typeMesh : uint32_t;
public:
	Program(typeMesh pTypeMesh, int32_t pScreenWidht, int32_t pScreenHeight);
	~Program();
	Program(const Program&) = delete;
	Program operator=(const Program&) = delete;

	void run();

	void getOpenGLInfo() const noexcept;
public:
	enum class typeMesh : uint32_t
	{
		TRIANGLE = 0,
		QUAD = 1,
		QUAD_IMG = 2,
		CUBE = 3,
		CUBE_IMG = 4
	};

private:
	void input();
	void draw();
	void preDraw();

	static void APIENTRY glDebugOutput(GLenum source,
									   GLenum type,
									   GLuint id,
									   GLenum severity,
									   GLsizei length,
									   const GLchar* message,
									   const void* userParam);

private:
	int32_t mScreenWidth{};
	int32_t mScreenHeight{};

	SDL_Window* mGraphicsApplicationWindow{ nullptr };
	SDL_GLContext mOpenGLContext{ nullptr };

	typeMesh mTypeMesh{ typeMesh::CUBE };

	bool mQuit{ false };
	bool mAttachCursor{ true };

	float mColorWindow[3]{ 0.0f,0.0f,0.0f };

	float mDelay{ 16.0f };

	GLuint mGraphicsPipelineProgram{ 0 };

	Mesh3D mMesh3D;
	Camera mCamera;
	UI mUI;
	ShadersSys mShadersSys;
	Texture mTexture;
};


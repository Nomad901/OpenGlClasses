#pragma once
#include <iostream>
#include <format>
#include <unordered_map>
#include <memory>

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "SDL3/SDL.h"
#include "glad/glad.h"

#include "FactoryMesh.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Primitive.h"

class Program
{
public:
	Program(uint32_t pWindowWidth = 1920, uint32_t pWindowHeight = 1080);
	~Program();

	void run();
	
private:
	void input();
	void preDraw();
	void draw();

	static void debugOutput(GLenum source,
							GLenum type,
							GLuint id,
							GLenum severity,
							GLsizei length,
							const GLchar* message,
							const void* userParam);
private:
	bool mProgIsRunning{ true };
	bool mWhiteScreen{ false };
	bool mTakeCursor{ true };
	float mRotateDegree{ -1.0f };
	uint32_t mWindowWidth, mWindowHeight;
	SDL_Window* mWindow;
	SDL_GLContext mContext;

	std::unordered_map<SDL_Keycode, bool> mKeyCodes;
	SDL_Event mEvent;

	Camera mCamera;
	Shader mShader;
	FactoryMesh mFactoryMeshes;
	Material mMaterial;

	glm::vec3 mPosLight{ 1.0f, 1.0f, 1.0f };

};
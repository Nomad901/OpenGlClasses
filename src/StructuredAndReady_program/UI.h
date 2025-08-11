#pragma once
#include "Mesh3D.h"

#include "SDL3/SDL.h"

#include "glad/glad.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

class UI
{
public:
	~UI();
	void init(SDL_Window* pWindow, SDL_GLContext pContext);
	
	void control(uint32_t pType, Mesh3D& pMesh3D);
};


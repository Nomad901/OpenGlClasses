#define SDL_MAIN_HANDLED
#include <iostream>
#include <format>
#include <SDL.h>
#include <glad.h>

int32_t screenWidth = 1280;
int32_t screenHeight = 720;

SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

bool gQuit = false;

void getOpenGLInfo()
{
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << '\n';
	std::cout << "nRenderer: " << glGetString(GL_RENDERER) << '\n';
	std::cout << "Version: " << glGetString(GL_VERSION) << '\n';
	std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
}

void initializeProgram()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Couldnt initilize the video!\n";
		exit(1);
	}

	// SETTING VERSION 4.6
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // VERSION 4
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6); // VERSION .6 

	// TURNING OFF OLD FUNCTIONS 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	gGraphicsApplicationWindow = SDL_CreateWindow("OpenGl", 1920 / 2, 1080 / 2, screenWidth, screenHeight,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!gGraphicsApplicationWindow)
	{
		std::cout << "Couldnt initialize the window!\n";
		exit(1);
	}

	gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);
	if (!gOpenGLContext)
	{
		std::cout << "Couldnt initialize the gl context!\n";
		exit(1);
	}

	// loading function pointers into SDL from glad (initiliazing glad)
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		std::cout << "glad wasnt initialized!\n";
		exit(1);
	}

	getOpenGLInfo();
}

void input()
{
	SDL_Event events;
	while (SDL_PollEvent(&events))
	{
		if (events.type == SDL_QUIT)
		{
			gQuit = true;
			return;
		}
	}

}

void preDraw()
{

}

void draw()
{

}

void mainLoop()
{
	while (!gQuit)
	{
		input();
		preDraw();
		draw();

		// Update the screen
		SDL_GL_SwapWindow(gGraphicsApplicationWindow);
	}
}

void cleanUp()
{
	SDL_DestroyWindow(gGraphicsApplicationWindow);

	SDL_Quit();
}

int main(int argc, char* argv[])
{
	initializeProgram();
	mainLoop();
	cleanUp();

	return 0;
}

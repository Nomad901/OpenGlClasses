#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "SDL3/SDL.h"
#include "glad/glad.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"

int32_t main(int32_t argc, int8_t* argv[])
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cout << "couldnt init sdl!\n";
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 4);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_Window* window = SDL_CreateWindow("OpenGlClassesByCherno", 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!window)
		std::cout << "Couldnt initialize the window!\n";
	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (!context)
		std::cout << "Couldnt initialize the context!\n";

	if (!SDL_GL_MakeCurrent(window, context))
		std::cout << "coulndt make window and context current!\n";
	if (!gladLoadGLLoader(GLADloadproc{}))
		std::cout << "coulndt make gladLoadGlLoader!\n";

	{
		float positions[] =
		{
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
		};

		uint32_t indicies[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout(2);
		layout.push(GL_FLOAT, 2);
		va.addBuffer(vb, layout);
		
		IndexBuffer ib(indicies, 6);

		std::string resourcePath = RESOURCES_PATH;
		Shader shader(resourcePath + "shaders.glsl");
		shader.bind();
		shader.setUniform4f("uColor", 0.8f, 0.3f, 0.8f, 1.0f);

		va.unbind();
		vb.unbind();
		ib.unbind();
		shader.unbind();

		float r = 0.0f;
		float increment = 0.05f;

		bool loopIsActive{ true };
		while (loopIsActive)
		{
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			shader.bind();
			shader.setUniform4f("uColor", r, 0.3f, 0.8f, 1.0f);

			va.bind();
			vb.bind();

			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			SDL_GL_SwapWindow(window);
		}
	}

	SDL_GL_DestroyContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
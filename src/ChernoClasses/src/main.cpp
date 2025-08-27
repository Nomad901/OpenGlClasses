#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "SDL3/SDL.h"
#include "glad/glad.h"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

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

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_Window* window = SDL_CreateWindow("OpenGlClassesByCherno", 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!window)
		std::cout << "Couldnt initialize the window!\n";
	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (!context)
		std::cout << "Couldnt initialize the context!\n";

	if (!SDL_GL_MakeCurrent(window, context))
		std::cout << "coulndt make window and context current!\n";
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		std::cout << "coulndt make gladLoadGlLoader!\n";

	{
		float positions[] =
		{
			-100.0f, -100.0f, 100.0f, 0.0f, 0.0f,
			 100.0f, -100.0f, 100.0f, 1.0f, 0.0f,
			 100.0f,  100.0f, 100.0f, 1.0f, 1.0f,
			-100.0f,  100.0f, 100.0f, 0.0f, 1.0f
		};

		uint32_t indicies[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		
		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.push(GL_FLOAT, 3);
		layout.push(GL_FLOAT, 2);
		va.addBuffer(vb, layout);

		IndexBuffer ib(indicies, 6);
		
		glm::mat4 proj = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -500.0f, 500.0f);
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0, 1.0f, 1.0f));
		glm::mat4 mvp = proj * viewMatrix;

		std::string resourcePath = RESOURCES_PATH;
		Shader shader(resourcePath + "shaders.shader");
		shader.bind();
		//shader.setUniform4f("uColor", 1.0f, 0.0f, 1.0f, 1.0f);
		shader.setUniform4fv("uMVP", mvp);

		Texture texture(resourcePath + "dockey.png");
		texture.bind(); // if we set inside of this function for instance 2, then we need to set this 2 in the next function;
		shader.setUniform1i("uTexture", 0); // right here, instead of 0 we would need to set 2;

		va.unbind();
		vb.unbind();
		ib.unbind();
		shader.unbind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;
		
		float projectionVariableY = 10.0f;
		float projectionVariableX = 10.0f;
		float projectionVariableZ = 10.0f;

		std::unordered_map<SDL_Keycode, bool> keyCodes;
		bool loopIsActive{ true };
		SDL_Event events;
		while (loopIsActive)
		{
			renderer.clear();

			while (SDL_PollEvent(&events))
			{
				if (events.type == SDL_EVENT_QUIT ||
					events.key.key == SDLK_ESCAPE)
				{
					loopIsActive = false;
					break;
				}
				
				if (events.type == SDL_EVENT_KEY_DOWN)
					keyCodes[events.key.key] = true;
				if (events.type == SDL_EVENT_KEY_UP)
					keyCodes[events.key.key] = false;
			}
			
			if (keyCodes[SDLK_W])
				projectionVariableY -= 10.0f;
			if (keyCodes[SDLK_S])
				projectionVariableY += 10.0f;
			if (keyCodes[SDLK_A])
				projectionVariableX += 10.0f;
			if (keyCodes[SDLK_D])
				projectionVariableX -= 10.0f;
			if (keyCodes[SDLK_UP])
				projectionVariableZ -= 10.0f;
			if (keyCodes[SDLK_DOWN])
				projectionVariableZ += 10.0f;

			glViewport(0, 0, 1280, 720);
			glClearColor(0.0f,
						 0.0f,
						 0.0f,
						 1.0f);

			renderer.draw(va, ib, shader);

			glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(projectionVariableX, projectionVariableY, projectionVariableZ));
			glm::mat4 mvp = proj * viewMatrix;
			shader.setUniform4fv("uMVP", mvp);

			if (r > 1.0f)
				increment = -0.005f;
			else if (r < 0.0f)
				increment = 0.005f;

			r += increment;

			SDL_GL_SwapWindow(window);
		}
	}

	SDL_GL_DestroyContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

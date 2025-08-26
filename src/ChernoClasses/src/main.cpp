#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "SDL3/SDL.h"
#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.BackendFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	
	ImGui::StyleColorsDark();

	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL3_InitForOpenGL(window, context);

	{
		float positions[] =
		{
			-0.5f, -0.5f, 
			 0.5f, -0.5f, 
			 0.5f,  0.5f, 
			-0.5f,  0.5f, 
		};

		// for the texture
		//0.0f, 0.0f,
		//1.0f, 0.0f,
		//1.0f, 1.0f,
		//0.0f, 1.0f

		float positions2[] =
		{
			-0.1f, -0.1f,
			 0.1f, -0.1f,
			 0.1f,  0.1f,
			-0.1f,  0.1f,
		};

		uint32_t indicies[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexArray va2;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));
		VertexBuffer vb2(positions2, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.push(GL_FLOAT, 2);
		va.addBuffer(vb, layout);
		
		VertexBufferLayout layout2; 
		layout2.push(GL_FLOAT, 2);
		va2.addBuffer(vb2, layout2);

		IndexBuffer ib(indicies, 6);

		std::string resourcePath = RESOURCES_PATH;
		Shader shader(resourcePath + "shaders.shader");
		shader.bind();
		shader.setUniform4f("uColor", 1.0f, 0.0f, 1.0f, 1.0f);

		Shader shader2(resourcePath + "shaders.shader");
		shader2.bind();
		shader2.setUniform4f("uColorSec", 1.0f, 1.0f, 1.0f, 0.5f);

		//Texture texture(resourcePath + "dockey.png");
		//texture.bind(); // if we set inside of this function for instance 2, then we need to set this 2 in the next function;
		//shader.setUniform1i("uTexture", 0); // right here, instead of 0 we would need to set 2;

		va.unbind();
		va2.unbind();
		vb.unbind();
		vb2.unbind();
		ib.unbind();
		shader.unbind();
		shader2.unbind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;

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
			}

			ImGui_ImplSDL3_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();
			glViewport(0, 0, 1280, 720);
			glClearColor(0.0f,
						 0.0f,
						 0.0f,
						 1.0f);

			renderer.draw(va, ib, shader);
			renderer.draw(va2, ib, shader2);
			//shader.setUniform4f("uColor", r, 0.3f, 0.8f, 1.0f);
			
			static bool firstTime = true;
			if (firstTime)
			{
				ImGui::SetNextWindowPos({ 100,100 });
				ImGui::SetNextWindowSize({ 300,300 });
				ImGui::SetNextWindowCollapsed(false);
			}

			ImGui::Begin("Cubes helper");

			ImGui::Button("fff");

			ImGui::End();

			if (r > 1.0f)
				increment = -0.005f;
			else if (r < 0.0f)
				increment = 0.005f;

			r += increment;

			ImGui::EndFrame();
			SDL_GL_SwapWindow(window);
		}
	}

	ImGui_ImplSDL3_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DestroyContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
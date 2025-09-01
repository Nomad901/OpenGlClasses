#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

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

#include "JSON/json.hpp"

void glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	}
	std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behavior"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behavior"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	}
	std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	}
	std::cout << std::endl << std::endl;
}

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

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&glDebugOutput, 0);

	{
		float vertices[] =
		{
			-1.0f, -1.0f, -9.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, -9.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, -9.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, -9.0f, 0.0f, 1.0f
		};

		uint32_t indices[] =
		{
			0, 1, 2,
			2, 3, 0,
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray VAO;
		VAO.bind();
		VertexBufferOrig VBO;
		VBO.init(vertices, 5 * 4 * sizeof(float));
		VertexBufferLayout VBOLayout;
		VBOLayout.push(GL_FLOAT, 3);
		VBOLayout.push(GL_FLOAT, 2);

		VAO.addBuffer(VBO, VBOLayout);
		IndexBuffer EBO;
		EBO.init(indices, 6);

		Camera camera;

		std::string resourcePath = RESOURCES_PATH;
		Shader shader(resourcePath + "shaders.shader");
		shader.bind();

		Texture texture(GL_TEXTURE_2D, "uTexture", resourcePath + "dockey.png");
		texture.activeTex();
		texture.bind();
		shader.setUniform1i("uTexture", 0);

		Renderer renderer;

		glm::vec3 translationA(2.0f, 1.0f, 0.0f);
		glm::vec3 translationB(4.0f, 1.0f, 0.0f);
		glm::vec3 translationC(6.0f, 1.0f, 0.0f);
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 20.0f);

		float r = 0.0f;
		float increment = 0.05f;

		std::unordered_map<SDL_Keycode, bool> keyCodes;
		bool loopIsActive{ true };
		bool whiteScreen{ false };
		bool attachCursor{ true };
		SDL_Event events;
		while (loopIsActive)
		{
			while (SDL_PollEvent(&events))
			{
				if (events.type == SDL_EVENT_QUIT ||
					events.key.key == SDLK_ESCAPE)
				{
					loopIsActive = false;
					break;
				}
				
				if (events.type == SDL_EVENT_MOUSE_MOTION &&
					attachCursor)
				{
					float x = events.motion.xrel / 5;
					float y = events.motion.yrel / 5;
					camera.moveCamera(glm::vec2(x, y));
				}

				if (events.type == SDL_EVENT_KEY_DOWN)
					keyCodes[events.key.key] = true;
				if (events.type == SDL_EVENT_KEY_UP)
					keyCodes[events.key.key] = false;
			}
			
			if (keyCodes[SDLK_V])
				whiteScreen = true;
			if (keyCodes[SDLK_B])
				whiteScreen = false;

			float speed = 0.1f;
			if (keyCodes[SDLK_W])
				camera.moveForward(speed);
			if (keyCodes[SDLK_S])
				camera.moveBackward(speed);
			if (keyCodes[SDLK_A])
				camera.moveLeft(speed);
			if (keyCodes[SDLK_D])
				camera.moveRight(speed);
			if (keyCodes[SDLK_Q])
				attachCursor = true;
			if (keyCodes[SDLK_E])
				attachCursor = false;

			if (attachCursor)
			{
				SDL_WarpMouseInWindow(window, 1280 / 2, 720 / 2);
				SDL_SetWindowRelativeMouseMode(window, true);
			}
			else 
				SDL_SetWindowRelativeMouseMode(window, false);

			glViewport(0, 0, 1280, 720);
			if (whiteScreen)
			{
				glClearColor(1.0f,
							 1.0f,
							 1.0f,
							 1.0f);
			}
			else
			{
				glClearColor(0.0f,
							 0.0f,
							 0.0f,
							 1.0f);
			}
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			texture.bind();
			shader.bind();
			
			glm::mat4 model(1.0f);
			glm::mat4 mvp(1.0f);
			model = glm::translate(model, translationA);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);
			
			renderer.draw(VAO, EBO, shader);

			model = glm::translate(model, translationB);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);

			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);
			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);			model = glm::translate(model, translationC);
			model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
			mvp = proj * camera.getMatrixView() * model;
			shader.setUniform4fv("uMVP", mvp);

			renderer.draw(VAO, EBO, shader);
			r += increment;

			SDL_GL_SwapWindow(window);
		}
	}

	SDL_GL_DestroyContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
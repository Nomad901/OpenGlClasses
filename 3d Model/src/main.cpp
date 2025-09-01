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
#include "Camera.h"
#include "Model.h"

#include "JSON/json.hpp"

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

	glViewport(0, 0, 1280, 720);
	
	std::string resourcePath = RESOURCES_PATH;
	Shader shader(resourcePath + "shaders.shader");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shader.bind();
	shader.setUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
	shader.setUniform4f("lightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	glEnable(GL_DEPTH_TEST);

	Camera camera(1280, 720, glm::vec3(0.0f, 0.0f, 2.0f), 45, 0.1f, 100.0f);

	std::filesystem::path modelPath = resourcePath + "grindstone_reworked" + "/" + "scene.gltf";

	Model model(modelPath);

	glm::vec2 mousePos;
	std::unordered_map<SDL_Keycode, bool> keyCodes;
	bool loopIsActive{ true };
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

			if (events.type == SDL_EVENT_KEY_DOWN)
				keyCodes[events.key.key] = true;
			if (events.type == SDL_EVENT_KEY_UP)
				keyCodes[events.key.key] = false;
			if (events.type == SDL_EVENT_MOUSE_MOTION)
			{
				mousePos.x = events.motion.xrel;
				mousePos.y = events.motion.yrel;
				camera.moveMouse(mousePos);
			}
		}

		glClearColor(0.07f,
					 0.13f,
					 0.17f,
					 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static float speed = 0.5f;
		if (keyCodes[SDLK_W])
			camera.moveForward(speed);
		if (keyCodes[SDLK_S])
			camera.moveBackward(speed);
		if (keyCodes[SDLK_A])
			camera.moveLeft(speed);
		if (keyCodes[SDLK_D])
			camera.moveRight(speed);

		glm::mat4 viewMatrix = camera.getViewMatrix();
		shader.setUniform4fv("uViewMatrix", viewMatrix);

		model.draw(shader, camera);

		SDL_GL_SwapWindow(window);
	}

	//{
	//	float positions[] =
	//	{
	//		-100.0f, -100.0f, 100.0f, 0.0f, 0.0f,
	//		 100.0f, -100.0f, 100.0f, 1.0f, 0.0f,
	//		 100.0f,  100.0f, 100.0f, 1.0f, 1.0f,
	//		-100.0f,  100.0f, 100.0f, 0.0f, 1.0f
	//	};
	//
	//	uint32_t indicies[] =
	//	{
	//		0, 1, 2,
	//		2, 3, 0
	//	};
	//
	//	GLCall(glEnable(GL_BLEND));
	//	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	//	
	//	Camera camera;
	//
	//	VertexArray va;
	//	VertexBuffer vb(positions, 4 * 5 * sizeof(float));
	//
	//	VertexBufferLayout layout;
	//	layout.push(GL_FLOAT, 3);
	//	layout.push(GL_FLOAT, 2);
	//	va.addBuffer(vb, layout);
	//
	//	IndexBuffer ib(indicies, 6);
	//
	//	std::string resourcePath = RESOURCES_PATH;
	//	Shader shader(resourcePath + "shaders.shader");
	//	shader.bind();
	//	//shader.setUniform4f("uColor", 1.0f, 0.0f, 1.0f, 1.0f);
	//
	//
	//	va.unbind();
	//	vb.unbind();
	//	ib.unbind();
	//	shader.unbind();
	//
	//	Renderer renderer;
	//
	//	float r = 0.0f;
	//	float increment = 0.05f;
	//	
	//	float projectionVariableY = 10.0f;
	//	float projectionVariableX = 10.0f;
	//	float projectionVariableZ = 10.0f;
	//
	//	glm::vec2 mousePos;
	//	std::unordered_map<SDL_Keycode, bool> keyCodes;
	//	bool loopIsActive{ true };
	//	SDL_Event events;
	//	while (loopIsActive)
	//	{
	//		renderer.clear();
	//
	//		while (SDL_PollEvent(&events))
	//		{
	//			if (events.type == SDL_EVENT_QUIT ||
	//				events.key.key == SDLK_ESCAPE)
	//			{
	//				loopIsActive = false;
	//				break;
	//			}
	//			
	//			if (events.type == SDL_EVENT_KEY_DOWN)
	//				keyCodes[events.key.key] = true;
	//			if (events.type == SDL_EVENT_KEY_UP)
	//				keyCodes[events.key.key] = false;
	//			if (events.type == SDL_EVENT_MOUSE_MOTION)
	//			{
	//				mousePos.x = events.motion.xrel;
	//				mousePos.y = events.motion.yrel;
	//				camera.moveMouse(mousePos);
	//			}
	//		}
	//		
	//		static float speed = 0.5f;
 //			if (keyCodes[SDLK_W])
	//			camera.moveForward(speed);
	//		if (keyCodes[SDLK_S])
	//			camera.moveBackward(speed);
	//		if (keyCodes[SDLK_A])
	//			camera.moveLeft(speed);
	//		if (keyCodes[SDLK_D])
	//			camera.moveRight(speed);
	//
	//		glClearColor(1.0f,
	//					 1.0f,
	//					 1.0f,
	//					 1.0f);
	//
	//		renderer.draw(va, ib, shader);
	//
	//		glm::mat4 viewMatrix = camera.getViewMatrix();
	//		shader.setUniform4fv("uViewMatrix", viewMatrix);
	//		glm::mat4 perspectiveMatrix = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	//		shader.setUniform4fv("uPerspective", perspectiveMatrix);
	//		
	//		if (r > 1.0f)
	//			increment = -0.005f;
	//		else if (r < 0.0f)
	//			increment = 0.005f;
//
	//		r += increment;
//
	//		SDL_GL_SwapWindow(window);
	//	}
	//}

	SDL_GL_DestroyContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
// the source of learning
// https://youtu.be/QjmPjT-Iheg?si=tA9sfI9sk42dLhv9 

#define SDL_MAIN_HANDLED
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <format>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <array>

#include <SDL3/SDL.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include "Camera.h"
#include "BlockFactory.h"

//#define WIN_WIDTH 1280
//#define WIN_HEIGHT 720 
//
//SDL_Window* window = nullptr;
//SDL_GLContext context;
//
//GLuint vertexArrayObject = 0;
//GLuint vertexBufferObject = 0;
//GLuint vertexIndexObject = 0;
//
//GLuint graphicsPipeLine = 0;
//
//std::string loadShaderAsAString(const std::filesystem::path& pPath)
//{
//	std::ifstream text(pPath.string());
//	if (text.is_open() && std::filesystem::exists(pPath))
//	{
//		std::string deducedString;
//		std::string line;
//		while (std::getline(text, line))
//		{
//			deducedString += line + '\n';
//		}
//		return deducedString;
//	}
//	std::cout << "File wasnt opened or file doesnt exist!\n";
//	return "";
//}
//
//std::pair<bool, int16_t> initProgram()
//{ 
//	if (!SDL_Init(SDL_INIT_VIDEO))
//		return std::make_pair(false, __LINE__);
//
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
//
//	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
//
//	window = SDL_CreateWindow("OpenGl", WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
//	if(!window)
//		return std::make_pair(false, __LINE__);
//	
//	context = SDL_GL_CreateContext(window);
//	if(!context)
//		return std::make_pair(false, __LINE__);
//
//	if(!SDL_GL_MakeCurrent(window, context))
//		return std::make_pair(false, __LINE__);
//
//	if(gladLoadGLLoader(GLADloadproc()))
//		return std::make_pair(false, __LINE__);
//
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO* io = &ImGui::GetIO();
//
//	io->BackendFlags = ImGuiBackendFlags_None;
//
//	ImGui::StyleColorsDark();
//	ImGui_ImplOpenGL3_Init();
//	ImGui_ImplSDL3_InitForOpenGL(window, context);
//	
//	return std::make_pair(true, __LINE__);
//}
//
//std::pair<bool, int16_t> createVertexes()
//{
//	std::vector<float> vertexes
//	{
//		-0.5f, -0.5f, 0.0f,
//		 0.0f,  1.0f, 0.0f,
//
//		 0.5f, -0.5f, 0.0f,
//		 1.0f,  0.0f, 0.0f,
//
//		 0.5f,  0.5f, 0.0f,
//		 1.0f,  1.0f, 0.0f,
//
//		-0.5f,  0.5f, 0.5f,
//		 0.0f,  1.0f, 0.01f
//	};
//
//	glCreateVertexArrays(1, &vertexArrayObject);
//	glCreateBuffers(1, &vertexBufferObject);
//
//	glNamedBufferData(vertexBufferObject,
//		vertexes.size() * sizeof(float),
//		vertexes.data(),
//		GL_STATIC_DRAW);
//
//	std::vector<int16_t> indexes{ 2,0,1, 3,0,1 };
//	glCreateBuffers(1, &vertexIndexObject);
//	glNamedBufferData(vertexIndexObject,
//					  indexes.size() * sizeof(int16_t),
//					  indexes.data(),
//					  GL_STATIC_DRAW);
//
//	glVertexArrayElementBuffer(vertexArrayObject, vertexIndexObject);
//
//	glVertexArrayVertexBuffer(vertexArrayObject, 
//							  0, 
//							  vertexBufferObject, 
//							  0, 
//							  6 * sizeof(float));
//
//	glEnableVertexArrayAttrib(vertexArrayObject, 0);
//	glVertexArrayAttribFormat(vertexArrayObject, 
//							  0, 
//							  3, 
//							  GL_FLOAT, 
//							  GL_FALSE, 
//							  0);
//	glVertexArrayAttribBinding(vertexArrayObject, 0, 0);
//
//	glEnableVertexArrayAttrib(vertexArrayObject, 1);
//	glVertexArrayAttribFormat(vertexArrayObject, 
//							  1, 
//							  3, 
//							  GL_FLOAT, 
//							  GL_FALSE, 
//							  3 * sizeof(float));
//	glVertexArrayAttribBinding(vertexArrayObject, 1, 0);
//}
//
//GLuint compileShaders(GLuint pType, std::string_view pSourceCode)
//{
//
//}
//
//GLuint getPipeline(std::string_view pVertexShader, std::string_view pFragmentShader)
//{
//
//}
//
//std::pair<bool, int16_t> createPipeline()
//{
//	std::string tmpResourcePath = RESOURCES_PATH;
//	std::string vertexShader = loadShaderAsAString(tmpResourcePath + "vertShad.glsl");
//	std::string fragmentShader = loadShaderAsAString(tmpResourcePath + "fragShad.glsl");
//
//	graphicsPipeLine = getPipeline(vertexShader, fragmentShader);
//}
//
//void input()
//{
//
//}
//
//void preDraw()
//{
//
//}
//
//void draw()
//{
//
//}
//
//void mainLoop()
//{
//	
//}
//
//void quitProgram()
//{
//
//}
//
//int main(int argc, char argv[])
//{
//	auto result = initProgram();
//	if (!result.first)
//		std::cout << "Cant initialize the program! Line: " << result.second;
//	result = createVertexes();
//	if (!result.first)
//		std::cout << "Cant create vertexes! Line: " << result.second;
//	result = createPipeline();
//	if (!result.first)
//		std::cout << "Cant create shaders! Line: " << result.second;
//	mainLoop();
//	quitProgram();
//
//	return 0;
//}

int32_t screenWidth = 1280;
int32_t screenHeight = 720;

SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

float gColorWindow[3] = { 0.0f,0.0f,0.0f	 };

bool gQuit = false;
bool attachCursor = true;

float delay = 16.0f;

Camera camera; 
BlockFactory factory;

std::unordered_map<SDL_Keycode, bool> strgKeyCodes;
std::vector<GLuint> indexBufferData;

// Program Object for our shaders 
GLuint gGraphicsPipelineProgram = 0;


std::string loadShaderAsString(const std::filesystem::path& filename)
{
	std::string result = "";
	std::string line = "";
	std::ifstream myFile(filename.string());
	if (myFile.is_open())
	{
		while (std::getline(myFile, line))
		{
			result += line + "\n";
		}
		myFile.close();
	}
	return result;
}

void getOpenGLInfo()
{
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << '\n';
	std::cout << "nRenderer: " << glGetString(GL_RENDERER) << '\n';
	std::cout << "Version: " << glGetString(GL_VERSION) << '\n';
	std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
}

void MessageCallback(GLenum pSource, GLenum pType, GLuint pId, GLenum pSeverity,
					 GLsizei pLength, const GLchar* pMessage, const void* pUserParam)
{
	std::cout << std::format("Error: {}; Source: {}\n", pMessage, pSource);
}

void initializeProgram()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cout << "Couldnt initilize the video!\n";
		exit(1);
	}

	// SETTING VERSION 4.6
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // VERSION 4
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6); // VERSION .6 
	
	// TURNING OFF OLD FUNCTIONS 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// DOUBLE BUFFER AND SMOOTH UPDATING 
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	gGraphicsApplicationWindow = SDL_CreateWindow("OpenGl",
												  screenWidth, screenHeight,
												  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
	if (!SDL_GL_MakeCurrent(gGraphicsApplicationWindow, gOpenGLContext))
	{
		std::cout << "Couldnt make the openGlContext current!\n";
		exit(1);
	}

	// loading function pointers into SDL from glad (initializing glad)
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "glad wasnt initialized!\n";
		exit(1);
	}
	glad_glEnable(GL_DEBUG_OUTPUT);
	glad_glDebugMessageCallback(MessageCallback, 0);

	IMGUI_CHECKVERSION();   // checking the version of ImGui for safety
	ImGui::CreateContext(); // creating a general context
	ImGuiIO& io = ImGui::GetIO(); // gets the IO configuration object
	
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.IniFilename = NULL;
	
	ImGui::StyleColorsDark();
	// correlating ImGui with SDL3 and OpenGL
	// needed headers were included!
	ImGui_ImplSDL3_InitForOpenGL(gGraphicsApplicationWindow, gOpenGLContext);
	ImGui_ImplOpenGL3_Init("#version 410");

	factory.pushFigure(Figure::Type::CUBE);
}

void vertexSpecification()
{
}

GLuint compileShader(GLuint pType, const std::string& pSourceCode)
{
	GLuint shaderObject = 0;

	if (pType == GL_VERTEX_SHADER)
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
	else if (pType == GL_FRAGMENT_SHADER)
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	// compilation proccess 
	const char* src = pSourceCode.c_str();
	glShaderSource(shaderObject, 1, &src, nullptr);
	glCompileShader(shaderObject);

	int32_t result;
	// retrieving compilation status
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int32_t length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		char* errorMessages = new char[length];
		glGetShaderInfoLog(shaderObject, length, &length, errorMessages);

		if (pType == GL_VERTEX_SHADER)
			std::cout << "ERROR: GL_VERTEX_SHADER compilation failed! " << errorMessages << '\n';
		else if (pType == GL_FRAGMENT_SHADER)
			std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed! " << errorMessages << '\n';

		delete[] errorMessages;
		glDeleteShader(shaderObject);
		return 0;
	}

	return shaderObject;
}

GLuint createShaderProgram(std::string_view pVertexShaderSource,
						   std::string_view pFragmentShaderSource)
{
	// create a new program object
	GLuint programObject = glCreateProgram();
	GLuint myVertexShader = compileShader(GL_VERTEX_SHADER, std::string(pVertexShaderSource));
	GLuint myFragmentShader = compileShader(GL_FRAGMENT_SHADER, std::string(pFragmentShaderSource));

	// merging our two shaders together into the one
	glAttachShader(programObject, myVertexShader);
	glAttachShader(programObject, myFragmentShader);
	glLinkProgram(programObject);

	//validate our program
	glValidateProgram(programObject);

	// once our final program object was created 
	// we can detach and erase our individual shaders
	glDetachShader(programObject, myVertexShader);
	glDetachShader(programObject, myFragmentShader);
	//
	// deleting our shaders
	//
	glDeleteShader(myVertexShader);
	glDeleteShader(myFragmentShader);

	return programObject;
}

void createGraphicsPipeline()
{
	std::string vertexShaderSource = loadShaderAsString("C:/Users/nomad/source/repos/OpenGLFinally/resources/vert.glsl");
	std::string fragmentShaderSource = loadShaderAsString("C:/Users/nomad/source/repos/OpenGLFinally/resources/frag.glsl");

	gGraphicsPipelineProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void imGuiSpecification()
{
	// showing the main window;
	//ImGui::ShowDemoWindow();
	static bool firstTime = true;
	static int32_t i = 0;
	if (firstTime)
	{
		ImGui::SetNextWindowPos({ 0,0 });
		ImGui::SetNextWindowSize({ 720,250 });
		ImGui::SetNextWindowCollapsed(true);
		firstTime = false;
	}
	ImGui::Begin("I am Quad", &gQuit, ImGuiFocusedFlags_None);

	static bool anotherWindow = false;
	ImGui::Checkbox("Choose exact block", &anotherWindow);
	if (anotherWindow)
	{
		static bool firstTime = true;
		if (firstTime)
		{
			ImGui::SetNextWindowPos({ 100,0 });
			ImGui::SetNextWindowSize({ 100,300 });
		}
		for (size_t i = 0; i < factory.getStorage().size(); ++i)
		{
			if (ImGui::Button(std::to_string(i).c_str()))
				factory.setCurrent(i);
		}

	}

	ImGui::Spacing();
	ImGui::Separator();

	//static bool FiguresShown = true;
	//if (ImGui::Checkbox("Show the figures/figure", &FiguresShown))
	//	factory.setAllFiguresShown(FiguresShown);

	//ImGui::Spacing();
	
	float colorFigure1[3] =
	{
		factory.getFigure(factory.getCurrentNum()).getColors()[0][0],
		factory.getFigure(factory.getCurrentNum()).getColors()[0][1],
		factory.getFigure(factory.getCurrentNum()).getColors()[0][2],
	};
	float colorFigure2[3] =
	{
		factory.getFigure(factory.getCurrentNum()).getColors()[1][0],
		factory.getFigure(factory.getCurrentNum()).getColors()[1][1],
		factory.getFigure(factory.getCurrentNum()).getColors()[1][2],
	};
	float colorFigure3[3] =
	{
		factory.getFigure(factory.getCurrentNum()).getColors()[2][0],
		factory.getFigure(factory.getCurrentNum()).getColors()[2][1],
		factory.getFigure(factory.getCurrentNum()).getColors()[2][2],
	};
	float colorFigure4[3] =
	{
		factory.getFigure(factory.getCurrentNum()).getColors()[3][0],
		factory.getFigure(factory.getCurrentNum()).getColors()[3][1],
		factory.getFigure(factory.getCurrentNum()).getColors()[3][2],
	};

	if (ImGui::ColorEdit3("Color of the Quad vertex 1", colorFigure1) ||
		ImGui::ColorEdit3("Color of the Quad vertex 2", colorFigure1) ||
		ImGui::ColorEdit3("Color of the Quad vertex 3", colorFigure1) ||
		ImGui::ColorEdit3("Color of the Quad vertex 4", colorFigure1))
	{
		std::array<float, 3> colors1;
		std::array<float, 3> colors2;
		std::array<float, 3> colors3;
		std::array<float, 3> colors4;
		for (size_t i = 0; i < 3; ++i)
		{
			colors1[i] = colorFigure1[i];
			colors2[i] = colorFigure2[i];
			colors3[i] = colorFigure3[i];
			colors4[i] = colorFigure4[i];
		}
		factory.getFigure(factory.getCurrentNum()).setColors({ {colors1, colors2,colors3,colors4} });
		for (auto& i : factory.getFigure(factory.getCurrentNum()).getColors())
		{
			for (auto& j : i)
			{
				std::cout << std::format("number: {}\n", j);
			}
		}
	}

	ImGui::Spacing();

	if (ImGui::SliderFloat("Width", &factory.getFigure(factory.getCurrentNum()).getWidth(), 0.1f, 1.0f) ||
		ImGui::SliderFloat("Height", &factory.getFigure(factory.getCurrentNum()).getHeight(), 0.1f, 1.0f))
	{
	}

	ImGui::Separator();

	ImGui::ColorEdit3("Color Of The Window", gColorWindow);

	ImGui::Spacing();

	ImGui::End();
}

void input()
{
	static float mouseX = screenWidth  / 2;
	static float mouseY = screenHeight / 2;

	auto state = SDL_GetKeyboardState(NULL);

	SDL_Event events;
	while (SDL_PollEvent(&events) != 0)
	{
		// UI interactions
		ImGui_ImplSDL3_ProcessEvent(&events);

		if (events.type == SDL_EVENT_QUIT)
		{
			gQuit = true;
			return;
		}

		if (events.type == SDL_EVENT_MOUSE_MOTION)
		{
			mouseX += events.motion.xrel;
			mouseY += events.motion.yrel;
			camera.mouseLook(mouseX, mouseY);
		}

		factory.getFigure(factory.getCurrentNum()).setRotation(factory.getFigure(factory.getCurrentNum()).getRotation() - 0.1f);
		float speed = 0.1f;
		// use some other keys to move our object
		if (state[SDL_SCANCODE_W])
			camera.moveForward(speed);
		if (state[SDL_SCANCODE_S])
			camera.moveBackward(speed);
		if (state[SDL_SCANCODE_A])
			camera.moveLeft(speed);
		if (state[SDL_SCANCODE_D])
			camera.moveRight(speed);
		if (state[SDL_SCANCODE_Z])
			attachCursor = !attachCursor;
	}
}

void preDraw()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//
	// ImGui
	//
	// Creating an ImGui's frame
	ImGui_ImplSDL3_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame(); 
	// main code goes here
	imGuiSpecification();

	// the background of the screen 
	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(gColorWindow[0], 
				 gColorWindow[1],
				 gColorWindow[2],
				 1.0f);
	
	//
	// clear color buffer and depth buffer (the screen)
	//
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// use our shader
	glUseProgram(gGraphicsPipelineProgram);

	factory.getFigure(factory.getCurrentNum()).setRotation(factory.getFigure(factory.getCurrentNum()).getRotation() - 0.1f);

	// from local to world space
	glm::mat4 model(1.0f); 
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, factory.getFigure(factory.getCurrentNum()).getOffset()));
	model = glm::rotate(model, glm::radians(factory.getFigure(factory.getCurrentNum()).getRotation()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

	GLuint uModelMatrixLocation = glGetUniformLocation(gGraphicsPipelineProgram, "uModelMatrix");
	if (uModelMatrixLocation >= 0)
		glUniformMatrix4fv(uModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
	else
	{
		std::cout << "Couldnt fint uModelMatrixLocation!\n";
		exit(EXIT_FAILURE);
	}


	glm::mat4 view = camera.getViewMatrix();
	GLuint uViewLocation = glGetUniformLocation(gGraphicsPipelineProgram, "uViewMatrix");
	if (uViewLocation >= 0)
		glUniformMatrix4fv(uViewLocation, 1, GL_FALSE, &view[0][0]);
	else
	{
		std::cout << "Couldnt find uViewMatrixLocation!\n";
		exit(EXIT_FAILURE);
	}

	// 1 - vide of view. than greater this - then more we can see. like in minecraft, when u change the view
	// 2 - ratio
	// 3 - how close we can see
	// 4 - how far we can see
	glm::mat4 perspective = glm::perspective(glm::radians(45.0f), 
											 float(screenWidth) / float(screenHeight),
											 0.1f,
											 20.0f);

	GLuint uProjectionLocation = glGetUniformLocation(gGraphicsPipelineProgram, "uProjection");
	if (uProjectionLocation >= 0)
		glUniformMatrix4fv(uProjectionLocation, 1, GL_FALSE, &perspective[0][0]);
	else
	{
		std::cout << "Couldnt find uProjectionLocation!\n";
		exit(EXIT_FAILURE);
	}
}

void draw()
{
	ImGui::EndFrame();

	factory.render();

	// stopping using our current graphics pipeline
	glUseProgram(0);

	// render everything
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void mainLoop()
{
	while (!gQuit)
	{
		if (attachCursor)
		{
			SDL_WarpMouseInWindow(gGraphicsApplicationWindow, screenWidth / 2, screenHeight / 2);
			SDL_SetWindowRelativeMouseMode(gGraphicsApplicationWindow, true);
		}
		else
			SDL_SetWindowRelativeMouseMode(gGraphicsApplicationWindow, false);

		input();
		float lastTime = SDL_GetTicks();
		preDraw();
		draw();		

		// Update the screen // swapping buffers 
		SDL_GL_SwapWindow(gGraphicsApplicationWindow);
		float delta = SDL_GetTicks() - lastTime;
		if (delta < delay)
			SDL_Delay(static_cast<Uint32>(delay - delta));
	}
}

void cleanUp()
{
	ImGui_ImplSDL3_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DestroyContext(gOpenGLContext);
	SDL_DestroyWindow(gGraphicsApplicationWindow);
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	initializeProgram();
	vertexSpecification(); // giving some vertex data on gpu
	createGraphicsPipeline();
	//getOpenGLInfo();
	mainLoop();
	cleanUp();

	return 0;
}

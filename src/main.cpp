// the source of learning
// https://youtu.be/QjmPjT-Iheg?si=tA9sfI9sk42dLhv9 

#define SDL_MAIN_HANDLED
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
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

int32_t screenWidth = 1280;
int32_t screenHeight = 720;

SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

float gColorWindow[3] = { 0.0f,0.0f,0.0f };
float gColorQuad1[3]  = { 1.0f,0.0f,0.0f };
float gColorQuad2[3]  = { 1.0f,0.0f,0.0f };
float gColorQuad3[3]  = { 1.0f,0.0f,0.0f };
float gColorQuad4[3]  = { 1.0f,0.0f,0.0f };

bool gShowQuad = false;
bool gQuit = false;

float gQuadWidth = 0.5f;
float gQuadHeight = 0.5f;

float g_uOffsetY = 0.0f;
float g_uOffsetX = 0.0f;

// VAO
GLuint gVertexArrayObject = 0;
// VBO
GLuint gVertexBufferObject = 0;
GLuint gIndexBufferObject = 0;
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
}

void vertexSpecification()
{
	//
	// lives on CPU.
	// setting our vertex positions
	//
	const std::vector<GLfloat> vertexData
	{
		// x    y     z
		-0.8f, -0.8f, 0.0f,  // left vertex position
		 1.0f, 0.0f, 0.0f,   // color 
		 0.8f, -0.8f, 0.0f,  // right vertex position
		 0.0f, 1.0f, 0.0f,	 // color 
		 0.0f,  0.8f, 0.0f,  // top vertex position 
		 0.0f, 0.0f, 1.0f    // color 
	};
	const std::vector<GLfloat> vertexDataQuad
	{
		//First triangle
		-gQuadWidth, -gQuadHeight, 0.0f,
		 gColorQuad1[0],  gColorQuad1[1], gColorQuad1[2],

		 gQuadWidth, -gQuadHeight, 0.0f,
		 gColorQuad2[0],  gColorQuad2[1], gColorQuad2[2],

		-gQuadWidth,  gQuadHeight, 0.0f,
		 gColorQuad3[0],  gColorQuad3[1], gColorQuad3[2],

		//Second triangle
		 gQuadWidth,  gQuadHeight, 0.0f,
		 gColorQuad4[0],  gColorQuad4[1], gColorQuad4[2],
	}; // we need to go in a wind order 

	/*<-----> WITH DSA <----->*/
	// creating VAO and VBO
	glCreateVertexArrays(1, &gVertexArrayObject);
	glCreateBuffers(1, &gVertexBufferObject);
	
	// populating data into VBO
	glNamedBufferData(gVertexBufferObject, 
					  vertexDataQuad.size() * sizeof(GL_FLOAT),
					  vertexDataQuad.data(),
					  GL_STATIC_DRAW);

	// setting up an index buffer object(IBO or EBO)
	const std::vector<GLuint> indexBufferData{ 2,0,1, 3,2,1 };
	glCreateBuffers(1, &gIndexBufferObject);
	glNamedBufferData(gIndexBufferObject, 
					  indexBufferData.size() * sizeof(GLuint), 
					  indexBufferData.data(), 
					  GL_STATIC_DRAW);
	glVertexArrayElementBuffer(gVertexArrayObject, gIndexBufferObject);

	//instead of binding vertexes and buffers we can use this:
	glVertexArrayVertexBuffer(gVertexArrayObject,
							  0,
							  gVertexBufferObject, 
							  0,
							  6 * sizeof(GLfloat));
	
	glEnableVertexArrayAttrib(gVertexArrayObject, 0);
	glVertexArrayAttribFormat(gVertexArrayObject,
							  0,
							  3, 
							  GL_FLOAT, 
							  GL_FALSE,
							  0);
	glVertexArrayAttribBinding(gVertexArrayObject, 0 /* the index of this operation */, 0 /* the index of VAO */);

	glEnableVertexArrayAttrib(gVertexArrayObject, 1);
	glVertexArrayAttribFormat(gVertexArrayObject,
							  1,
							  3,
							  GL_FLOAT,
							  GL_FALSE, 
							  3 * sizeof(GLfloat));
	glVertexArrayAttribBinding(gVertexArrayObject, 1, 0);

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
		ImGui::SetNextWindowPos({ 100,100 });
		ImGui::SetNextWindowSize({ 720,250 });
		firstTime = false;
	}
	ImGui::Begin("I am Quad", &gQuit, ImGuiFocusedFlags_None);
	
	ImGui::Text("If you want to move the Quad, you can use your arrows on the keyboard or WASD if you dont have one.");

	ImGui::Spacing();
	ImGui::Separator();

	ImGui::Checkbox("Show the Quad", &gShowQuad);

	ImGui::Spacing();

	if (ImGui::ColorEdit3("Color of the Quad vertex 1", gColorQuad1) ||
		ImGui::ColorEdit3("Color of the Quad vertex 2", gColorQuad2) ||
		ImGui::ColorEdit3("Color of the Quad vertex 3", gColorQuad3) ||
		ImGui::ColorEdit3("Color of the Quad vertex 4", gColorQuad4))
	{
		vertexSpecification();
	}

	ImGui::Spacing();

	if (ImGui::SliderFloat("Width", &gQuadWidth, 0.1f, 1.0f) ||
		ImGui::SliderFloat("Height", &gQuadHeight, 0.1f, 1.0f))
	{
		vertexSpecification();
	}

	ImGui::Separator();

	ImGui::ColorEdit3("Color Of The Window", gColorWindow);

	ImGui::Spacing();

	ImGui::End();
}

void input()
{
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
		if (state[SDL_SCANCODE_UP] ||
			state[SDL_SCANCODE_W])
			g_uOffsetY += 0.1f;
		if (state[SDL_SCANCODE_DOWN] ||
			state[SDL_SCANCODE_S])
			g_uOffsetY -= 0.1f;
		if (state[SDL_SCANCODE_RIGHT] ||
			state[SDL_SCANCODE_D])
			g_uOffsetX += 0.1f;
		if (state[SDL_SCANCODE_LEFT] ||
			state[SDL_SCANCODE_A])
			g_uOffsetX -= 0.1f;
	}
}

void preDraw()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

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

	GLuint locationY = glGetUniformLocation(gGraphicsPipelineProgram, "uOffsetY");
	if (locationY >= 0)
		glUniform1f(locationY, g_uOffsetY);
	else
		std::cout << "Couldnt find the location of the variable!\n";

	GLuint locationX = glGetUniformLocation(gGraphicsPipelineProgram, "uOffsetX");
	if (locationX >= 0)
		glUniform1f(locationX, g_uOffsetX);
	else
		std::cout << "Couldnt find the location of the variable!\n";
}

void draw()
{
	ImGui::EndFrame();

	// enable our attributes
	glBindVertexArray(gVertexArrayObject);
	// select the VBO which we wanna use
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
	// render the data. 0 the beginning of the array, 3 - the end
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	if (gShowQuad)
	{
		glDrawElements(GL_TRIANGLES,
					   6,
					   GL_UNSIGNED_INT,
					   0);
	}
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
		input();
		preDraw();
		draw();		

		// Update the screen // swapping buffers 
		SDL_GL_SwapWindow(gGraphicsApplicationWindow);
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
#include "Program.h"

Program::Program(int32_t pWinWidth, int32_t pWinHeigth)
	: mWinWidth(pWinHeigth), 
	  mWinHeight(pWinHeigth)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cout << "couldnt init sdl!\n";
		return;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	mWindow = SDL_CreateWindow("OpenGlClassesByCherno", pWinWidth, pWinHeigth, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!mWindow)
		std::cout << "Couldnt initialize the window!\n";
	mContext = SDL_GL_CreateContext(mWindow);
	if (!mContext)
		std::cout << "Couldnt initialize the context!\n";

	if (!SDL_GL_MakeCurrent(mWindow, mContext))
		std::cout << "coulndt make window and context current!\n";
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		std::cout << "coulndt make gladLoadGlLoader!\n";

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&glDebugOutput, 0);
	mVAO.init();
}

Program::~Program()
{
	SDL_GL_DestroyContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Program::run()
{

	uint32_t indices[] =
	{
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	mVAO.bind();
	mVBO.init(nullptr, sizeof(Vertex) * 1000, GL_DYNAMIC_DRAW);
	mVBOLayout.push(GL_FLOAT, 3);
	mVBOLayout.push(GL_FLOAT, 4);
	mVBOLayout.push(GL_FLOAT, 2);
	mVBOLayout.push(GL_FLOAT, 1);
	mVAO.addBuffer(mVBO, mVBOLayout);
	mEBO.init(indices, 12);

	std::string resourcePath = RESOURCES_PATH;
	mShader.init(resourcePath + "shaders.shader");
	mShader.bind();

	Texture texture2(GL_TEXTURE_2D, "uTexture", resourcePath + "dock.png");
	Texture texture(GL_TEXTURE_2D, "uTexture", resourcePath + "dockey.png");
	texture.bind(0);
	texture2.bind(1);
	int32_t textures[2] = { 0,1 };
	mShader.setUniform1iv("uTexture", 2, textures);

	glm::vec3 translationA(2.0f, 1.0f, 0.0f);
	glm::vec3 translationB(4.0f, 1.0f, 0.0f);
	glm::vec3 translationC(6.0f, 1.0f, 0.0f);
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), float(mWinWidth / mWinHeight), 0.1f, 200.0f);

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
				mCamera.moveCamera(glm::vec2(x, y));
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
			mCamera.moveForward(speed);
		if (keyCodes[SDLK_S])
			mCamera.moveBackward(speed);
		if (keyCodes[SDLK_A])
			mCamera.moveLeft(speed);
		if (keyCodes[SDLK_D])
			mCamera.moveRight(speed);
		if (keyCodes[SDLK_Q])
			attachCursor = true;
		if (keyCodes[SDLK_E])
			attachCursor = false;

		if (attachCursor)
		{
			SDL_WarpMouseInWindow(mWindow, mWinWidth / 2, mWinHeight / 2);
			SDL_SetWindowRelativeMouseMode(mWindow, true);
		}
		else
			SDL_SetWindowRelativeMouseMode(mWindow, false);

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

		auto quad1 = createQuad(mSizeQuad1.x, mSizeQuad1.y, mSizeQuad1.z, 0.0f);
		auto quad2 = createQuad(mSizeQuad2.x, mSizeQuad2.y, mSizeQuad2.z, 1.0f);

		Vertex vertices[8];
		memcpy(vertices, quad1.data(), quad1.size() * sizeof(Vertex));
		memcpy(vertices + quad1.size(), quad2.data(), quad2.size() * sizeof(Vertex));

		mVAO.bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mShader.bind();
		texture.bind(0);
		texture2.bind(1);
		int32_t textures[2] = { 0,1 };
		mShader.setUniform1iv("uTexture", 2, textures);

		glm::mat4 model(1.0f);
		glm::mat4 mvp(1.0f);
		model = glm::translate(model, translationA);
		model = glm::rotate(model, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
		mvp = proj * mCamera.getMatrixView() * model;
		mShader.setUniform4fv("uMVP", mvp);

		mRenderer.draw(mVAO, mEBO, mShader);

		r += increment;

		SDL_GL_SwapWindow(mWindow);
	}
}

void Program::glDebugOutput(GLenum source,
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

std::array<Vertex, 4> Program::createQuad(float x, float y, float z, float pTextureID)
{
	size_t size = 1.0f;
	Vertex vertex1 = { glm::vec3(x, y, z),				 glm::vec4(1.0f,1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), pTextureID };
	Vertex vertex2 = { glm::vec3(x + size, y, z),		 glm::vec4(1.0f,1.0f,1.0f,1.0f), glm::vec2(1.0f,0.0f), pTextureID };
	Vertex vertex3 = { glm::vec3(x + size, y + size, z), glm::vec4(1.0f,1.0f,1.0f,1.0f), glm::vec2(1.0f,1.0f), pTextureID };
	Vertex vertex4 = { glm::vec3(x, y + size, z),		 glm::vec4(1.0f,1.0f,1.0f,1.0f), glm::vec2(0.0f,1.0f), pTextureID };
	return { vertex1, vertex2, vertex3, vertex4 };
}

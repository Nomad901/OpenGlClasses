#include "Program.h"

Program::Program(typeMesh pTypeMesh, int32_t pScreenWidht, int32_t pScreenHeight)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		LOG(type::CRITICAL_ERROR, "Couldnt initialize SDL");
		exit(1);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	mScreenWidth  = pScreenWidht;
	mScreenHeight = pScreenHeight;

	mGraphicsApplicationWindow = SDL_CreateWindow("OpenGl",
												  pScreenWidht, pScreenHeight,
												  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!mGraphicsApplicationWindow)
	{
		LOG(type::CRITICAL_ERROR, "Couldnt initialize the window!");
		exit(1);
	}

	mOpenGLContext = SDL_GL_CreateContext(mGraphicsApplicationWindow);
	if (!mOpenGLContext)
	{
		LOG(type::CRITICAL_ERROR, "Couldnt initialize the gl context!");
		exit(1);
	}
	if (!SDL_GL_MakeCurrent(mGraphicsApplicationWindow, mOpenGLContext))
	{
		LOG(type::ERROR, "Couldnt make the openGlContext current!");
		exit(1);
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		LOG(type::CRITICAL_ERROR, "glad wasnt initialized!");
		exit(1);
	}
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	std::string resourcePath = RESOURCES_PATH;
	mShadersSys.init(mGraphicsPipelineProgram, 
					 resourcePath + "vert.glsl", GL_VERTEX_SHADER, 
					 resourcePath + "frag.glsl", GL_FRAGMENT_SHADER);
	mTypeMesh = pTypeMesh;
	switch (pTypeMesh)
	{
	case Program::typeMesh::TRIANGLE:
		mMesh3D.initTriangle();
		break;
	case Program::typeMesh::QUAD:
		mMesh3D.initQuad();
		break;
	case Program::typeMesh::CUBE:
		mMesh3D.initCube(); 
		break;
	default:
		break;
	}
	
	mUI.init(mGraphicsApplicationWindow, mOpenGLContext);
}

Program::~Program()
{
	SDL_GL_DestroyContext(mOpenGLContext);
	SDL_DestroyWindow(mGraphicsApplicationWindow);
	SDL_Quit();
}

void Program::run()
{
	while (!mQuit)
	{
		if (mAttachCursor)
		{
			SDL_WarpMouseInWindow(mGraphicsApplicationWindow, mScreenWidth / 2, mScreenHeight / 2);
			SDL_SetWindowRelativeMouseMode(mGraphicsApplicationWindow, true);
		}
		else
			SDL_SetWindowRelativeMouseMode(mGraphicsApplicationWindow, false);

		input();
		float lastTime = SDL_GetTicks();
		preDraw();
		draw();

		SDL_GL_SwapWindow(mGraphicsApplicationWindow);
		float delta = SDL_GetTicks() - lastTime;
		if (delta < mDelay)
			SDL_Delay(static_cast<Uint32>(mDelay - delta));
	}
}

void Program::getOpenGLInfo() const noexcept
{
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << '\n';
	std::cout << "nRenderer: " << glGetString(GL_RENDERER) << '\n';
	std::cout << "Version: " << glGetString(GL_VERSION) << '\n';
	std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
}

void Program::input()
{
	static float mouseX = mScreenWidth / 2;
	static float mouseY = mScreenHeight / 2;

	auto state = SDL_GetKeyboardState(NULL);

	SDL_Event events;
	while (SDL_PollEvent(&events) != 0)
	{
		ImGui_ImplSDL3_ProcessEvent(&events);

		if (events.type == SDL_EVENT_QUIT)
		{
			mQuit = true;
			return;
		}

		if (events.type == SDL_EVENT_MOUSE_MOTION)
		{
			mouseX += events.motion.xrel;
			mouseY += events.motion.yrel;
			mCamera.mouseLook(mouseX, mouseY);
		}

		mMesh3D.setRotation(mMesh3D.getRotation() - 0.1f);
		float speed = 0.1f;
		if (state[SDL_SCANCODE_W])
			mCamera.moveForward(speed);
		if (state[SDL_SCANCODE_S])
			mCamera.moveBackward(speed);
		if (state[SDL_SCANCODE_A])
			mCamera.moveLeft(speed);
		if (state[SDL_SCANCODE_D])
			mCamera.moveRight(speed);
		if (state[SDL_SCANCODE_Z])
			mAttachCursor = !mAttachCursor;
		if (state[SDL_SCANCODE_ESCAPE])
			mQuit = true;
	}
}

void Program::draw()
{
	ImGui::EndFrame();

	mMesh3D.render();

	glUseProgram(0);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Program::preDraw()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	ImGui_ImplSDL3_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	mUI.control(std::underlying_type_t<typeMesh>(mTypeMesh) , mMesh3D);

	glViewport(0, 0, mScreenWidth, mScreenHeight);
	glClearColor(mColorWindow[0],
				 mColorWindow[1],
				 mColorWindow[2],
				 1.0f);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(mGraphicsPipelineProgram);

	mMesh3D.setRotation(mMesh3D.getRotation() - 0.1f);

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, mMesh3D.getOffset()));
	model = glm::rotate(model, glm::radians(mMesh3D.getRotation()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

	GLuint uModelMatrixLocation = glGetUniformLocation(mGraphicsPipelineProgram, "uModelMatrix");
	if (uModelMatrixLocation >= 0)
		glUniformMatrix4fv(uModelMatrixLocation, 1, false, &model[0][0]);
	else
	{
		std::cout << "Couldnt fint uModelMatrixLocation!\n";
		exit(EXIT_FAILURE);
	}


	glm::mat4 view = mCamera.getViewMatrix(); 
	GLuint uViewLocation = glGetUniformLocation(mGraphicsPipelineProgram, "uViewMatrix");
	if (uViewLocation >= 0)
		glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]);
	else
	{
		std::cout << "Couldnt find uViewMatrixLocation!\n";
		exit(EXIT_FAILURE);
	}

	glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
											 float(mScreenWidth) / float(mScreenHeight),
											 0.1f,
											 20.0f);

	GLuint uProjectionLocation = glGetUniformLocation(mGraphicsPipelineProgram, "uProjection");
	if (uProjectionLocation >= 0)
		glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]);
	else
	{
		std::cout << "Couldnt find uProjectionLocation!\n";
		exit(EXIT_FAILURE);
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
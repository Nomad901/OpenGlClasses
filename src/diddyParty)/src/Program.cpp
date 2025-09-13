#include "Program.h"

Program::Program(uint32_t pWindowWidth, uint32_t pWindowHeight)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cout << "Couldnt initialize the window!\n";
		exit(1);
	}
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	mProgramProperties.mWindowWidth  = pWindowWidth;
	mProgramProperties.mWindowHeight = pWindowHeight;

	mProgramProperties.mWindow = SDL_CreateWindow("Batch Renderer", pWindowWidth, pWindowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!mProgramProperties.mWindow)
		std::cout << "Couldnt set the window!\n";
	mProgramProperties.mContext = SDL_GL_CreateContext(mProgramProperties.mWindow);
	if (!mProgramProperties.mContext)
		std::cout << "Couldnt set the context!\n";
	if (!SDL_GL_MakeCurrent(mProgramProperties.mWindow, mProgramProperties.mContext))
		std::cout << "Couldnt set the window and the context current!\n";
	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		std::cout << "Couldnt load the glad loader!\n";
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	mUI.init(mProgramProperties.mWindow, mProgramProperties.mContext);
}

Program::~Program()
{
	SDL_GL_DestroyContext(mProgramProperties.mContext);
	SDL_DestroyWindow(mProgramProperties.mWindow);
	SDL_Quit();
}

void Program::run()
{
	mProgramProperties.mResourcePath = RESOURCES_PATH;
	initAll();

	while (mProgramProperties.mProgIsRunning)
	{
		input();
		preDraw();
		draw();

		SDL_GL_SwapWindow(mProgramProperties.mWindow);
	}
}

void Program::input()
{
	while (SDL_PollEvent(&mProgramProperties.mEvent))
	{
		SDL_Event event = mProgramProperties.mEvent;

		ImGui_ImplSDL3_ProcessEvent(&event);

		if (event.type == SDL_EVENT_QUIT ||
			event.key.key == SDLK_ESCAPE)
		{
			mProgramProperties.mProgIsRunning = false;
			break;
		}

		// KEYS CHECKERS
		if (event.type == SDL_EVENT_KEY_DOWN)
			mProgramProperties.mKeyCodes[event.key.key] = true;
		if (event.type == SDL_EVENT_KEY_UP)
			mProgramProperties.mKeyCodes[event.key.key] = false;
		
		// CAMERA MOVING
		if (event.type == SDL_EVENT_MOUSE_MOTION && mProgramProperties.mTakeCursor)
			mProgramProperties.mCamera.mouseMovement(glm::vec2(event.motion.xrel / 5, event.motion.yrel / 5));
		
		// MOUSE WHEEL
		if (event.type == SDL_EVENT_MOUSE_WHEEL)
		{
			if (event.wheel.y > 0)
				mModelProperties.mDistanceFromCamera++;
			else if (event.wheel.y < 0)
				mModelProperties.mDistanceFromCamera--;
			mModelProperties.mDistanceFromCamera = std::clamp(mModelProperties.mDistanceFromCamera, 2.0f, 40.0f);
		}

		// MOUSE BUTTONS
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			mModelProperties.mFactoryMeshes.getMesh("block2").takeMesh(true);
		}
		if (event.button.button == SDL_BUTTON_RIGHT)
		{
			mModelProperties.mFactoryMeshes.getMesh("block2").takeMesh(false);
			mModelProperties.mLastPos = mModelProperties.mMousePos;
		}
	}

	controlScreen();
	controlCamera();
	controlModel();
	controlLight();
}

void Program::preDraw()
{
	float beginFrame = SDL_GetTicks();

	takerCursor();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	mUI.control(*this);
	
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight);
	if (mProgramProperties.mWhiteScreen)
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	else
		glClearColor(0.20f, 0.20f, 0.20f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setLights();
	setLightCube();
	setModels();

	mProgramProperties.mShader.setUnifrom1f("uTime", SDL_GetTicks() * 0.001f);

	float deltaTime = SDL_GetTicks() - beginFrame;
	if (deltaTime < 8)
		SDL_Delay(8 - deltaTime);
}

void Program::draw()
{
	ImGui::EndFrame();
	
	//mModel[0]->render(mShader);
	//mFactoryMeshes.render();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Program::initAll()
{
	initShader();
	initTextures();
	initPrimitives();
	initMeshes();
	initMaterial();
	initModels();
	initLights();

	/* SECRET :) */
	makeDiddyParty();
}

void Program::initShader()
{
	mProgramProperties.mShader.init(mProgramProperties.mResourcePath + "vert.glsl", 
									mProgramProperties.mResourcePath + "frag.glsl");
	mProgramProperties.mLightShader.init(mProgramProperties.mResourcePath + "vert.glsl", mProgramProperties.mResourcePath + "light.glsl");
}

void Program::initTextures()
{
	mModelProperties.mTextures.reserve(32);
	mModelProperties.mTextures.emplace_back(mProgramProperties.mResourcePath + "diddySwayed.png", "material.textures", true);
	mModelProperties.mTextures.emplace_back(mProgramProperties.mResourcePath + "diddy.png", "material.textures");
	//mModelProperties.mTextures.emplace_back(mProgramProperties.mResourcePath + "container2.png", "material.textures");
	//mModelProperties.mTextures.emplace_back(mProgramProperties.mResourcePath + "container2_specular.png", "material.textures");
	//mModelProperties.mTextures.emplace_back(mProgramProperties.mResourcePath + "matrix.jpg", "material.textures", true);
	//mModelProperties.mTextures.emplace_back(mProgramProperties.mResourcePath + "Fire_Texture_01.png", "material.textures", true);
}

void Program::initPrimitives()
{
	mModelProperties.mPrimitives.insert_or_assign("cube",	   std::make_shared<Cube>(mModelProperties.mTextures[0]));
	mModelProperties.mPrimitives.insert_or_assign("lightCube", std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	mModelProperties.mPrimitives.insert_or_assign("floor",     std::make_shared<Cube>(glm::vec4(0.0f, 0.2f, 0.0f, 1.0f)));
	mModelProperties.mPrimitives.insert_or_assign("block1",    std::make_shared<Cube>(glm::vec4(1.0f, 0.2f, 1.0f, 1.0f)));
	mModelProperties.mPrimitives.insert_or_assign("block2",    std::make_shared<Cube>(glm::vec4(0.2f, 1.0f, 1.0f, 1.0f)));
}

void Program::initMeshes()
{
	std::weak_ptr<Primitive> lightCube = mModelProperties.mPrimitives["lightCube"];
	std::weak_ptr<Primitive> floor	   = mModelProperties.mPrimitives["floor"];
	std::weak_ptr<Primitive> block1    = mModelProperties.mPrimitives["block1"];
	std::weak_ptr<Primitive> block2    = mModelProperties.mPrimitives["block2"];

 	mModelProperties.mFactoryMeshes.pushMesh("lightCube", std::make_unique<Mesh>(lightCube));
	mModelProperties.mFactoryMeshes.pushMesh("floor", std::make_unique<Mesh>(floor, mModelProperties.mTextures));
	mModelProperties.mFactoryMeshes.pushMesh("block1", std::make_unique<Mesh>(block1, mModelProperties.mTextures));
	mModelProperties.mFactoryMeshes.pushMesh("block2", std::make_unique<Mesh>(block2, mModelProperties.mTextures));
}

void Program::initMaterial()
{
	mMaterialProperties.mMaterial = std::make_unique<Material>(mMaterialProperties.mAmbient,
															   mMaterialProperties.mDiffuse,
															   mMaterialProperties.mSpecular,
															   32.0f, mModelProperties.mTextures);
}

void Program::initModels()
{
	mModelProperties.mModel.push_back(std::make_unique<Model>(glm::vec3(2.0f), mMaterialProperties.mMaterial.get(),
														      mProgramProperties.mResourcePath + "monkeyTri.obj", 
															  std::vector<Texture2>()));
	//mModelProperties.mModel.push_back(std::make_unique<Model>(glm::vec3(2.0f), mMaterialProperties.mMaterial.get(), 
	//															ProgramProperties.mResourcePath + "bunch.obj", 
	//															mModelProperties.mTextures));
}

void Program::initLights()
{
	mLightProperties.mPointLight.init(mLightProperties.mPosLight, true);

	PointLight pointLight0(mLightProperties.mPointLight.getPosLight(), false);
	pointLight0.setColor(mLightProperties.mLightColor);
	pointLight0.setIntensity(0.7f);

	glm::vec3 newPos = mLightProperties.mPointLight.getPosLight();
	newPos.x = mLightProperties.mPointLight.getPosLight().x + 30.0f;
	PointLight pointLight1(newPos, false);
	pointLight1.setColor(glm::vec3(1.0f, 0.0, 0.0f));
	pointLight0.setIntensity(0.7f);

	newPos.x = newPos.x + 30.0f;
	PointLight pointLight2(newPos, false);
	pointLight2.setColor(glm::vec3(0.0f, 1.0f, 0.0f));
	pointLight0.setIntensity(0.7f);
	

	mLightProperties.mStrgLights.reserve(3);
	mLightProperties.mStrgLights.push_back(std::move(pointLight0));
	mLightProperties.mStrgLights.push_back(std::move(pointLight1));
	mLightProperties.mStrgLights.push_back(std::move(pointLight2));
}

void Program::controlScreen()
{
	// WHITE/BLACK SCREEN
	if (mProgramProperties.mKeyCodes[SDLK_V])
		mProgramProperties.mWhiteScreen = true;
	if (mProgramProperties.mKeyCodes[SDLK_B])
		mProgramProperties.mWhiteScreen = false;

	// CURSOR
	if (mProgramProperties.mKeyCodes[SDLK_Z])
		mProgramProperties.mTakeCursor = !mProgramProperties.mTakeCursor;
}

void Program::controlCamera()
{
	float speedCamera = 0.05f;
	if (mProgramProperties.mKeyCodes[SDLK_LSHIFT])
		speedCamera = 0.1f;
	if (mProgramProperties.mKeyCodes[SDLK_W])
		mProgramProperties.mCamera.moveForward(speedCamera);
	if (mProgramProperties.mKeyCodes[SDLK_S])
		mProgramProperties.mCamera.moveBackward(speedCamera);
	if (mProgramProperties.mKeyCodes[SDLK_A])
		mProgramProperties.mCamera.moveLeft(speedCamera);
	if (mProgramProperties.mKeyCodes[SDLK_D])
		mProgramProperties.mCamera.moveRight(speedCamera);
	if (mProgramProperties.mKeyCodes[SDLK_LCTRL])
		mProgramProperties.mCamera.moveDown(speedCamera);
	if (mProgramProperties.mKeyCodes[SDLK_SPACE])
		mProgramProperties.mCamera.moveUp(speedCamera);
}

void Program::controlModel()
{
	// ROTATE DEGREE
	if (mProgramProperties.mKeyCodes[SDLK_R])
		mModelProperties.mRotateDegree -= 0.4f;
	if (mProgramProperties.mKeyCodes[SDLK_F])
		mModelProperties.mRotateDegree += 0.4f;
}

void Program::controlLight()
{
	// MOVING LIGHT
	float speedMovingLight = 0.1f;
	if (!mModelProperties.mRotatedMode)
	{
		if (mProgramProperties.mKeyCodes[SDLK_LEFT])
			mLightProperties.mPosLight.x -= speedMovingLight;
		if (mProgramProperties.mKeyCodes[SDLK_RIGHT])
			mLightProperties.mPosLight.x += speedMovingLight;
		if (mProgramProperties.mKeyCodes[SDLK_UP])
			mLightProperties.mPosLight.y += speedMovingLight;
		if (mProgramProperties.mKeyCodes[SDLK_DOWN])
			mLightProperties.mPosLight.y -= speedMovingLight;
		if (mProgramProperties.mKeyCodes[SDLK_RCTRL])
			mLightProperties.mPosLight.z -= speedMovingLight;
		if (mProgramProperties.mKeyCodes[SDLK_RSHIFT])
			mLightProperties.mPosLight.z += speedMovingLight;
	}
}

void Program::takerCursor()
{
	if (mProgramProperties.mTakeCursor)
	{
		SDL_WarpMouseInWindow(mProgramProperties.mWindow,
							  mProgramProperties.mWindowWidth / 2,
							  mProgramProperties.mWindowHeight / 2);
		SDL_SetWindowRelativeMouseMode(mProgramProperties.mWindow, true);
	}
	else
		SDL_SetWindowRelativeMouseMode(mProgramProperties.mWindow, false);
}

void Program::setLights()
{
	mLightProperties.mStrgLights[0].setPosLight(mLightProperties.mPointLight.getPosLight());
	mLightProperties.mLightColor.x = sin(SDL_GetTicks() * 2.0f * 0.005f);
	mLightProperties.mLightColor.y = sin(SDL_GetTicks() * 0.7f * 0.005f);
	mLightProperties.mLightColor.z = sin(SDL_GetTicks() * 1.3f * 0.005f);
	mLightProperties.mStrgLights[0].setColor(mLightProperties.mLightColor);

	setMaterials();

	mProgramProperties.mShader.bind();
	mMaterialProperties.mMaterial->sendToShader(mProgramProperties.mShader);
	mLightProperties.mStrgLights[0].sendToShaderArray(mProgramProperties.mShader, 0);
	mProgramProperties.mShader.setUniform3fv("point_light[0].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	mProgramProperties.mShader.setUniform3fv("point_light[0].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	mProgramProperties.mShader.setUniform3fv("point_light[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	mProgramProperties.mShader.setUnifrom1i("uNumberOfLights", 1);
}

void Program::setLightCube()
{
	mProgramProperties.mLightShader.bind();
	
	mModelProperties.mFactoryMeshes.getMesh("lightCube").initMVP(mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight, 
																 mProgramProperties.mCamera.getViewMatrix(),
																 mLightProperties.mStrgLights[0].getPosLight(), 
																 std::make_pair(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
																 glm::vec3(1.0f, 1.0f, 1.0f));
	mProgramProperties.mLightShader.setMatrixUniform4fv("uModel", mModelProperties.mFactoryMeshes.getMesh("lightCube").getModelMatrix());
	mProgramProperties.mLightShader.setMatrixUniform4fv("uMVP", mModelProperties.mFactoryMeshes.getMesh("lightCube").getMVP(false));
	mProgramProperties.mLightShader.setUniform3fv("uColor", mLightProperties.mStrgLights[0].getColor());

	mModelProperties.mFactoryMeshes.getMesh("lightCube").draw();
}

void Program::setMaterials()
{
	mMaterialProperties.mMaterial->setAmbient(mMaterialProperties.mAmbient);
	mMaterialProperties.mMaterial->setDiffuse(mMaterialProperties.mDiffuse);
	mMaterialProperties.mMaterial->setSpecular(mMaterialProperties.mSpecular);
	mMaterialProperties.mMaterial->setShines(mMaterialProperties.mShines);
}

void Program::setModels()
{

	// floor tex
 	mProgramProperties.mShader.bind();
	mModelProperties.mFactoryMeshes.getMesh("floor").initMVP(mProgramProperties.mWindowWidth, 
															 mProgramProperties.mWindowHeight, 
															 mProgramProperties.mCamera.getViewMatrix(),
															 glm::vec3(2.0f, 2.0f, -10.0f), std::make_pair(-90.0f, glm::vec3(1.0f, 0.0, 0.0f)),
															 glm::vec3(100.0f, 100.0f, 100.0f));
	mModelProperties.mFactoryMeshes.getMesh("floor").setUniforms(mProgramProperties.mCamera.getPos(), mProgramProperties.mCamera.getViewMatrix(), 
																 glm::vec4(0.4f, 0.6f, 1.0f, 1.0f), 
																 mProgramProperties.mShader, *mMaterialProperties.mMaterial);
	mLightProperties.mPointLight.setPosLight(mLightProperties.mPosLight);
	mModelProperties.mFactoryMeshes.getMesh("floor").draw();

	/* SECRET :)))) */
	setDiddyParty();

	//collide system check

	//// block 1
	//mProgramProperties.mShader.bind();
	//mModelProperties.mFactoryMeshes.getMesh("block1").initMVP(mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight, 
	//														  mProgramProperties.mCamera.getViewMatrix(),
	//														  mModelProperties.mPos1, std::make_pair(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
	//														  glm::vec3(1.0f, 1.0f, 1.0f));
	//mModelProperties.mFactoryMeshes.getMesh("block1").setUniforms(mProgramProperties.mCamera.getPos(), mProgramProperties.mCamera.getViewMatrix(),
	//															  glm::vec4(1.0f, 0.2f, 1.0f, 1.0f),
	//															  mProgramProperties.mShader, *mMaterialProperties.mMaterial);
	//mModelProperties.mFactoryMeshes.getMesh("block1").draw();


	//// block 2
	//if (mModelProperties.mFactoryMeshes.getMesh("block2").meshIsTaken())
	//{
	//	mModelProperties.mMousePos = mProgramProperties.mCamera.getPos() + mProgramProperties.mCamera.getDirection() * mModelProperties.mDistanceFromCamera;
	//	mModelProperties.mFactoryMeshes.getMesh("block2").initMVP(mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight,
	//															  mProgramProperties.mCamera.getViewMatrix(),
	//															  mModelProperties.mMousePos, std::make_pair(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
	//															  glm::vec3(1.0f, 1.0f, 1.0f));
	//}
	//else
	//{
	//	mModelProperties.mFactoryMeshes.getMesh("block2").initMVP(mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight,
	//															  mProgramProperties.mCamera.getViewMatrix(),
	//															  mModelProperties.mLastPos, std::make_pair(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
	//															  glm::vec3(1.0f, 1.0f, 1.0f));
	//}
	//mModelProperties.mFactoryMeshes.getMesh("block2").setUniforms(mProgramProperties.mCamera.getPos(), mProgramProperties.mCamera.getViewMatrix(), 
	//															  glm::vec4(0.2f, 1.0f, 1.0f, 1.0f),
	//															  mProgramProperties.mShader, *mMaterialProperties.mMaterial);
	//mModelProperties.mFactoryMeshes.getMesh("block2").draw();
}

void Program::makeDiddyParty()
{
	std::vector<std::weak_ptr<Primitive>> diddyHelpers;

	for (size_t i = 0; i < 15; ++i)
	{
		std::string diddyStr = "diddy" + std::to_string(i);
		mModelProperties.mPrimitives.insert_or_assign(diddyStr, std::make_shared<Cube>(glm::vec4(0.2f, 1.0f, 1.0f, 1.0f)));
		diddyHelpers.push_back(mModelProperties.mPrimitives[diddyStr]);
	}
	for (size_t i = 0; i < 15; ++i)
	{
		std::string diddyModel = "diddyModel" + std::to_string(i);
		mModelProperties.mFactoryMeshes.pushMesh(diddyModel, std::make_unique<Mesh>(diddyHelpers[i], mModelProperties.mTextures));
	}
}

void Program::setDiddyParty()
{
	std::array<glm::vec3, 15> diddyParty =
	{
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f),

		glm::vec3((rand() % 20 - 10) * 1.0f, (rand() % 20 - 10) * 1.0f, (rand() % 20 - 20) * 1.0f),
		glm::vec3((rand() % 20 - 10) * 1.0f, (rand() % 20 - 10) * 1.0f, (rand() % 20 - 20) * 1.0f),
		glm::vec3((rand() % 20 - 10) * 1.0f, (rand() % 20 - 10) * 1.0f, (rand() % 20 - 20) * 1.0f),
		glm::vec3((rand() % 20 - 10) * 1.0f, (rand() % 20 - 10) * 1.0f, (rand() % 20 - 20) * 1.0f),
		glm::vec3((rand() % 20 - 10) * 1.0f, (rand() % 20 - 10) * 1.0f, (rand() % 20 - 20) * 1.0f)
	};
	for (size_t i = 0; i < 15; ++i)
	{
		std::string diddyModel = "diddyModel" + std::to_string(i);
		mModelProperties.mFactoryMeshes.getMesh(diddyModel).initMVP(mProgramProperties.mWindowWidth, mProgramProperties.mWindowHeight,
																	mProgramProperties.mCamera.getViewMatrix(),
																	diddyParty[i], std::make_pair(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
																	glm::vec3(1.0f, 1.0f, 1.0f));
		mModelProperties.mFactoryMeshes.getMesh(diddyModel).setUniforms(mProgramProperties.mCamera.getPos(), mProgramProperties.mCamera.getViewMatrix(),
																		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
																		mProgramProperties.mShader, *mMaterialProperties.mMaterial);
		mModelProperties.mFactoryMeshes.getMesh(diddyModel).draw();
	}
}

void Program::debugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
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
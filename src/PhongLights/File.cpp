// vert shader

#version 440

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 posTex;

out vec3 fragPos;
out vec4 fragColor;
out vec2 fragPosTex;
out vec3 fragNormals;

uniform mat4 uMVP;
uniform mat4 uModel;
uniform mat4 uProj;
uniform mat4 uViewMatrix;

void main()
{
	gl_Position = uMVP * vec4(pos, 1.0);

	fragColor = color;
	fragPosTex = posTex;
	fragNormals = mat3(transpose(inverse(uModel))) * normals;
	fragPos = vec3(uModel * vec4(pos, 1.0f));
}



// frag shader

#version 440

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D matTexture[8];
	int index;
	int isJustColored;
};

struct pointLight
{
	vec3 position;
	vec3 color;
	float intensity;
	float constant;
	float linear;
	float quadratic;
};

in vec3 fragPos;
in vec4 fragColor;
in vec2 fragPosTex;
in vec3 fragNormals;
in vec3 lightPos;

out vec4 finalColor;

uniform Material material;
uniform pointLight point_light[3];
uniform int uNumberOfLights;
uniform vec3 cameraPos;

uniform vec3 uObjectColor;

void main()
{
	vec3 finalResult = vec3(0.0f);

	for(int i = 0; i < uNumberOfLights; ++i)
	{
		float ambientStrength = 0.1f; // than less - then darker will be the objects
		vec3 ambient = ambientStrength * point_light[i].color * point_light[i].intensity;

		vec3 norm = normalize(fragNormals);
		vec3 lightDir = normalize(point_light[i].position - fragPos);
		float diff = max(dot(norm, lightDir), 0.0f);
		vec3 diffuse = diff * point_light[i].color * point_light[i].intensity;

		float specularStrength = 0.5f;
		vec3 viewDir = normalize(fragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
		vec3 specular = specularStrength * spec * point_light[i].color * point_light[i].intensity;

		finalResult += ambient + diffuse + specular;
	}

	vec3 result = finalResult * uObjectColor;
	finalColor = vec4(result, 1.0);
}

// main.cpp

#include "Program.h"

Program::Program(uint32_t pWindowWidth, uint32_t pWindowHeight)
	: mWindowWidth(pWindowWidth),
	  mWindowHeight(pWindowHeight)
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

	mWindow = SDL_CreateWindow("Batch Renderer", mWindowWidth, mWindowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!mWindow)
		std::cout << "Couldnt set the window!\n";
	mContext = SDL_GL_CreateContext(mWindow);
	if (!mContext)
		std::cout << "Couldnt set the context!\n";
	if (!SDL_GL_MakeCurrent(mWindow, mContext))
		std::cout << "Couldnt set the window and the context current!\n";
	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		std::cout << "Couldnt load the glad loader!\n";
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	mUI.init(mWindow, mContext);
}

Program::~Program()
{
	SDL_GL_DestroyContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Program::run()
{
	std::string resourcePath = RESOURCES_PATH;
	mShader.init(resourcePath + "vert.glsl", resourcePath + "frag.glsl");
	mLightShader.init(resourcePath + "vert.glsl", resourcePath + "light.glsl");

	//Textures
	std::vector<Texture2> textures;
	textures.reserve(32);
	textures.emplace_back(resourcePath + "broken_brick_wall_diff_1k.jpg", "material.matTexture");
	textures.emplace_back(resourcePath + "broken_brick_wall_spec_1k.jpg", "material.matTexture");

	// Primitives
	std::shared_ptr<Primitive> quad = std::make_shared<Quad>(textures[0]);
	std::shared_ptr<Primitive> pyramid = std::make_shared<Cube>(textures[0]);
	std::shared_ptr<Primitive> lightCube = std::make_shared<Cube>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	std::shared_ptr<Primitive> floor = std::make_unique<Quad>(glm::vec4(0.0f, 0.2f, 0.0f, 1.0f));

	// Meshes
	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(mOBJLoader.getVertices(), std::vector<uint32_t>({}), textures);
	mFactoryMeshes.pushMesh("lightCube", std::make_unique<Mesh>(std::weak_ptr<Primitive>(lightCube)));
	mFactoryMeshes.pushMesh("mesh", std::move(mesh));
	mFactoryMeshes.pushMesh("floor", std::make_unique<Mesh>(std::weak_ptr<Primitive>(floor)));

	// Material
	mMaterial = std::make_unique<Material>(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(0.1f), textures);
	
	// Model
	//mModel.push_back(std::make_unique<Model>(glm::vec3(2.0f), mMaterial.get(), resourcePath + "Cube.obj", textures));
    mModel.push_back(std::make_unique<Model>(glm::vec3(2.0f), mMaterial.get(), resourcePath + "sphere.obj", textures));
	//mModel.push_back(std::make_unique<Model>(glm::vec3(2.0f), mMaterial.get(), resourcePath + "Brick.obj", textures));
	//mModel.push_back(std::make_unique<Model>(glm::vec3(2.0f), mMaterial.get(), resourcePath + "monkey.obj", textures));
	//mModel.push_back(std::make_unique<Model>(glm::vec3(2.0f), mMaterial.get(), resourcePath + "triangleCylind.obj", textures));
	//mModel.push_back(std::make_unique<Model>(glm::vec3(2.0f), mMaterial.get(), resourcePath + "monkeyTri.obj", textures));
	//mModel.push_back(std::make_unique<Model>(glm::vec3(2.0f), mMaterial.get(), resourcePath + "bunch.obj", textures));
	//mModel.push_back(std::make_unique<Model>(glm::vec3(2.0f), mMaterial.get(), resourcePath + "fpsBreaker.obj", textures));

	// Light
	mPointLight.init(mPosLight, true);

	PointLight pointLight0(mPointLight.getPosLight(), false);
	pointLight0.setColor(mLightColor);
	pointLight0.setIntensity(0.7f);

	glm::vec3 newPos = mPointLight.getPosLight();
	newPos.x = mPointLight.getPosLight().x + 30.0f;
	PointLight pointLight1(newPos, false);
	pointLight1.setColor(glm::vec3(1.0f, 0.0, 0.0f));
	pointLight0.setIntensity(0.7f);

	newPos.x = newPos.x + 30.0f;
	PointLight pointLight2(newPos, false);
	pointLight2.setColor(glm::vec3(0.0f, 1.0f, 0.0f));
	pointLight0.setIntensity(0.7f);

	mStrgLights.reserve(3);
	mStrgLights.push_back(std::move(pointLight0));
	mStrgLights.push_back(std::move(pointLight1));
	mStrgLights.push_back(std::move(pointLight2));

	while (mProgIsRunning)
	{
		input();
		preDraw();
		draw();

		SDL_GL_SwapWindow(mWindow);
	}
}

void Program::input()
{
	while (SDL_PollEvent(&mEvent))
	{
		ImGui_ImplSDL3_ProcessEvent(&mEvent);

		if (mEvent.type == SDL_EVENT_QUIT ||
			mEvent.key.key == SDLK_ESCAPE)
		{
			mProgIsRunning = false;
			break;
		}
		if (mEvent.type == SDL_EVENT_KEY_DOWN)
			mKeyCodes[mEvent.key.key] = true;
		if (mEvent.type == SDL_EVENT_KEY_UP)
			mKeyCodes[mEvent.key.key] = false;
		if (mEvent.type == SDL_EVENT_MOUSE_MOTION && mTakeCursor)
			mCamera.mouseMovement(glm::vec2(mEvent.motion.xrel / 5, mEvent.motion.yrel / 5));
	}

	// WHITE/BLACK SCREEN
	if (mKeyCodes[SDLK_V])
		mWhiteScreen = true;
	if (mKeyCodes[SDLK_B])
		mWhiteScreen = false;

	// CURSOR
	if (mKeyCodes[SDLK_Z])
		mTakeCursor = !mTakeCursor;

	// CAMERA
	float speedCamera = 0.05f;
	if (mKeyCodes[SDLK_LSHIFT])
		speedCamera = 0.1f;
	if (mKeyCodes[SDLK_W])
		mCamera.moveForward(speedCamera);
	if (mKeyCodes[SDLK_S])
		mCamera.moveBackward(speedCamera);
	if (mKeyCodes[SDLK_A])
		mCamera.moveLeft(speedCamera);
	if (mKeyCodes[SDLK_D])
		mCamera.moveRight(speedCamera);
	if (mKeyCodes[SDLK_LCTRL])
		mCamera.moveDown(speedCamera);
	if (mKeyCodes[SDLK_SPACE])
		mCamera.moveUp(speedCamera);

	// ROTATE DEGREE
	if (mKeyCodes[SDLK_R])
		mRotateDegree -= 0.4f;
	if (mKeyCodes[SDLK_F])
		mRotateDegree += 0.4f;
	
	// MOVING LIGHT
	float speedMovingLight = 0.1f;
	if (!mRotatedMode)
	{
		if (mKeyCodes[SDLK_LEFT])
			mPosLight.x -= speedMovingLight;
		if (mKeyCodes[SDLK_RIGHT])
			mPosLight.x += speedMovingLight;
		if (mKeyCodes[SDLK_UP])
			mPosLight.y += speedMovingLight;
		if (mKeyCodes[SDLK_DOWN])
			mPosLight.y -= speedMovingLight;
		if (mKeyCodes[SDLK_RCTRL])
			mPosLight.z -= speedMovingLight;
		if (mKeyCodes[SDLK_RSHIFT])
			mPosLight.z += speedMovingLight;
	}
}

void Program::preDraw()
{
	float beginFrame = SDL_GetTicks();

	if (mTakeCursor)
	{
		SDL_WarpMouseInWindow(mWindow, mWindowWidth / 2, mWindowHeight / 2);
		SDL_SetWindowRelativeMouseMode(mWindow, true);
	}
	else
		SDL_SetWindowRelativeMouseMode(mWindow, false);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	mUI.control(*this);
	
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, mWindowWidth, mWindowHeight);
	if (mWhiteScreen)
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	else
		glClearColor(0.20f, 0.20f, 0.20f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	PointLight pointLight0(mPointLight.getPosLight(), false);
	pointLight0.setColor(mLightColor);
	pointLight0.setIntensity(0.7f);

	mStrgLights[0].setPosLight(mPointLight.getPosLight());
	mStrgLights[0].setColor(mLightColor);

	glm::vec3 newPos = mPointLight.getPosLight();
	newPos.x = mPointLight.getPosLight().x + 30.0f;
	mStrgLights[1].setPosLight(newPos);

	newPos.x = newPos.x + 30.0f;
	mStrgLights[2].setPosLight(newPos);

	mShader.bind();
	for (size_t i = 0; i < mStrgLights.size(); ++i)
	{
		mStrgLights[i].sendToShaderArray(mShader, i);
	}
	mShader.setUnifrom1i("uNumberOfLights", mStrgLights.size());

	mLightShader.bind();
	for (size_t i = 0; i < mStrgLights.size(); ++i)
	{
		mFactoryMeshes.getMesh("lightCube").initMVP(mWindowWidth, mWindowHeight, mCamera.getViewMatrix(),
													mStrgLights[i].getPosLight(), std::make_pair(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
													glm::vec3(1.0f, 1.0f, 1.0f));

		mLightShader.setMatrixUniform4fv("uModel", mFactoryMeshes.getMesh("lightCube").getModelMatrix());
		mLightShader.setMatrixUniform4fv("uMVP", mFactoryMeshes.getMesh("lightCube").getMVP(false));
		mLightShader.setUniform3fv("uColor", mStrgLights[i].getColor());

		mFactoryMeshes.getMesh("lightCube").draw();
	}

	mShader.bind();
	mFactoryMeshes.getMesh("floor").initMVP(mWindowWidth, mWindowHeight, mCamera.getViewMatrix(),
											glm::vec3(1.0f, 1.0f, 1.0f), std::make_pair(-90.0f, glm::vec3(1.0f, 0.0, 0.0f)),
											glm::vec3(100.0f, 100.0f, 100.0f));

	mShader.setUniform3fv("uObjectColor", glm::vec3(0.4f, 0.6f, 1.0f));
	mShader.setUniform3fv("cameraPos", mCamera.getPos());
	mShader.setMatrixUniform4fv("uView", mCamera.getViewMatrix());
	mShader.setMatrixUniform4fv("uModel", mFactoryMeshes.getMesh("floor").getModelMatrix());
	mShader.setMatrixUniform4fv("uMVP", mFactoryMeshes.getMesh("floor").getMVP(false));
	
	mFactoryMeshes.getMesh("floor").draw();

	mModel[0]->initMVP(mWindowWidth, mWindowHeight, mCamera.getViewMatrix(),
					   glm::vec3(1.0f, 1.0f, 1.0f), std::make_pair(45.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
					   glm::vec3(1.0f, 1.0f, 1.0f));
	

	if (mRotatedMode)
	{
		float angle = SDL_GetTicks() * 0.001f;

		mPosLight.x = posX + 2.0f * cos(angle);
		mPosLight.y = 5.0f;
		mPosLight.z = posZ + 2.0f * sin(angle);
	}
	
	mPointLight.setPosLight(mPosLight);
	mModel[0]->setCurrentPos(mPosLight);

	float deltaTime = SDL_GetTicks() - beginFrame;
	if (deltaTime < 8)
		SDL_Delay(8 - deltaTime);
}

void Program::draw()
{
	ImGui::EndFrame();

	mModel[0]->render(mShader);
	//mFactoryMeshes.render();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

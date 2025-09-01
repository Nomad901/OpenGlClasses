#include "UI.h"

UI::~UI()
{
	ImGui_ImplSDL3_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void UI::init(SDL_Window* pWindow, SDL_GLContext pContext)
{
	IMGUI_CHECKVERSION();   
	ImGui::CreateContext(); 
	ImGuiIO& io = ImGui::GetIO(); 

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.IniFilename = NULL;

	ImGui::StyleColorsDark();
	ImGui_ImplSDL3_InitForOpenGL(pWindow, pContext);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void UI::control(uint32_t pType, Mesh3D& pMesh3D)
{
	auto initHelper = [&]
		{
			pMesh3D.clean();
			switch (pType)
			{
			case 0:
				pMesh3D.initTriangle();
				break;
			case 1:
				pMesh3D.initQuad();
				break;
			case 2:
				pMesh3D.initCube();
				break;
			default:
				break;
			}
		};
	static bool firstTime = true;
	static int32_t i = 0;
	if (firstTime)
	{
		ImGui::SetNextWindowPos({ 0,0 });
		ImGui::SetNextWindowSize({ 720,250 });
		ImGui::SetNextWindowCollapsed(true);
		firstTime = false;
	}
	static bool quit = false;
	ImGui::Begin("I am Quad", &quit, ImGuiFocusedFlags_None);

	for (int32_t i = 0; i < std::ssize(pMesh3D.mColorsStrg); ++i)
	{
		if (ImGui::ColorEdit3(("Color of the object. Vertex " + std::to_string(i)).c_str(), pMesh3D.mColorsStrg[i].data()))
		{
			initHelper();
		}
	}

	ImGui::Spacing();

	if (ImGui::SliderFloat("Width", &pMesh3D.mWidth, 0.1f, 1.0f))
		initHelper();
	if (ImGui::SliderFloat("Height", &pMesh3D.mHeight, 0.1f, 1.0f))
		initHelper();

	ImGui::End();	
}

#include "UI.h"
#include "Program.h"

UI::UI(SDL_Window* pWindow, const SDL_GLContext& pContext)
{
	init(pWindow, pContext);
}

UI::~UI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void UI::init(SDL_Window* pWindow, const SDL_GLContext& pContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.IniFilename = nullptr;

	ImGui::StyleColorsDark();
	ImGui_ImplSDL3_InitForOpenGL(pWindow, pContext);
	ImGui_ImplOpenGL3_Init("#version 440");
}

void UI::control(Program& pProgram)
{
	static bool firstTime = true;
	if (firstTime)
	{
		ImGui::SetNextWindowPos({ 1, 1 });
		ImGui::SetNextWindowSize({ 400, 550 });
		ImGui::SetNextWindowCollapsed(true);
		firstTime = false;
	}

	ImGui::Begin("Control", &pProgram.mProgramProperties.mProgIsRunning, ImGuiFocusedFlags_None);
	
	ImGui::SliderFloat("R", &pProgram.mLightProperties.mLightColor.x, 0.0f, 1.0f);
	ImGui::SliderFloat("G", &pProgram.mLightProperties.mLightColor.y, 0.0f, 1.0f);
	ImGui::SliderFloat("B", &pProgram.mLightProperties.mLightColor.z, 0.0f, 1.0f);

	ImGui::Spacing();

	ImGui::SliderFloat("Ambient R", &pProgram.mMaterialProperties.mAmbient.x, 0.0f, 1.0f);
	ImGui::SliderFloat("Ambient G", &pProgram.mMaterialProperties.mAmbient.y, 0.0f, 1.0f);
	ImGui::SliderFloat("Ambient B", &pProgram.mMaterialProperties.mAmbient.z, 0.0f, 1.0f);

	ImGui::Spacing();

	ImGui::SliderFloat("Diffuse R", &pProgram.mMaterialProperties.mDiffuse.x, 0.0f, 1.0f);
	ImGui::SliderFloat("Diffuse G", &pProgram.mMaterialProperties.mDiffuse.y, 0.0f, 1.0f);
	ImGui::SliderFloat("Diffuse B", &pProgram.mMaterialProperties.mDiffuse.z, 0.0f, 1.0f);

	ImGui::Spacing();

	ImGui::SliderFloat("Specular R", &pProgram.mMaterialProperties.mSpecular.x, 0.0f, 1.0f);
	ImGui::SliderFloat("Specular G", &pProgram.mMaterialProperties.mSpecular.y, 0.0f, 1.0f);
	ImGui::SliderFloat("Specular B", &pProgram.mMaterialProperties.mSpecular.z, 0.0f, 1.0f);

	ImGui::Spacing();

	ImGui::SliderFloat("Scale of shines", &pProgram.mMaterialProperties.mShines, 0.0f, 256.0f);

	ImGui::Spacing();

	ImGui::SliderFloat("Pos block1 X", &pProgram.mModelProperties.mPos1.x, 0.0f, 100.0f);
	ImGui::SliderFloat("Pos block1 Y", &pProgram.mModelProperties.mPos1.y, 0.0f, 100.0f);
	ImGui::SliderFloat("Pos block1 Z", &pProgram.mModelProperties.mPos1.z, 0.0f, 100.0f);
	
	ImGui::SliderFloat("Pos block2 X", &pProgram.mModelProperties.mPos2.x, 0.0f, 100.0f);
	ImGui::SliderFloat("Pos block2 Y", &pProgram.mModelProperties.mPos2.y, 0.0f, 100.0f);
	ImGui::SliderFloat("Pos block2 Z", &pProgram.mModelProperties.mPos2.z, 0.0f, 100.0f);

	ImGui::Spacing();

	ImGui::End();
}


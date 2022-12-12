#include "UIManager.h"
#include "..\imgui\imgui_impl_glfw.h"
#include "..\imgui\imgui_impl_opengl3.h"
#include "OpenGLDevice.h"
#include "Define.h"
#include "LuaBrain.h"

#include <sstream>
#include <iomanip>

SINGLETON_FUNCTION(UIManager)
USING(Engine)
USING(ImGui)
USING(std)

UIManager::UIManager()
{
	m_pBrain = LuaBrain::GetInstance(); m_pBrain->AddRefCnt();
	ZeroMemory(m_chText, sizeof(m_chText));
}

UIManager::~UIManager()
{
}

void UIManager::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	DestroyContext();

	SafeDestroy(m_pBrain);
}

void UIManager::RenderUI()
{
	if (nullptr == m_pBrain)
		return;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	NewFrame();

	ImVec2 screen = ImVec2((_float)COpenGLDevice::GetInstance()->GetWidthSize(), (_float)COpenGLDevice::GetInstance()->GetHeightSize());
	SetNextWindowPos(ImVec2(0.f, 0.f));
	SetNextWindowSize(screen);
	if (Begin("Game Info", (bool*)0,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar))
	{
		Text("Lua File");

		Text("File Path: "); SameLine(100.f); Text(m_pBrain->GetPath().c_str());
		Text("File Name: "); SameLine(100.f); Text(m_pBrain->GetFileName().c_str());

		Text(" "); SameLine(100.f);
		SetNextItemWidth(250.f);
		InputText("##fileName", m_chText, sizeof(m_chText));
		SameLine(370.f);
		if (Button("Load", ImVec2(50.f, 0.f)))
		{
			string str(m_chText);
			if (str.size() > 0)
			{
				m_pBrain->LoadLuaFile(str);
			}
		}

		Text(" "); SameLine(100.f); Text(m_pBrain->GetErrorStr().c_str());
	}
	End();

	Render();
	ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
}

RESULT UIManager::Ready()
{
	IMGUI_CHECKVERSION();
	CreateContext();
	ImGuiIO& io = GetIO();

	if (!ImGui_ImplGlfw_InitForOpenGL(COpenGLDevice::GetInstance()->GetWindow(), true) ||
		!ImGui_ImplOpenGL3_Init("#version 460"))
		return PK_ERROR_IMGUI;
	StyleColorsDark();

	return PK_NOERROR;
}
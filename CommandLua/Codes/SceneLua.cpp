#include "SceneLua.h"
#include "Function.h"
#include "InputDevice.h"
#include "DefaultCamera.h"
#include <sstream>
#include <atlconv.h>
#include "XMLParser.h"
#include "Layer.h"
#include "ComponentMaster.h"
#include "Component.h"
#include "Shader.h"
#include "LightMaster.h"
#include "Object.h"
#include "Define.h"
#include "UIManager.h"
#include "Enums.h"
#include "SkyBox.h"
#include "Renderer.h"

#include "LuaBrain.h"
#include "CommandManager.h"

USING(Engine)
USING(glm)
USING(std)

SceneLua::SceneLua()
	: m_pDefaultCamera(nullptr), m_pObjLayer(nullptr), m_pSkyBox(nullptr)
{
	m_pInputDevice = CInputDevice::GetInstance(); m_pInputDevice->AddRefCnt();
	m_pInputDevice->SetMouseSensitivity(0.05f);
	m_pUIManager = UIManager::GetInstance(); m_pUIManager->AddRefCnt();
	m_pCommandManager = CommandManager::GetInstance(); m_pCommandManager->AddRefCnt();
	m_pLuaBrain = LuaBrain::GetInstance(); m_pLuaBrain->AddRefCnt();

	wchar_t path[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, path, MAX_PATH);
	USES_CONVERSION;
	std::string str = W2A(path);
	str = str.substr(0, str.find_last_of("\\/"));//
	stringstream ss;
	ss << str << "\\..\\";

	m_DataPath = ss.str();
	m_ObjListFileName = "SceneLua_mapObjects.xml";
	m_LightListFileName = "SceneLua_lights.xml";
}

SceneLua::~SceneLua()
{
}

void SceneLua::AddGameObject(string objName, string meshName, string layer, vec3 vPos, vec3 vRot, vec3 vScale)
{
	CLayer* targetLayer = nullptr;

	if ("bg" == layer)
		targetLayer = GetLayer((_uint)LAYER_BACKGROUND);
	else
		targetLayer = m_pObjLayer;

	CGameObject* pGameObject = nullptr;

	pGameObject = Object::Create((_uint)SCENE_FPS, targetLayer->GetTag()
		, (_uint)OBJ_BACKGROUND, targetLayer, meshName, objName, vPos, vRot, vScale);
	AddGameObjectToLayer(targetLayer->GetTag(), pGameObject);
}

Object* SceneLua::FindObjectByName(std::string objName)
{
	CGameObject* pGameObject = m_pObjLayer->FindGameObjectByObjName(objName);
	if (nullptr != pGameObject)
		return dynamic_cast<Object*>(pGameObject);

	CLayer* bgLayer = GetLayer((_uint)LAYER_BACKGROUND);
	pGameObject = bgLayer->FindGameObjectByObjName(objName);
	if (nullptr != pGameObject)
		return dynamic_cast<Object*>(pGameObject);

	return nullptr;
}

_bool SceneLua::CheckLocationTrigger(glm::vec3 vPos, _float radius)
{
	list<CGameObject*>* pListObj = m_pObjLayer->GetObjectList();
	list<CGameObject*>::iterator iter;
	for (iter = pListObj->begin(); iter != pListObj->end(); ++iter)
	{
		vec3 vObjPos = (*iter)->GetPosition();
		if (radius >= distance(vPos, vObjPos))
			return true;
	}

	return false;
}

void SceneLua::RemoveAllObjects()
{
	m_pObjLayer->RemoveAllGameObject();

	CLayer* bgLayer = GetLayer((_uint)LAYER_BACKGROUND);
	bgLayer->RemoveAllGameObject();
}

void SceneLua::KeyCheck()
{
	//static _bool isF1Down = false;
	//if (m_pInputDevice->IsKeyDown(GLFW_KEY_F1))
	//{
	//	if (!isF1Down)
	//	{
	//		isF1Down = true;

	//		if (SHOT_MODE == m_pPhysicsSystem->GetGameMode())
	//			m_pPhysicsSystem->SetGameMode(HOVER_MODE);
	//		else
	//			m_pPhysicsSystem->SetGameMode(SHOT_MODE);
	//	}
	//}
	//else
	//	isF1Down = false;
}

void SceneLua::Update(const _float& dt)
{
	if (nullptr != m_pSkyBox)
		CRenderer::GetInstance()->AddRenderObj(m_pSkyBox);

	CLightMaster::GetInstance()->SetLightInfo();

	KeyCheck();

	//if (nullptr != masterComm)
	//	masterComm->Update(dt);
	if (nullptr != m_pCommandManager)
		m_pCommandManager->Update(dt);

	CScene::Update(dt);
}

void SceneLua::Render()
{
	if (nullptr != m_pUIManager)
		m_pUIManager->RenderUI();
}

void SceneLua::Destroy()
{
	SafeDestroy(m_pInputDevice);
	SafeDestroy(m_pUIManager);
	SafeDestroy(m_pSkyBox);
	SafeDestroy(m_pLuaBrain);
	SafeDestroy(m_pCommandManager);

	//if (nullptr != masterComm)
	//	delete masterComm;

	CScene::Destroy();
}

RESULT SceneLua::Ready()
{
	RESULT result = PK_NOERROR;
	result = ReadyLayerAndGameObject();
	if (PK_NOERROR != result)
		return result;

	// Light
	CComponent* shader = CComponentMaster::GetInstance()->FindComponent("DefaultShader");
	_uint shaderID = 0;
	if (nullptr != shader)
		shaderID = dynamic_cast<CShader*>(shader)->GetShaderProgram();
	CLightMaster::GetInstance()->SetShader(shaderID);
	CLightMaster::GetInstance()->LoadLights(m_DataPath, m_LightListFileName);

	if (nullptr != m_pDefaultCamera)
		m_pDefaultCamera->SetShaderLocation(shaderID);

	if (nullptr != m_pUIManager)
		m_pUIManager->Ready();

	if (nullptr == m_pSkyBox)
	{
		stringstream ss, ss2;
		ss << m_DataPath << "Assets\\textures\\SkyBox\\";

		vector<string> faces;
		ss2.str(""); ss2 << ss.str() << "right.jpg"; faces.push_back(ss2.str());
		ss2.str(""); ss2 << ss.str() << "left.jpg"; faces.push_back(ss2.str());
		ss2.str(""); ss2 << ss.str() << "top.jpg"; faces.push_back(ss2.str());
		ss2.str(""); ss2 << ss.str() << "bottom.jpg"; faces.push_back(ss2.str());
		ss2.str(""); ss2 << ss.str() << "front.jpg"; faces.push_back(ss2.str());
		ss2.str(""); ss2 << ss.str() << "back.jpg"; faces.push_back(ss2.str());

		CComponent* skyboxShader = CComponentMaster::GetInstance()->FindComponent("SkyBoxShader");
		m_pSkyBox = CSkyBox::Create(faces, dynamic_cast<CShader*>(skyboxShader));
	}

	//if (nullptr != masterComm)
	//	masterComm->Run();
	m_pLuaBrain->Ready(this);
	m_pCommandManager->Ready(this);

	return PK_NOERROR;
}

RESULT SceneLua::ReadyLayerAndGameObject()
{
	//Create.Camera
	CLayer* pLayer = GetLayer((_uint)LAYER_CHARACTER);
	if (nullptr != pLayer)
	{
		vec3 vPos = vec3(0.f, 0.f, 0.f);
		vec3 vRot = vec3(0.f, 0.f, 0.f);
		vec3 vScale = vec3(1.f);
		CGameObject* pGameObject = DefaultCamera::Create((_uint)SCENE_FPS, pLayer->GetTag(), (_uint)OBJ_CAMERA, pLayer,
			vPos, vRot, vScale, 0.6f, 0.1f, 1000.f);
		if (nullptr != pGameObject)
		{
			AddGameObjectToLayer(pLayer->GetTag(), pGameObject);
			m_pDefaultCamera = dynamic_cast<DefaultCamera*>(pGameObject);
		}
	}

	//Create.BackgroundLayer 
	LoadBackgroundObjects();

	return PK_NOERROR;
}

SceneLua* SceneLua::Create()
{
	SceneLua* pInstance = new SceneLua();
	if (PK_NOERROR != pInstance->Ready())
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}

void SceneLua::LoadBackgroundObjects()
{
	m_pObjLayer = GetLayer((_uint)LAYER_OBJECT);
	CGameObject* pGameObject = nullptr;

	if (nullptr != m_pObjLayer)
	{
		m_pObjLayer->RemoveAllGameObject();
		vector<CXMLParser::sObjectData> vecObjects;
		CXMLParser::sObjectData cameraData;
		CXMLParser::GetInstance()->LoadMapObjectData(m_DataPath, m_ObjListFileName, vecObjects, cameraData);
		vector<CXMLParser::sObjectData>::iterator iter;
		for (iter = vecObjects.begin(); iter != vecObjects.end(); ++iter)
		{
			pGameObject = Object::Create((_uint)SCENE_FPS, m_pObjLayer->GetTag(), (_uint)OBJ_BACKGROUND, m_pObjLayer
				, iter->ID, "", iter->POSITION, iter->ROTATION, iter->SCALE);
			if (nullptr == pGameObject)
				continue;
			AddGameObjectToLayer(m_pObjLayer->GetTag(), pGameObject);
		}
		vecObjects.clear(); 

		if (nullptr != m_pDefaultCamera)
		{
			m_pDefaultCamera->SetCameraEye(cameraData.POSITION);
			m_pDefaultCamera->SetCameraRot(cameraData.ROTATION);
			m_pDefaultCamera->SetCameraTarget(cameraData.SCALE);
		}
	}
}
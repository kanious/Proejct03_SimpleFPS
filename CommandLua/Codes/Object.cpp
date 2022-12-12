#include "Object.h"
#include "ComponentMaster.h"
#include "Layer.h"

#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Renderer.h"
#include "BoundingBox.h"
#include "OpenGLDefines.h"
#include "OpenGLDevice.h"
#include "SoundMaster.h"
#include "LightMaster.h"
#include "Shader.h"
#include "Command.h"


USING(Engine)
USING(glm)
USING(std)

_uint Object::NEXT_ID = 10;

Object::Object()
	: m_pMesh(nullptr), m_pSubObj(nullptr), m_bSub(false), m_bRot(false)
{
	m_bDebug = false;
	m_iUniqueID = Object::NEXT_ID;
	++Object::NEXT_ID;
}

Object::~Object()
{
}

void Object::AddCommand(CommandType type, Command* pComm)
{
	if (nullptr == pComm)
		return;

	m_queueCommands[type].push(pComm);
}

void Object::StartRotatingSubPart()
{
	if (nullptr != m_pSubObj)
		m_pSubObj->SetRot(true);
}

void Object::StopRotatingSubPart()
{
	if (nullptr != m_pSubObj)
		m_pSubObj->SetRot(false);
}

void Object::Update(const _float& dt)
{
	if (!m_bEnable)
		return;

	if (nullptr == m_pTransform)
		return;

	for (int i = 0; i < Command_End; ++i)
	{
		if (0 >= m_queueCommands[i].size())
			continue;

		if (nullptr == m_pCurComm[i])
		{
			m_pCurComm[i] = m_queueCommands[i].front();
			m_pCurComm[i]->SetControlStart();
		}

		if (m_pCurComm[i]->isControlEnd())
		{
			m_pCurComm[i]->SetCommandFinish();
			m_pCurComm[i] = nullptr;
			m_queueCommands[i].pop();
			if (0 < m_queueCommands[i].size())
			{
				m_pCurComm[i] = m_queueCommands[i].front();
				m_pCurComm[i]->SetControlStart();
			}
		}
	}

	if (nullptr != m_pSubObj)
		m_pSubObj->Update(dt);

	if (m_bRot)
	{
		//m_pTransform->AddRotationY(dt * 500.f);
		vec3 vRev = m_pTransform->GetRevolve();
		vRev.z += dt * 500.f;
		if (vRev.z > 360.f)
			vRev.z -= 360.f; 
		m_pTransform->SetRevolve(vRev);
	}

	CGameObject::Update(dt);

	if (nullptr != m_pRenderer)
		m_pRenderer->AddRenderObj(this);
}

void Object::Render()
{
	if (nullptr != m_pSubObj)
		m_pSubObj->Render();

	CGameObject::Render();
}

void Object::Destroy()
{
	for (int i = 0; i < Command_End; ++i)
	{
		while (0 < m_queueCommands[i].size())
			m_queueCommands[i].pop();
	}

	SafeDestroy(m_pSubObj);

	CGameObject::Destroy();
}

RESULT Object::Ready(_uint sTag, _uint lTag, _uint oTag, CLayer* pLayer, string meshID, string objName, vec3 vPos, vec3 vRot, vec3 vScale, _bool sub)
{
	SetupGameObject(sTag, lTag, oTag);
	m_pLayer = pLayer;
	m_meshName = meshID;
	m_objName = objName;
	m_bSub = sub;

	//Clone.Mesh
 	m_pMesh = CloneComponent<CMesh*>(meshID);
	if (nullptr != m_pMesh)
	{
		AttachComponent("Mesh", m_pMesh);
		m_pMesh->SetTransform(m_pTransform);
		m_pBoundingBox = m_pMesh->GetBoundingBox();
		if (nullptr != m_pBoundingBox)
			m_pBoundingBox->SetTransform(m_pTransform);
		m_pMesh->SetWireFrame(false);
		m_pMesh->SetDebugBox(false);
	}

	if ((_uint)LAYER_OBJECT == m_pLayer->GetTag() && !sub)
	{
		m_pSubObj = Object::Create(0, 0, 0, m_pLayer, "Sphere", "SubObj", vec3(0.0f, 0.2f, 0.3f), vec3(0.f), vec3(0.01f), true);
		if (nullptr != m_pSubObj)
			m_pSubObj->SetParentTransform(m_pTransform);
	}

	if (nullptr != m_pTransform)
	{
		m_pTransform->SetPosRotScale(vPos, vRot, vScale);
		m_pTransform->Update(0);
	}

	return PK_NOERROR;
}

Object* Object::Create(_uint sTag, _uint lTag, _uint oTag, CLayer* pLayer, string meshID, string objName, vec3 vPos, vec3 vRot, vec3 vScale, _bool sub)
{
	Object* pInstance = new Object();
	if (PK_NOERROR != pInstance->Ready(sTag, lTag, oTag, pLayer, meshID, objName, vPos, vRot, vScale, sub))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}

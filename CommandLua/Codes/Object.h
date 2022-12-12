#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "GameObject.h"
#include "EngineStruct.h"
#include "Enums.h"
#include <queue>

namespace Engine
{
	class CMesh;
}
class Command;

class Object : public Engine::CGameObject
{
private:
	Engine::CMesh*					m_pMesh;
	_uint							m_iUniqueID;
	static _uint					NEXT_ID;

	std::queue<Command*>			m_queueCommands[Command_End];
	Command*						m_pCurComm[Command_End];

	Object*							m_pSubObj;
	_bool							m_bSub;
	_bool							m_bRot;

private:
	explicit Object();
	virtual ~Object();

public:
	_uint GetID()						{ return m_iUniqueID; }
	void SetRot(_bool value)			{ m_bRot = value; }

public:
	void AddCommand(CommandType type, Command* pComm);
	void StartRotatingSubPart();
	void StopRotatingSubPart();

public:
	virtual void Update(const _float& dt);
	virtual void Render();
private:
	virtual void Destroy();
	RESULT Ready(_uint sTag, _uint lTag, _uint oTag, Engine::CLayer* pLayer, std::string meshID,
		std::string objName, glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale, _bool sub);
public:
	static Object* Create(_uint sTag, _uint lTag, _uint oTag, Engine::CLayer* pLayer, std::string meshID,
		std::string objName, glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale, _bool sub = false);
};

#endif //_OBJECT_H_
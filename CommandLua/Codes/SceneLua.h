#ifndef _SCENE3DSOUND_H_
#define _SCENE3DSOUND_H_

#include "Scene.h"
#include "glm/vec3.hpp"

namespace Engine
{
	class CInputDevice;
	class CLayer;
	class CSkyBox;
}
class UIManager;
class DefaultCamera;
class LuaBrain;
class CommandManager;
class Object;

class SceneLua : public Engine::CScene
{
private:
	Engine::CInputDevice*	m_pInputDevice;
	UIManager*				m_pUIManager;

	DefaultCamera*			m_pDefaultCamera;
	Engine::CLayer*			m_pObjLayer;

	Engine::CSkyBox*		m_pSkyBox;

	//Object* m_pTestObj;
	//Object* m_pTestObj2;
	//Object* m_pTestObj3;
	LuaBrain*				m_pLuaBrain;
	CommandManager*			m_pCommandManager;

private:
	explicit SceneLua();
	virtual ~SceneLua();
	
public:
	void AddGameObject(std::string objName, std::string meshName, std::string layer, glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale);
	Object* FindObjectByName(std::string objName);
	_bool CheckLocationTrigger(glm::vec3 vPos, _float radius);
	void RemoveAllObjects();

private:
	void KeyCheck();

public:
	virtual void Update(const _float& dt);
	virtual void Render();
private:
	virtual void Destroy();

private:
	RESULT Ready();
	RESULT ReadyLayerAndGameObject();
public:
	static SceneLua* Create();
	virtual void LoadBackgroundObjects();
};

#endif //_SCENE3DSOUND_H_
#ifndef _COMMANDMANAGER_H_
#define _COMMANDMANAGER_H_

#include "Base.h"
#include "Enums.h"
#include "glm/vec3.hpp"

class SceneLua;
class Command;
class Object;

class CommandManager : public Engine::CBase
{
	SINGLETON(CommandManager)
private:
	struct sTriggerCondition
	{
		glm::vec3 pos = glm::vec3(0.f);
		_float radius = 0.f;;
		Command* onComm = nullptr;
		Command* offComm = nullptr;
		_bool turnOnExecuted = false;
		_bool turnOffExecuted = true;
	};

private:
	SceneLua*							m_pScene;
	std::list<Command*>					m_listParallelCommand;
	std::vector<sTriggerCondition*>		m_vecLocTriggers;
	Command*							m_pMasterCommand;

	typedef std::unordered_map<std::string, Command*> MAP_COMMAND;
	MAP_COMMAND							m_mapCommands;

private:
	explicit CommandManager();
	~CommandManager();

public:
	_bool RegistMasterCommand(std::string name);
	_bool CreateCommandGroup(std::string name, CommandType type, _uint repeat = 1);
	_bool AddCommandGroupToGroup(std::string srcName, std::string destName);
	_bool AddCommandMoveTo(std::string commName, EaseType type, Object* target
		, glm::vec3 destPos, _float fTime, _float in, _float out);
	_bool AddCommandOrientTo(std::string commName, EaseType type, Object* target
		, glm::vec3 destRot, glm::vec3 dirRot, _float fTime, _float in, _float out);
	_bool AddCommandFollowCurve(std::string commName, EaseType type, Object* target
		, glm::vec3 destPos, glm::vec3 viaPos, _float fTime, _float in, _float out, _bool isBezier);
	_bool AddCommandFollowObject(std::string commName, Object* target, Object* follow, _float maxSpeed
		, _float followTime, _float decelerateDist, _float stopDist);
	_bool AddLocationBasedTrigger(glm::vec3 targetPos, _float radius, std::string onName
		, std::string offName);
	_bool AddCommandPlaySound(std::string commName, std::string soundTag);
	_bool AddCommandRotSubPart(std::string commName, Object* target, _float fTime);
	_bool AddCommandStay(std::string commName, Object* target, _float fTime);

public:
	void ResetAllCommand();
	void Run();

public:
	void AddParallelCommand(Command* pCommand);

public:
	void Destroy();
	void Update(const _float& dt);
	RESULT Ready(SceneLua* pScene);
};

#endif //_COMMANDMANAGER_H_
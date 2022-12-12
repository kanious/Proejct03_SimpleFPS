#include "CommandManager.h"
#include "OpenGLDevice.h"
#include "Define.h"
#include "Object.h"
#include "SceneLua.h"

#include "Command.h"
#include "CommandSerial.h"
#include "CommandParallel.h"
#include "CommandMoveTo.h"
#include "CommandOrientTo.h"
#include "CommandFollowCurve.h"
#include "CommandFollowObject.h"
#include "CommandPlaySound.h"
#include "CommandRotSubPart.h"
#include "CommandStay.h"

SINGLETON_FUNCTION(CommandManager)
USING(Engine)
USING(glm)
USING(std)

CommandManager::CommandManager()
	: m_pMasterCommand(nullptr), m_pScene(nullptr)
{
	m_listParallelCommand.clear();
	m_mapCommands.clear();
	m_vecLocTriggers.clear();
}

CommandManager::~CommandManager()
{
}

_bool CommandManager::RegistMasterCommand(std::string name)
{
	MAP_COMMAND::iterator iter = m_mapCommands.find(name);
	if (iter == m_mapCommands.end())
		return false;

	m_pMasterCommand = iter->second;

	return true;
}

_bool CommandManager::CreateCommandGroup(std::string name, CommandType type, _uint repeat)
{
	MAP_COMMAND::iterator iter = m_mapCommands.find(name);
	if (iter != m_mapCommands.end())
		return false;

	Command* newCommand = nullptr;
	if (Command_Serial == type)
		newCommand = new CommandSerial(Command_Serial, name, repeat);
	else
		newCommand = new CommandParallel(Command_Parallel, name);

	m_mapCommands.insert(MAP_COMMAND::value_type(name, newCommand));

	return true;
}

_bool CommandManager::AddCommandGroupToGroup(std::string srcName, std::string destName)
{
	MAP_COMMAND::iterator iterScr = m_mapCommands.find(srcName);
	MAP_COMMAND::iterator iterDest = m_mapCommands.find(destName);
	if (iterScr == m_mapCommands.end() || iterDest == m_mapCommands.end())
		return false;

	iterDest->second->AddCommand(iterScr->second);

	return true;
}

_bool CommandManager::AddCommandMoveTo(string commName, EaseType type, Object* target, vec3 destPos, _float fTime, _float in, _float out)
{
	MAP_COMMAND::iterator iterComm = m_mapCommands.find(commName);
	if (iterComm == m_mapCommands.end())
		return false;

	Command* newComm = new CommandMoveTo(type, target, destPos, fTime, in, out);
	iterComm->second->AddCommand(newComm);

	return true;
}

_bool CommandManager::AddCommandOrientTo(std::string commName, EaseType type, Object* target, vec3 destRot, vec3 dirRot, _float fTime, _float in, _float out)
{
	MAP_COMMAND::iterator iterComm = m_mapCommands.find(commName);
	if (iterComm == m_mapCommands.end())
		return false;

	Command* newComm = new CommandOrientTo(type, target, destRot, dirRot, fTime, in, out);
	iterComm->second->AddCommand(newComm);

	return true;
}

_bool CommandManager::AddCommandFollowCurve(std::string commName, EaseType type, Object* target, glm::vec3 destPos, glm::vec3 viaPos, _float fTime, _float in, _float out, _bool isBezier)
{
	MAP_COMMAND::iterator iterComm = m_mapCommands.find(commName);
	if (iterComm == m_mapCommands.end())
		return false;

	Command* newComm = new CommandFollowCurve(type, target, destPos, viaPos, fTime, in, out, isBezier);
	iterComm->second->AddCommand(newComm);

	return true;
}

_bool CommandManager::AddCommandFollowObject(string commName, Object* target, Object* follow, _float maxSpeed, _float followTime, _float decelerateDist, _float stopDist)
{
	MAP_COMMAND::iterator iterComm = m_mapCommands.find(commName);
	if (iterComm == m_mapCommands.end())
		return false;

	Command* newComm = new CommandFollowObject(target, follow, maxSpeed, followTime, decelerateDist, stopDist);
	iterComm->second->AddCommand(newComm);

	return true;
}

_bool CommandManager::AddLocationBasedTrigger(vec3 targetPos, _float radius, string onName, string offName)
{
	MAP_COMMAND::iterator iterCommOn = m_mapCommands.find(onName);
	MAP_COMMAND::iterator iterCommOff = m_mapCommands.find(offName);
	if (iterCommOn == m_mapCommands.end() || iterCommOff == m_mapCommands.end())
		return false;

	sTriggerCondition* condition = new sTriggerCondition;
	condition->pos = targetPos;
	condition->radius = radius;
	condition->onComm = iterCommOn->second;
	condition->offComm = iterCommOff->second;
	condition->turnOnExecuted = false;
	condition->turnOffExecuted = true;

	m_vecLocTriggers.push_back(condition);

	return true;
}

_bool CommandManager::AddCommandPlaySound(string commName, string soundTag)
{
	MAP_COMMAND::iterator iterComm = m_mapCommands.find(commName);
	if (iterComm == m_mapCommands.end())
		return false;

	Command* newComm = new CommandPlaySound(soundTag);
	iterComm->second->AddCommand(newComm);

	return true;
}

_bool CommandManager::AddCommandRotSubPart(string commName, Object* target, _float fTime)
{
	MAP_COMMAND::iterator iterComm = m_mapCommands.find(commName);
	if (iterComm == m_mapCommands.end())
		return false;

	Command* newComm = new CommandRotSubPart(target, fTime); 
	iterComm->second->AddCommand(newComm);

	return true;
}

_bool CommandManager::AddCommandStay(string commName, Object* target, _float fTime)
{
	MAP_COMMAND::iterator iterComm = m_mapCommands.find(commName);
	if (iterComm == m_mapCommands.end())
		return false;

	Command* newComm = new CommandStay(target, fTime);
	iterComm->second->AddCommand(newComm);

	return true;
}

void CommandManager::ResetAllCommand()
{
	if (nullptr != m_pMasterCommand)
		delete m_pMasterCommand;
	m_pMasterCommand = nullptr;

	vector<sTriggerCondition*>::iterator iterT;
	for (iterT = m_vecLocTriggers.begin(); iterT != m_vecLocTriggers.end(); ++iterT)
	{
		delete (*iterT)->onComm;
		delete (*iterT)->offComm;
		delete (*iterT);
	}
	m_vecLocTriggers.clear();

	m_listParallelCommand.clear();
	m_mapCommands.clear();
}

void CommandManager::Run()
{
	if (nullptr != m_pMasterCommand)
		m_pMasterCommand->Run();
}

void CommandManager::AddParallelCommand(Command* pCommand)
{
	if (nullptr != pCommand && Command_Parallel == pCommand->GetCommandType())
		m_listParallelCommand.push_back(pCommand);
}

void CommandManager::Destroy()
{
	if (nullptr != m_pMasterCommand)
		delete m_pMasterCommand;
	m_pMasterCommand = nullptr;

	vector<sTriggerCondition*>::iterator iterT;
	for (iterT = m_vecLocTriggers.begin(); iterT != m_vecLocTriggers.end(); ++iterT)
	{
		delete (*iterT)->onComm;
		delete (*iterT)->offComm;
		delete (*iterT);
	}
	m_vecLocTriggers.clear();
}

void CommandManager::Update(const _float& dt)
{
	if (nullptr != m_pMasterCommand)
		m_pMasterCommand->Update(dt);

	list<Command*>::iterator iter;
	for (iter = m_listParallelCommand.begin(); iter != m_listParallelCommand.end(); )
	{
		if ((*iter)->isControlEnd())
			m_listParallelCommand.erase(iter++);
		else
		{
			(*iter)->Update(dt);
			++iter;
		}
	}

	for (int i = 0; i < m_vecLocTriggers.size(); ++i)
	{
		sTriggerCondition* pCondition = m_vecLocTriggers[i];
		if (m_pScene->CheckLocationTrigger(pCondition->pos, pCondition->radius))
		{
			pCondition->turnOffExecuted = false;
			if (!pCondition->turnOnExecuted)
			{
				pCondition->turnOnExecuted = true;
				if (pCondition->onComm->isFinished())
					pCondition->onComm->Run();
			}
		}
		else
		{
			pCondition->turnOnExecuted = false;
			if (!pCondition->turnOffExecuted)
			{
				pCondition->turnOffExecuted = true;
				if (pCondition->offComm->isFinished())
					pCondition->offComm->Run();
			}
		}
	}
}

RESULT CommandManager::Ready(SceneLua* pScene)
{
	m_pScene = pScene;

	return PK_NOERROR;
}
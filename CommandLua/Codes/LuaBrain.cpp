#include "LuaBrain.h"
#include "SceneLua.h"
#include "CommandManager.h"
#include "glm\vec3.hpp"

#include <sstream>
#include <atlconv.h>
#include <fstream>

SINGLETON_FUNCTION(LuaBrain)
USING(Engine)
USING(std)
USING(glm)

SceneLua* LuaBrain::m_pScene = nullptr;
CommandManager* LuaBrain::m_pCommandManager = nullptr;

LuaBrain::LuaBrain()
	: m_pLuaState(nullptr), m_path(""), m_fileName(""), m_errorStr("")
{
	m_pCommandManager = CommandManager::GetInstance(); m_pCommandManager->AddRefCnt();
	m_mapScripts.clear();
}

LuaBrain::~LuaBrain()
{
}

void LuaBrain::LoadScript(string scriptName, string scriptSource)
{
	m_mapScripts[scriptName] = scriptSource;
}

void LuaBrain::DeleteScript(string scriptName)
{
	m_mapScripts.erase(scriptName);
}

_bool LuaBrain::RunScriptImmediately(string script)
{
	int error = luaL_loadstring(m_pLuaState, script.c_str());

	if (0 != error)
	{
		cout << "Error LOADING Lua script: " << DecodeLuaErrorToString(error) << endl;
		return false;
	}

	error = lua_pcall(m_pLuaState, 0, 0, 0);
	if (0 != error)
	{
		cout << "Error RUNNING Lua script: " << DecodeLuaErrorToString(error);
		cout << "(" << lua_tostring(m_pLuaState, -1) << ")" << endl;
		lua_pop(m_pLuaState, 1);
		return false;
	}

	return true;
}

int LuaBrain::Lua_CreateObject(lua_State* L)
{
	string objName = lua_tostring(L, 1);
	string meshName = lua_tostring(L, 2);

	vec3 vPos(0.f);
	vPos.x = (_float)lua_tonumber(L, 3);
	vPos.y = (_float)lua_tonumber(L, 4);
	vPos.z = (_float)lua_tonumber(L, 5);

	vec3 vRot(0.f);
	vRot.x = (_float)lua_tonumber(L, 6);
	vRot.y = (_float)lua_tonumber(L, 7);
	vRot.z = (_float)lua_tonumber(L, 8);

	vec3 vScale(1.f);
	vScale.x = (_float)lua_tonumber(L, 9);
	vScale.y = (_float)lua_tonumber(L, 10);
	vScale.z = (_float)lua_tonumber(L, 11);

	string layerName = lua_tostring(L, 12);

	m_pScene->AddGameObject(objName, meshName, layerName, vPos, vRot, vScale);

	lua_pushboolean(L, true);
	return 1;
}

int LuaBrain::Lua_RegistMasterCommand(lua_State* L)
{
	string masterName = lua_tostring(L, 1);
	_bool result = m_pCommandManager->RegistMasterCommand(masterName);

	lua_pushboolean(L, result);
	return 1;
}

int LuaBrain::Lua_CreateCommandGroup(lua_State* L)
{
	string commandName = lua_tostring(L, 1);
	string typeStr = lua_tostring(L, 2);
	_int repeat = (_int)lua_tonumber(L, 3);

	CommandType commandType = Command_Serial;
	if ("parallel" == typeStr)
		commandType = Command_Parallel;

	_bool result = m_pCommandManager->CreateCommandGroup(commandName, commandType, repeat);

	lua_pushboolean(L, result);
	return 1;
}

int LuaBrain::Lua_AddCommandGroupToGroup(lua_State* L)
{
	string srcName = lua_tostring(L, 1);
	string destName = lua_tostring(L, 2);

	_bool result = m_pCommandManager->AddCommandGroupToGroup(srcName, destName);

	lua_pushboolean(L, result);
	return 1;
}

int LuaBrain::Lua_AddCommandMoveTo(lua_State* L)
{
	string commandName = lua_tostring(L, 1);
	string typestr = lua_tostring(L, 2);
	string objName = lua_tostring(L, 3);

	vec3 vDestPos(0.f);
	vDestPos.x = (_float)lua_tonumber(L, 4);
	vDestPos.y = (_float)lua_tonumber(L, 5);
	vDestPos.z = (_float)lua_tonumber(L, 6);

	_float fTime = (_float)lua_tonumber(L, 7);
	_float fIn = (_float)lua_tonumber(L, 8);
	_float fOut = (_float)lua_tonumber(L, 9);

	EaseType easeType = Ease_Time;
	if ("ease_distance" == typestr)
		easeType = Ease_Distance;

	Object* targetObj = m_pScene->FindObjectByName(objName);
	if (nullptr == targetObj)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	_bool result = m_pCommandManager->AddCommandMoveTo(commandName, easeType, targetObj
		, vDestPos, fTime, fIn, fOut);

	lua_pushboolean(L, result);
	return 1;
}

int LuaBrain::Lua_AddCommandOrientTo(lua_State* L)
{
	string commandName = lua_tostring(L, 1);
	string typestr = lua_tostring(L, 2);
	string objName = lua_tostring(L, 3);

	vec3 vDestRot(0.f);
	vDestRot.x = (_float)lua_tonumber(L, 4);
	vDestRot.y = (_float)lua_tonumber(L, 5);
	vDestRot.z = (_float)lua_tonumber(L, 6);

	vec3 vDirRot(0.f);
	vDirRot.x = (_float)lua_tonumber(L, 7);
	vDirRot.y = (_float)lua_tonumber(L, 8);
	vDirRot.z = (_float)lua_tonumber(L, 9);

	_float fTime = (_float)lua_tonumber(L, 10);
	_float fIn = (_float)lua_tonumber(L, 11);
	_float fOut = (_float)lua_tonumber(L, 12);

	EaseType easeType = Ease_Time;
	if ("ease_distance" == typestr)
		easeType = Ease_Distance;

	Object* targetObj = m_pScene->FindObjectByName(objName);
	if (nullptr == targetObj)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	_bool result = m_pCommandManager->AddCommandOrientTo(commandName, easeType, targetObj
		, vDestRot, vDirRot, fTime, fIn, fOut);

	lua_pushboolean(L, result);
	return 1;
}

int LuaBrain::Lua_AddCommandFollowCurve(lua_State* L)
{
	string commandName = lua_tostring(L, 1);
	string typestr = lua_tostring(L, 2);
	string objName = lua_tostring(L, 3);

	vec3 vDestPos(0.f);
	vDestPos.x = (_float)lua_tonumber(L, 4);
	vDestPos.y = (_float)lua_tonumber(L, 5);
	vDestPos.z = (_float)lua_tonumber(L, 6);

	vec3 vViaPos(0.f);
	vViaPos.x = (_float)lua_tonumber(L, 7);
	vViaPos.y = (_float)lua_tonumber(L, 8);
	vViaPos.z = (_float)lua_tonumber(L, 9);

	_float fTime = (_float)lua_tonumber(L, 10);
	_float fIn = (_float)lua_tonumber(L, 11);
	_float fOut = (_float)lua_tonumber(L, 12);

	_bool bBezier = lua_toboolean(L, 13);

	EaseType easeType = Ease_Time;
	if ("ease_distance" == typestr)
		easeType = Ease_Distance;

	Object* targetObj = m_pScene->FindObjectByName(objName);
	if (nullptr == targetObj)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	_bool result = m_pCommandManager->AddCommandFollowCurve(commandName, easeType, targetObj
		, vDestPos, vViaPos, fTime, fIn, fOut, bBezier);

	lua_pushboolean(L, result);
	return 1;
}

int LuaBrain::Lua_AddCommandFollowObject(lua_State* L)
{
	string commandName = lua_tostring(L, 1);
	string objName = lua_tostring(L, 2);
	string followObjName = lua_tostring(L, 3);

	_float maxSpeed = (_float)lua_tonumber(L, 4);
	_float followTime = (_float)lua_tonumber(L, 5);
	_float decelerateDist = (_float)lua_tonumber(L, 6);
	_float stopDist = (_float)lua_tonumber(L, 7);

	Object* targetObj = m_pScene->FindObjectByName(objName);
	Object* followObj = m_pScene->FindObjectByName(followObjName);
	if (nullptr == targetObj || nullptr == followObj)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	_bool result = m_pCommandManager->AddCommandFollowObject(commandName, targetObj, followObj
		, maxSpeed, followTime, decelerateDist, stopDist);

	lua_pushboolean(L, result);
	return 1;
}

int LuaBrain::Lua_AddTrigger(lua_State* L)
{
	vec3 vPos(0.f);
	vPos.x = (_float)lua_tonumber(L, 1);
	vPos.y = (_float)lua_tonumber(L, 2);
	vPos.z = (_float)lua_tonumber(L, 3);

	_float radius = (_float)lua_tonumber(L, 4);
	
	string onName = lua_tostring(L, 5);
	string offName = lua_tostring(L, 6);

	_bool result = m_pCommandManager->AddLocationBasedTrigger(vPos, radius, onName, offName);

	lua_pushboolean(L, result);
	return 1;
}

int LuaBrain::Lua_AddCommandPlaySound(lua_State* L)
{
	string commandName = lua_tostring(L, 1);
	string soundTag = lua_tostring(L, 2);

	_bool result = m_pCommandManager->AddCommandPlaySound(commandName, soundTag);

	lua_pushboolean(L, result);
	return 1;
}

int LuaBrain::Lua_AddCommandRotSubPart(lua_State* L)
{
	string commandName = lua_tostring(L, 1);
	string objName = lua_tostring(L, 2);
	_float time = (_float)lua_tonumber(L, 3);

	Object* targetObj = m_pScene->FindObjectByName(objName);
	if (nullptr == targetObj)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	_bool result = m_pCommandManager->AddCommandRotSubPart(commandName, targetObj, time);

	lua_pushboolean(L, result);
	return 1;
}

int LuaBrain::Lua_AddCommandStay(lua_State* L)
{
	string commandName = lua_tostring(L, 1);
	string objName = lua_tostring(L, 2);
	_float time = (_float)lua_tonumber(L, 3);

	Object* targetObj = m_pScene->FindObjectByName(objName);
	if (nullptr == targetObj)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	_bool result = m_pCommandManager->AddCommandStay(commandName, targetObj, time);

	lua_pushboolean(L, result);
	return 1;
}

void LuaBrain::LoadLuaFile(string fileName)
{
	if (nullptr == m_pCommandManager || nullptr == m_pScene)
		return;

	stringstream ss;
	ss << m_path << fileName;

	ifstream luaFile(ss.str());
	if (!luaFile.is_open())
	{
		ss.str("");
		ss << "( Cannot find this file: ## " << fileName << " )";
		m_errorStr = ss.str();
		return;
	}

	m_fileName = fileName;
	m_errorStr = "";
	m_pCommandManager->ResetAllCommand();
	m_pScene->RemoveAllObjects();

	string nextLine;
	while (getline(luaFile, nextLine))
	{
		RunScriptImmediately(nextLine);
	}

	m_pCommandManager->Run();
}

string LuaBrain::DecodeLuaErrorToString(int error)
{
	switch (error)
	{
	case 0:
		return "Lua: no error";
		break;
	case LUA_ERRSYNTAX:
		return "Lua: syntax error";
		break;
	case LUA_ERRMEM:
		return "Lua: memory allocation error";
		break;
	case LUA_ERRRUN:
		return "Lua: Runtime error";
		break;
	case LUA_ERRERR:
		return "Lua: Error while running the error handler function";
		break;
	}

	return "Lua: UNKNOWN error";
}

void LuaBrain::Destroy()
{
	lua_close(m_pLuaState);

	SafeDestroy(m_pCommandManager);
	m_pScene = nullptr;
}

void LuaBrain::Update(const _float& dt)
{
	MAP_SCRIPT::iterator iter;
	for (iter = m_mapScripts.begin(); iter != m_mapScripts.end(); ++iter)
	{
		if (!RunScriptImmediately(iter->second))
			continue;
	}
}

RESULT LuaBrain::Ready(SceneLua* pScene)
{
	m_pLuaState = luaL_newstate();
	m_pScene = pScene;

	lua_pushcfunction(m_pLuaState, Lua_CreateObject);
	lua_setglobal(m_pLuaState, "CreateObject");

	lua_pushcfunction(m_pLuaState, Lua_RegistMasterCommand);
	lua_setglobal(m_pLuaState, "RegistMasterCommand");

	lua_pushcfunction(m_pLuaState, Lua_CreateCommandGroup);
	lua_setglobal(m_pLuaState, "CreateCommandGroup");

	lua_pushcfunction(m_pLuaState, Lua_AddCommandGroupToGroup);
	lua_setglobal(m_pLuaState, "AddCommandGroupToGroup");

	lua_pushcfunction(m_pLuaState, Lua_AddCommandMoveTo);
	lua_setglobal(m_pLuaState, "AddCommandMoveTo");

	lua_pushcfunction(m_pLuaState, Lua_AddCommandOrientTo);
	lua_setglobal(m_pLuaState, "AddCommandOrientTo");

	lua_pushcfunction(m_pLuaState, Lua_AddCommandFollowCurve);
	lua_setglobal(m_pLuaState, "AddCommandFollowCurve");

	lua_pushcfunction(m_pLuaState, Lua_AddCommandFollowObject);
	lua_setglobal(m_pLuaState, "AddCommandFollowObject");

	lua_pushcfunction(m_pLuaState, Lua_AddTrigger);
	lua_setglobal(m_pLuaState, "AddTrigger");

	lua_pushcfunction(m_pLuaState, Lua_AddCommandPlaySound);
	lua_setglobal(m_pLuaState, "AddCommandPlaySound");

	lua_pushcfunction(m_pLuaState, Lua_AddCommandRotSubPart);
	lua_setglobal(m_pLuaState, "AddCommandRotSubPart");

	lua_pushcfunction(m_pLuaState, Lua_AddCommandStay);
	lua_setglobal(m_pLuaState, "AddCommandStay");


	wchar_t path[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, path, MAX_PATH);
	USES_CONVERSION;
	std::string str = W2A(path);
	str = str.substr(0, str.find_last_of("\\/"));
	stringstream ss;
	ss << str << "\\..\\Assets\\";
	m_path = ss.str();

	LoadLuaFile("SampleCommand_moveTo.Lua");

	return PK_NOERROR;
}

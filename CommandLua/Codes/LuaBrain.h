#ifndef _LUABRAIN_H_
#define _LUABRAIN_H_

extern "C"
{
#include <Lua5.4.4/lua.h>
#include <Lua5.4.4/lualib.h>
#include <Lua5.4.4/lauxlib.h>
}
#include "Base.h"
#include "Enums.h"

class SceneLua;
class CommandManager;
class LuaBrain : public Engine::CBase
{
	SINGLETON(LuaBrain)
private:
	typedef std::unordered_map<std::string, std::string> MAP_SCRIPT;
	MAP_SCRIPT								m_mapScripts;
	lua_State*								m_pLuaState;
	std::string								m_path;
	std::string								m_fileName;
	std::string								m_errorStr;

	static SceneLua*						m_pScene;
	static CommandManager*					m_pCommandManager;

private:
	explicit LuaBrain();
	~LuaBrain();

public:
	void LoadScript(std::string scriptName, std::string scriptSource);
	void DeleteScript(std::string scriptName);
	_bool RunScriptImmediately(std::string script);

	static int Lua_CreateObject(lua_State* L);
	static int Lua_RegistMasterCommand(lua_State* L);
	static int Lua_CreateCommandGroup(lua_State* L);
	static int Lua_AddCommandGroupToGroup(lua_State* L);
	static int Lua_AddCommandMoveTo(lua_State* L);
	static int Lua_AddCommandOrientTo(lua_State* L);
	static int Lua_AddCommandFollowCurve(lua_State* L);
	static int Lua_AddCommandFollowObject(lua_State* L);
	static int Lua_AddTrigger(lua_State* L);
	static int Lua_AddCommandPlaySound(lua_State* L);
	static int Lua_AddCommandRotSubPart(lua_State* L);
	static int Lua_AddCommandStay(lua_State* L);

	void LoadLuaFile(std::string fileName);

public:
	std::string GetPath()		{ return m_path; }
	std::string GetFileName()	{ return m_fileName; }
	std::string GetErrorStr()	{ return m_errorStr; }
	
private:
	std::string DecodeLuaErrorToString(int error);

public:
	void Destroy();
	void Update(const _float& dt);
	RESULT Ready(SceneLua* pScene);
};

#endif //_LUABRAIN_H_
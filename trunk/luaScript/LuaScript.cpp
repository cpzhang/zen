#include "LuaScript.h"
#include "lua/src/lua.hpp"
#include "misc/FileSystem.h"
//extern int tolua_LuaScript_open (lua_State* tolua_S);
LuaScript::LuaScript()
{
	state_ = luaL_newstate(); /* Open Lua */
	luaL_openlibs(state_);
	//tolua_LuaScript_open(state_);
}

LuaScript::~LuaScript()
{
	
}

void LuaScript::doString( const tstring& s )
{
	luaL_dostring(state_, s.c_str());
}

void LuaScript::doFile( const tstring& s )
{
	tstring fp(s);
	if (!FileSystem::isFileExist(fp))
	{
		fp = FileSystem::getDataDirectory() + s;
		if (!FileSystem::isFileExist(fp))
		{
			return;
		}
	}
	luaL_dofile(state_, fp.c_str());
}

void LuaScript::destroy()
{
	lua_close(state_);
}

lua_State* LuaScript::getLuaState()
{
	return state_;
}

ApiLuaScript_ LuaScript* createLuaScript()
{
	new LuaScript;
	return LuaScript::getInstancePtr();
}

ApiLuaScript_ LuaScript* getLuaScript()
{
	return LuaScript::getInstancePtr();
}

ApiLuaScript_ void destroyLuaScript()
{
	if (getLuaScript())
	{
		getLuaScript()->destroy();
		delete LuaScript::getInstancePtr();
	}
}

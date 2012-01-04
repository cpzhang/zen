#include "LuaScript.h"
#include "lua/src/lua.hpp"
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

void LuaScript::doString( const std::string& s )
{
	luaL_dostring(state_, s.c_str());
}

void LuaScript::doFile( const std::string& s )
{
	luaL_dofile(state_, s.c_str());
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

#pragma once
#include <string>
#include "misc/Singleton.h"
#include "ConfigLuaScript.h"
struct lua_State;
class ApiLuaScript_ LuaScript : public Singleton<LuaScript>
{
public:
	LuaScript();
	~LuaScript();
public:
	void doString(const tstring& s);
	void doFile(const tstring& s);
	void destroy();
	lua_State* getLuaState();
private:
	lua_State* state_;
};

ApiLuaScript_ LuaScript* createLuaScript();

ApiLuaScript_ LuaScript* getLuaScript();

ApiLuaScript_ void destroyLuaScript();
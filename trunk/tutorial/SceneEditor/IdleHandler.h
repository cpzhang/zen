#pragma once
// idle handler
class IdleHandler
{
#define InterfaceIdleHandler(terminal)\
public:\
	virtual void onIdle() ##terminal\
	virtual void onRefreshLuaScript() ##terminal

#define IdleHandler_Base InterfaceIdleHandler(=0;)
#define IdleHandler_Derived InterfaceIdleHandler(;)
	IdleHandler_Base
};

#pragma once
#include "IState.h"
#include "misc/Singleton.h"
class StateManager : public Singleton<StateManager>
{
public:
	StateManager();
public:
	void create();
	void destroy();
	void gotoState(eState e);
	void update();
private:
	IState* createState_(eState e);
private:
	eState current_;
	IState* states_[eState_Size];
};

StateManager* createStateManager();
StateManager* getStateManager();
void destroyStateManager();;
#pragma once
#include "IState.h"
#include "misc/Singleton.h"
#include "PlaceModelState.h"
class StateManager : public Singleton<StateManager>
{
public:
	StateManager();
public:
	void create();
	void destroy();
	void gotoState(eState e);
	void update();
	IState* getState(eState e);
	eState getCurrentState();
	PlaceModelState* getPlaceModelState();
private:
	IState* createState_(eState e);
private:
	eState current_;
	IState* states_[eState_Size];
};

StateManager* createStateManager();
StateManager* getStateManager();
void destroyStateManager();;
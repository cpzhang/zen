#include "StateManager.h"
void StateManager::create()
{
	for (size_t i = 0; i != eState_Size; ++i)
	{
		states_[i] = createState_((eState)i);
	}
}

void StateManager::destroy()
{
	for (size_t i = 0; i != eState_Size; ++i)
	{
		IState* s = states_[i];
		if (s)
		{
			s->destroy();
			delete s;
			states_[i] = 0;
		}
	}
}

IState* StateManager::createState_( eState e )
{
	IState* s = 0;
	switch(e)
	{
	case eState_None:
		s = new IState;
		break;
	}
	return s;
}

void StateManager::gotoState( eState e )
{
	if (states_[current_])
	{
		states_[current_]->leave();
	}
	current_ = e;
	states_[current_]->leave();
}

void StateManager::update()
{
	if (states_[current_])
	{
		states_[current_]->update();
	}
}

StateManager::StateManager()
{
	current_ = eState_None;
}


StateManager* createStateManager()
{
	new StateManager;
	StateManager::getInstancePtr()->create();
	return StateManager::getInstancePtr();
}

StateManager* getStateManager()
{
	return StateManager::getInstancePtr();
}

void destroyStateManager()
{
	if (StateManager::getInstancePtr())
	{
		StateManager::getInstancePtr()->destroy();
		delete StateManager::getInstancePtr();
	}
}

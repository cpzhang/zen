#include "StateManager.h"
#include "TerrainHeightState.h"
#include "TerrainTextureState.h"
#include "PlaceModelState.h"
#include "NavState.h"
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
	case eState_TerrainHeight:
		s = new TerrainHeightState;
		break;
	case eState_TerrainTexture:
		s = new TerrainTextureState;
		break;
	case eState_PlaceModel:
		s = new PlaceModelState;
		break;
	case eState_Nav:
		s = new NavState;
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
	states_[current_]->enter();
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

IState* StateManager::getState( eState e )
{
	return states_[e];
}

eState StateManager::getCurrentStateEnum()
{
	return current_;
}

PlaceModelState* StateManager::getPlaceModelState()
{
	return (PlaceModelState*)states_[eState_PlaceModel];
}

TerrainTextureState* StateManager::getTerrainTextureState()
{
	return (TerrainTextureState*)states_[eState_TerrainTexture];
}

IState* StateManager::getCurrentState()
{
	return states_[current_];
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

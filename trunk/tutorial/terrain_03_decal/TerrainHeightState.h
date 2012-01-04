#pragma once
#include "IState.h"
class TerrainHeightState : public IState
{
public:
	TerrainHeightState();
	~TerrainHeightState();
public:
	virtual void update();
	virtual void destroy(){}
	virtual void enter();
	virtual void leave()
	{

	}
public:

private:
};
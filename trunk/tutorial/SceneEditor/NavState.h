#pragma once
#include "IState.h"
#include "misc/stdHead.h"
#include "render/vector3.h"
class EntityInstance;
class NavState : public IState
{
public:
	NavState();
	~NavState();
public:
	IState_Derived
public:
	virtual void onMouseLeftButtonUp(); ;
	virtual void onMouseRightButtonUp();;
public:
private:
	Vector3 Begin_;
	Vector3 End_;
};
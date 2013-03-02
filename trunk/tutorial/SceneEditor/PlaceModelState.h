#pragma once
#include "IState.h"
#include "misc/stdHead.h"
class EntityInstance;
class PlaceModelState : public IState
{
public:
	PlaceModelState();
	~PlaceModelState();
public:
	IState_Derived
public:
	virtual void setModelFile(const tstring& mf);
	virtual void onMouseLeftButtonUp(); ;
	virtual void onMouseRightButtonUp();;
public:
	//
	tstring getModelFile();
private:
	tstring ModelFile_;
	EntityInstance* ModelShadow_;
};
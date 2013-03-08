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
	virtual void onMouseLeftButtonUp();
	virtual void onMouseRightButtonUp();
	virtual void setPosition(const Vector3& p);
	virtual void setScale(const Vector3& p);
	virtual void setRotation(const Vector3& p);
public:
	//
	tstring getModelFile();
private:
	tstring ModelFile_;
	EntityInstance* ModelShadow_;
	EntityInstance* ModelSelected_;
};
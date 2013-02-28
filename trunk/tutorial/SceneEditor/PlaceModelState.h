#pragma once
#include "IState.h"
#include "misc/stdHead.h"
class PlaceModelState : public IState
{
public:
	PlaceModelState();
	~PlaceModelState();
public:
	IState_Derived
public:
	void setModelFile(const tstring& mf);
private:
	tstring ModelFile_;
};
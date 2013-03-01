#pragma once
#include "IState.h"
#include "misc/stdHead.h"
class TerrainTextureState : public IState
{
public:
	TerrainTextureState();
	~TerrainTextureState();
public:
	IState_Derived
public:
	void setBrushTextureFile(const tstring& n);
	tstring getBrushTextureFile() const
	{
		return BrushTextureFileSelected_;
	}
	
private:
	tstring BrushTextureFileSelected_;
};
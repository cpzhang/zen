#pragma once
#include "IState.h"
#include "misc/stdHead.h"
class Decal;
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
	virtual void setBrushRadiusInner(float f);
	virtual void setBrushRadiusOuter(float f);
	virtual float getBrushRadiusInner() const
	{
		return BrushRadiusInner_;
	}
	virtual float getBrushRadiusOuter() const
	{
		return BrushRadiusOuter_;
	}
	virtual void setBrushStrength(float f);
	virtual float getBrushStrength() const
	{
		return BrushStrength_;
	}
	bool createBrushDecal();
	virtual void setModelFile(const tstring& mf);
private:
	Decal*	BrushDecalInner_;
	Decal*	BrushDecalOuter_;
	tstring BrushTextureFileSelected_;
	float BrushRadiusInner_;
	float BrushRadiusOuter_;
	float BrushStrength_;
};
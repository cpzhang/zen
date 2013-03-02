#pragma once
#include "IState.h"
class Decal;
class TerrainHeightState : public IState
{
public:
	TerrainHeightState();
	~TerrainHeightState();
public:
	IState_Derived
public:
	virtual void setSculptorStrength(float f);
	virtual float getSculptorStrength() const;
	virtual void setSculptorRadius(float f);
	virtual float getSculptorRadius()
	{
		return SculptorRadius_;
	}
	bool createBrushDecal();
private:
	Decal*	SculptorDecal_;
	float SculptorStrength_;
	float SculptorRadius_;
};
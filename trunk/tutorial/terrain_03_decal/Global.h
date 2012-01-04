#pragma once
#include "misc/Singleton.h"
class Decal;
class Global : public Singleton<Global>
{
public:
	Global();
	~Global();
public:
	bool create();
	void destroy();
	Decal* getBrushDecal();
	void update();
	//
	float getBrushStrength();
	void setBrushStrength(float f);
	//
	bool isAbosoluteHeight();
	void setIsAbsoluteHeight(bool b);
	//
	float getAbsoluteHeight();
	void setAbsoluteHeight(float f);
	//
	bool isSmoothAverage();
	void setIsSmoothAverage(bool b);
private:
	void clear_();
	bool createBrushDecal();
	void updatePickingPoint_();
private:
	Decal*	brushDecal_;
	float	brushStrength_;
	bool	isAbsoluteHeight_;
	float	absoluteHeight_;
	bool	isSmoothAverage_;
};

void createGlobal();
Global* getGlobal();
void destroyGlobal();
#pragma once
#include "misc/Singleton.h"
#include "misc/stdHead.h"
class Decal;
class IdleHandler;
class IRender;
// tolua_begin
class Global : public Singleton<Global>
{
// tolua_end
public:
	Global();
	~Global();
public:
	void render();
	bool create();
	void destroy();
	Decal* getBrushDecal();
	void update(float delta);
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
	//
	// tolua_begin
	tstring getDataRootDirectory();
	void setDataRootDirectory(const char* s);
	// tolua_end
	//
	void addHandler(const tstring& name, IdleHandler* h);
	void removeHandler(const tstring& name);
	//
	void setCurrentLayer(const tstring& name);
	tstring getCurrentLayer();
	//
	void refreshDataRoot();
	//
	void setAnimation(const tstring& name);
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
	tstring dataRootDirectory_;
	typedef stdext::hash_map<tstring, IdleHandler*> NameIdleHandlerMap;
	NameIdleHandlerMap nameHandlers_;
	//
	tstring layer_;
	IRender* pi_;
	// tolua_begin
};
	// tolua_end

// tolua_begin
void createGlobal();
Global* getGlobal();
// tolua_end
void destroyGlobal();
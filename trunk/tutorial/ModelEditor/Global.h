#pragma once
#include "misc/Singleton.h"
#include "misc/stdHead.h"
#include "render/OrbitCamera.h"
#include "scene/Node.h"
class Decal;
class IdleHandler;
class IRender;
class EntityInstance;
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
	void update(float delta);
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
//	tstring getDataRootDirectory();
	void setDataRootDirectory(const char* s);
	void setHero(const char* resID);
	// tolua_end
	//
	void addHandler(const tstring& name, IdleHandler* h);
	void removeHandler(const tstring& name);
	//
	void onSelectFile(const tstring& name);
	//tstring getCurrentLayer();
	//
	void refreshDataRoot();
	//
	void setAnimation(const tstring& name);
private:
	void clear_();
	std::string selectedFileParticle( const tstring& name );
	EntityInstance* selectedFileEntity( const tstring& name );
private:
	tstring dataRootDirectory_;
	typedef stdext::hash_map<tstring, IdleHandler*> NameIdleHandlerMap;
	NameIdleHandlerMap nameHandlers_;
	//
	tstring layer_;
	//
	Node Root_;
	// tolua_begin
};
	// tolua_end

// tolua_begin
void createGlobal();
Global* getGlobal();
// tolua_end
void destroyGlobal();
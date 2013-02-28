#pragma once
#include "misc/Singleton.h"
#include "misc/stdHead.h"
//#include "misc/atlHead.h"
#include "render/MouseCamera.h"
#include "render/OrbitCamera.h"
//#include "ViewWindow.h"
class Decal;
class IdleHandler;
class IRender;
class IMovable;
class PreviewWindow;
class EntityInstance;
class Node;

//
class HeroController
{
public:
	HeroController()
	{
		angleY_ = 0.0f;
		position_ = Vector3::Zero;
		speed_ = 1.0f;
	}
public:
	void update(float delta);
	void apply(IMovable* m);
	void apply(OrbitCamera& m, float delta);
	Vector3 getPosition()
	{
		return position_;
	}
	void setCameraAngleY(float a)
	{
		cameraAngleY = a;
	}
private:
	float cameraAngleY;
	float angleY_;
	Vector3 position_;
	float speed_;
};
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
	void setHero(const char* resID);
	// tolua_end
	//
	void addHandler(const tstring& name, IdleHandler* h);
	void removeHandler(const tstring& name);
	//
	void onSelectFile(const tstring& name);
	tstring getCurrentLayer();
	//
	void refreshDataRoot();
	//
	void setAnimation(const tstring& name);
	//
	void onMouseWheel(float d);
	void setPreviewWindowHandle(HWND h);
	void renderPreviewWindow(const float delta);
	//
	PreviewWindow* getPreviewWindow();
	void setPreviewWindow(PreviewWindow* pw);
	void createPreviewer();
	OrbitCamera* getCamera();
	//
	void addEntityInstance(const std::string& resID);
	std::string saveBackBuffer(const std::string resID);
private:
	void clear_();
	bool createBrushDecal();
	void updatePickingPoint_();
	std::string selectedFilePart(const tstring& name);
	std::string selectedFileParticle(const tstring& name);
	EntityInstance* selectedFileEntity(const tstring& name);
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
	HeroController heroController_;
	OrbitCamera	camera_;
	//
	u32 renderTargetKey_;
	HWND previewWindowHandle_;
	PreviewWindow* Previewer_;
	Node* Hero_;
	EntityInstance* HeroInstance_;
	// tolua_begin
};
	// tolua_end

// tolua_begin
void createGlobal();
Global* getGlobal();
// tolua_end
void destroyGlobal();
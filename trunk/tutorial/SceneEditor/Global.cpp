#include "Global.h"
#include "scene/SceneManager.h"
#include "render/Decal.h"
#include "render/rendercontext.h"
#include "render/FxManager.h"
#include "render/TextureManager.h"
#include "render/Ray.h"
#include "scene/QuadNode.h"
#include "scene/Chunk.h"
#include "scene/Terrain.h"
#include "StateManager.h"
#include "luaScript/LuaScript.h"
#include "IdleHandler.h"
#include "model/Part.h"
#include "model/Entity.h"
#include "model/Part.h"
#include "model/Mesh.h"
#include "model/Material.h"
#include "model/Mz.h"
#include "model/Skeleton.h"
#include "misc/FileSystem.h"
#include "tinyXML2/tinyxml2.h"
#include "model/EntityInstance.h"
#include "font/FontManager.h"
#include "PreviewWindow.h"
#include "scene/Node.h"
extern int tolua_LuaAPI_open (lua_State* tolua_S);
Global::Global()
{
	clear_();
}

Global::~Global()
{

}

bool Global::create()
{
	createLuaScript();
	tolua_LuaAPI_open(getLuaScript()->getLuaState());
	tstring d = FileSystem::guessDataDirectory();
	FileSystem::setDataDirectory(d);
	setDataRootDirectory(d.c_str());
	refreshDataRoot();
	//
	createStateManager();
	createFxManager();
	createTextureManager();
	//
	createSceneManager();
	//
	Camera c = getRenderContex()->getCamera();
	c.setFarPlane(10000.0f);
	getRenderContex()->setCamera(c);

	new NodeManager;
	return true;
}

void Global::destroy()
{
	if (Hero_)
	{
		Hero_->release();
		Hero_ = NULL;
	}
	if (HeroInstance_)
	{
		HeroInstance_->release();
		HeroInstance_ = NULL;
	}
	if (Previewer_)
	{
		//delete Previewer_;
		Previewer_->destroy();//stack变量，不能delete
		Previewer_ = NULL;
	}
	getRenderContex()->releaseRenderTarget(renderTargetKey_);
	FontManager::getPointer()->destroy();
	//
	destroySceneManager();
	//
	destroyTextureManager();
	//
	destroyFxManager();
	//
	destroyStateManager();
	//
	ModelResManager::getInstance()->destroy();
//	destroyLuaScript();//导致非法，不解！！先注释掉
	//在此以前，把VB 纹理等显存资源释放干净
	if (getRenderContex())
	{
		destroyRenderContex();
	}
}

void Global::clear_()
{
	isSmoothAverage_ = false;
	absoluteHeight_ = 0;
	isAbsoluteHeight_ = false;
	brushStrength_ = 10;
	//
	renderTargetKey_ = 0;
	previewWindowHandle_ = 0;
	//
	Previewer_ = NULL;
	Hero_ = NULL;
	HeroInstance_ = NULL;
}

void Global::update(float delta)
{
	//
	getStateManager()->update();
	//
	camera_.update(delta, 0);
	getRenderContex()->setViewMatrix(camera_.getViewMatrix());
	//
	if (Hero_)
	{
		Hero_->update(delta);
		//调整主角位置和方向
		Vector3 position_ = camera_.getCenter();
		position_.y = getSceneManager()->getTerrain()->getHeightFromeWorldSpacePosition(position_.x, position_.z);
		HeroInstance_->setPosition(position_);
		HeroInstance_->rotateY(camera_.getAngleY() + MATH_PI);
	}
}
bool Global::isAbosoluteHeight()
{
	return isAbsoluteHeight_;
}

void Global::setIsAbsoluteHeight( bool b )
{
	isAbsoluteHeight_ = b;
}

void Global::setAbsoluteHeight( float f )
{
	absoluteHeight_ = f;
}

float Global::getAbsoluteHeight()
{
	return absoluteHeight_;
}

bool Global::isSmoothAverage()
{
	return isSmoothAverage_;
}

void Global::setIsSmoothAverage( bool b )
{
	isSmoothAverage_ = b;
}

tstring Global::getDataRootDirectory()
{
	return dataRootDirectory_;
}

void Global::setDataRootDirectory( const char* s)
{
	dataRootDirectory_ = s;
	FileSystem::setDataDirectory(dataRootDirectory_);
}

void Global::addHandler( const tstring& name, IdleHandler* h )
{
	nameHandlers_[name] = h;
}

void Global::removeHandler( const tstring& name )
{
	NameIdleHandlerMap::iterator it = nameHandlers_.find(name);
	if (it != nameHandlers_.end())
	{
		nameHandlers_.erase(it);
	}
}
std::string decode(const std::string& name)
{
	std::string fileName(name);
	fileName = FileSystem::standardFilePath(fileName);
	std::string exportPath = FileSystem::getDataDirectory();
	std::string fileFinalName = FileSystem::getParent(fileName);
	fileFinalName = FileSystem::removeParent(fileFinalName);
	exportPath += "/model/";
	exportPath += fileFinalName;
	Mz tM;
	tM.load(fileName);
	Mesh m;
	std::string skeletonPath;
	std::string skinPath;
	//
	{
		//
			skeletonPath = exportPath + "\\" + fileFinalName + ".skeleton";
			FileSystem::createFolder(skeletonPath);
			tM.saveSkeleton(skeletonPath);
			//
			std::string materialPath = exportPath + "\\";
			FileSystem::createFolder(materialPath);
			materialPath = FileSystem::standardFilePath(materialPath);
			tM.saveMaterial(materialPath);
			//
			{
				std::string animationsPath = exportPath + "\\";
				FileSystem::createFolder(animationsPath);
				animationsPath += fileFinalName;
				animationsPath += ".animation";
				animationsPath = FileSystem::standardFilePath(animationsPath);
				tM.saveAnimation(animationsPath);
			}
			//
			{
				std::string animationsPath = exportPath + "\\";
				FileSystem::createFolder(animationsPath);
				tM.saveSubEntity(animationsPath);
			}
			//
			{
				skinPath = exportPath + "\\";
				FileSystem::createFolder(skinPath);
				tM.saveSkin(skinPath);
			}
		}
	for (int i = 0; i < tM.getSubMeshNumber(); ++i)
	{
		tM.create(i, &m);
		std::string path = exportPath + "\\" + tM.getSubMeshName(i) + ".mesh";
		FileSystem::createFolder(path);
		m.save(path);
		std::string bmPath = exportPath + "\\" + tM.getSubMeshName(i) + ".boneMapping";
		FileSystem::createFolder(bmPath);
		m.saveBoneMapping(bmPath, &tM);

		m.destroy();		
	}
	// .entity 
	{
		//============================================================================
		tinyxml2::XMLDocument doc;
		// 
		tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
		doc.LinkEndChild(dec);
		//
		tinyxml2::XMLElement* ele = doc.NewElement("entity");
		ele->SetAttribute("name", fileFinalName.c_str());

		for (int i = 0; i < tM.getSubMeshNumber(); ++i)
		{
			tinyxml2::XMLElement* a = doc.NewElement("part");
			std::string meshPath;
			meshPath += tM.getSubMeshName(i);
			meshPath += ".part";
			a->SetAttribute("file", meshPath.c_str());
			ele->LinkEndChild(a);
		}
		for (int i = 0; i < tM.getParticleSystemNumber(); ++i)
		{
			tstring n = FileSystem::removeParent(fileName);
			n = FileSystem::removeFileExtension(n);
			std::ostringstream ss;
			ss<<"particle/"<<n<<"_"<<i<<".particle";
			tinyxml2::XMLElement* a = doc.NewElement("particle");
			a->SetAttribute("file", ss.str().c_str());
			a->SetAttribute("bone", tM.getParticleSystemBone(i).c_str());
			ele->LinkEndChild(a);
		}
		if(tM.getBoneNumbers() > 0)
		{
			tinyxml2::XMLElement* a = doc.NewElement("skeleton");
			std::string meshPath;
			meshPath += fileFinalName;
			meshPath += ".skeleton";
			a->SetAttribute("file", meshPath.c_str());
			ele->LinkEndChild(a);
		}
		if(tM.getAnimationNumber() > 0)
		{
			tinyxml2::XMLElement* a = doc.NewElement("animation");
			std::string meshPath;
			meshPath += fileFinalName;
			meshPath += ".animation";
			a->SetAttribute("file", meshPath.c_str());
			ele->LinkEndChild(a);
		}
		doc.LinkEndChild(ele);
		//
		std::string path = exportPath + "\\" + fileFinalName + ".entity";
		FileSystem::createFolder(path);
		doc.SaveFile(path.c_str());
		return path;
	}
}
void Global::setHero( const char* resID )
{
	if (HeroInstance_)
	{
		HeroInstance_->release();
	}
	HeroInstance_ = getSceneManager()->createEntityInstance(resID);
	if (NULL == Hero_)
	{
		Hero_ = NodeManager::getInstancePtr()->createNode("Hero");
	}
	Hero_->attach(HeroInstance_);
	HeroInstance_->setAnimation("Run");
}
void Global::onSelectFile( const tstring& name )
{
	std::string suffix = FileSystem::getFileExtension(name);
	std::transform(suffix.begin(), suffix.end(), suffix.begin(), tolower);
	EntityInstance* i = NULL;
	std::string finalEntityName;
	if(suffix == "part")
	{
		finalEntityName = selectedFilePart(name);
	}
	else if(suffix == "particle")
	{
		finalEntityName = selectedFileParticle(name);
	}
	else if(suffix == "entity")
	{
		finalEntityName = name;
	}
	else if(suffix == "mz")
	{
		finalEntityName = decode(name);
	}
	else
	{
		return;
	}
	if (!finalEntityName.empty())
	{
		Previewer_->setModel(finalEntityName);
	}
}

void Global::render()
{
	if (Hero_)
	{
		Hero_->render();
	}
}

void Global::refreshDataRoot()
{
	//从lua脚本，初始化参数
	getLuaScript()->doFile("lua/SceneEditor.lua");
	{
		NameIdleHandlerMap::iterator it = nameHandlers_.begin();
		for (; it != nameHandlers_.end(); ++it)
		{
			IdleHandler* h = it->second;
			if (h)
			{
				h->onRefreshLuaScript();
			}
		}
	}
}

void Global::setAnimation( const tstring& name )
{
// 	if (pi_)
// 	{
// 		EntityInstance* e = (EntityInstance*)pi_;
// 		e->setAnimation(name);
// 	}
}

void Global::onMouseWheel( float d )
{
	camera_.onMouseWheel(d);
}
std::string Global::saveBackBuffer(const std::string resID)
{
	u32 rtk = getRenderContex()->getCurrentRenderTarget();
	getRenderContex()->setCurrentRenderTarget(renderTargetKey_);
	//
	//
	if (Previewer_)
	{
		Previewer_->onIdle(0);
	}
	tstring fn = getRenderContex()->screenShot("bmp", resID, false);
	//
	getRenderContex()->setCurrentRenderTarget(rtk);
	return fn;
}
void Global::renderPreviewWindow(const float delta)
{
	u32 rtk = getRenderContex()->getCurrentRenderTarget();
	getRenderContex()->setCurrentRenderTarget(renderTargetKey_);
	//
	//
	if (Previewer_)
	{
		Previewer_->onIdle(delta);
	}
	//
	getRenderContex()->setCurrentRenderTarget(rtk);
}

void Global::setPreviewWindowHandle( HWND h )
{
	previewWindowHandle_ = h;
	//
	//
	{
		//
		renderTargetKey_ = getRenderContex()->createRenderTarget(eRenderTarget_Additional);
		IRenderTarget* rt = getRenderContex()->getRenderTarget(renderTargetKey_);
		rt->create(previewWindowHandle_);
		//
		u32 ort = getRenderContex()->getCurrentRenderTarget();
		getRenderContex()->setCurrentRenderTarget(renderTargetKey_);
		Camera c = getRenderContex()->getCamera();
		c.setFarPlane(10000.0f);
		c.setHeight(60);
		c.setOrtho(true);
		getRenderContex()->setCamera(c);
		getRenderContex()->setCurrentRenderTarget(ort);
	}
}

PreviewWindow* Global::getPreviewWindow()
{
	return Previewer_;
}

void Global::setPreviewWindow( PreviewWindow* pw )
{
	Previewer_ = pw;
}

void Global::createPreviewer()
{
	setPreviewWindowHandle(Previewer_->m_hWnd);
}

std::string Global::selectedFilePart( const tstring& name)
{
	tstring fileName = FileSystem::removeParent(name);
	tstring partName = FileSystem::removeFileExtension(fileName);
	tstring entityName = FileSystem::getParent(name);
	entityName = FileSystem::removeParent(entityName);
	//============================================================================
	tinyxml2::XMLDocument doc;
	// 
	tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
	doc.LinkEndChild(dec);
	//
	tinyxml2::XMLElement* ele = doc.NewElement("entity");
	ele->SetAttribute("name", partName.c_str());

	{
		tinyxml2::XMLElement* a = doc.NewElement("part");
		std::string meshPath;
		a->SetAttribute("file", fileName.c_str());
		ele->LinkEndChild(a);
	}
	{
		tinyxml2::XMLElement* a = doc.NewElement("skeleton");
		std::string meshPath;
		meshPath += entityName;
		meshPath += ".skeleton";
		a->SetAttribute("file", meshPath.c_str());
		ele->LinkEndChild(a);
	}
	{
		tinyxml2::XMLElement* a = doc.NewElement("animation");
		std::string meshPath;
		meshPath += entityName;
		meshPath += ".animation";
		a->SetAttribute("file", meshPath.c_str());
		ele->LinkEndChild(a);
	}
	doc.LinkEndChild(ele);
	//
	std::string path;
	{
		path = FileSystem::getParent(name) + "\\" + partName + "_t.entity";
	}
	doc.SaveFile(path.c_str());
	
	return path;
}

std::string Global::selectedFileParticle( const tstring& name )
{
	tstring fileName = FileSystem::removeParent(name);
	tstring partName = FileSystem::removeFileExtension(fileName);
	tstring entityName = FileSystem::getParent(name);
	entityName = FileSystem::removeParent(entityName);
	//============================================================================
	tinyxml2::XMLDocument doc;
	// 
	tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
	doc.LinkEndChild(dec);
	//
	tinyxml2::XMLElement* ele = doc.NewElement("entity");
	ele->SetAttribute("name", partName.c_str());
	{
		tinyxml2::XMLElement* a = doc.NewElement("particle");
		std::string meshPath("particle/");
		meshPath += fileName;
		a->SetAttribute("file", meshPath.c_str());
		ele->LinkEndChild(a);
	}
	doc.LinkEndChild(ele);
	//
	std::string path;// = exportPath + "\\" + fileFinalName + ".entity";
	{
		path = FileSystem::getParent(name) + "\\" + partName + "_t.entity";
	}
	//FileSystem::createFolder(path);
	doc.SaveFile(path.c_str());
	return path;
}

EntityInstance* Global::selectedFileEntity( const tstring& name )
{
	EntityInstance* e = getSceneManager()->createEntityInstance(name);
	e->create(name);
	//
	/*Skeleton* s = e->getSkeleton();
	for (int i = 0; i != s->getSkinAnimationNumber(); ++i)
	{
		e->setAnimation(s->getSkinAnimation(i)->name);
		break;
	}*/
	e->setAnimation("Stand");
	return e;
}

// OrbitCamera* Global::getCamera()
// {
// 	return &camera_;
// }

void Global::addEntityInstance( const std::string& resID )
{
	std::string finalEntityName = resID;
	if (!finalEntityName.empty())
	{
		Previewer_->setModel(finalEntityName);
		getSceneManager()->addEntityInstance(finalEntityName);
	}
}

void Global::onMouseMove()
{
	camera_.onMouseMove();
}

void Global::onKeyDown( WPARAM wParam )
{
	switch(wParam)
	{
	case VK_ADD:
		{
			//if (g_bActive)
			{
				camera_.setSpeed(camera_.getSpeed() * 1.2f);
			}
		}break;
	case VK_SUBTRACT:
		{
			//if (g_bActive)
			{
				camera_.setSpeed(camera_.getSpeed() * 0.8f);
			}
		}break;
	}
}

void createGlobal()
{
	new Global;
}

Global* getGlobal()
{
	return Global::getInstancePtr();
}

void destroyGlobal()
{
	if (Global::getInstancePtr())
	{
		Global::getInstancePtr()->destroy();
		delete Global::getInstancePtr();
	}
}
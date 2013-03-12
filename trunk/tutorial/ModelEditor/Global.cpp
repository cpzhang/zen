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
	//createSkeletonManager();
	createSceneManager();
	return true;
}

void Global::destroy()
{
	if (Root_.getEntityInstance())
	{
		Root_.getEntityInstance()->release();
	}
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
	//
	//destroyLuaScript();
	//在此以前，把VB 纹理等显存资源释放干净
	if (getRenderContex())
	{
		destroyRenderContex();
	}
}

void Global::clear_()
{
}

void Global::update(float delta)
{
	//
	getStateManager()->update();
	//
	Root_.update(delta);
}

void Global::setDataRootDirectory( const char* s)
{
	dataRootDirectory_ = s;
	FileSystem::setDataDirectory(dataRootDirectory_ + "/");
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
	EntityInstance* e = Root_.getEntityInstance();
	if (e)
	{
		e->release();
		e = NULL;
	}
	e = getSceneManager()->createEntityInstance(resID);
	Root_.attach(e);	
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


void Global::render()
{
	Root_.render();
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
}

void createGlobal()
{
	new Global;
//	Global::getInstancePtr()->create();
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
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
	refreshDataRoot();
	//
	createStateManager();
	createFxManager();
	createTextureManager();
	//createSkeletonManager();
	createSceneManager();
	if (!createBrushDecal())
	{
		return false;
	}
	camera_.setSpeed(5.0f);
	Vector3 minBound = -Vector3( 100.5f, 0.f, 100.5f );
	Vector3 maxBound = Vector3(10000, 5000.0f, 10000.0f);
	camera_.limit_ =  BoundingBox( minBound, maxBound );
	camera_.create(10, MATH_PI, MATH_PI_Half);
	//
	Camera c = getRenderContex()->getCamera();
	c.setFarPlane(10000.0f);
	getRenderContex()->setCamera(c);
	new NodeManager;
	return true;
}

void Global::destroy()
{
	if (Previewer_)
	{
		delete Previewer_;
		Previewer_ = NULL;
	}
	getRenderContex()->releaseRenderTarget(renderTargetKey_);
// 	if (pi_)
// 	{
// 		//delete pi_;
// 		pi_ = NULL;
// 	}
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
	destroyLuaScript();
	//在此以前，把VB 纹理等显存资源释放干净
	if (getRenderContex())
	{
		destroyRenderContex();
	}
}

void Global::clear_()
{
// 	pi_ = NULL;
// 	movable_ = NULL;
	isSmoothAverage_ = false;
	absoluteHeight_ = 0;
	isAbsoluteHeight_ = false;
	brushDecal_ = 0;
	brushStrength_ = 10;
	//
	renderTargetKey_ = 0;
	previewWindowHandle_ = 0;
	//
	Previewer_ = NULL;
	Hero_ = NULL;
}

bool Global::createBrushDecal()
{
	brushDecal_ = getSceneManager()->createDecal(TEXT("HeightBrush"));
	brushDecal_->setFxFile(TEXT("shader/PositionDecal.fx"));
	brushDecal_->setTexture(TEXT("brush/heighttool.dds"));
	brushDecal_->setRadius(100.0f);
	return true;
}

Decal* Global::getBrushDecal()
{
	return brushDecal_;
}

void Global::update(float delta)
{
	camera_.setCenter(heroController_.getPosition() + Vector3(0, 2, 0));
	camera_.update(delta);
	heroController_.setCameraAngleY(camera_.angleXZ_);
	getRenderContex()->setViewMatrix(camera_.view_);
	//
	updatePickingPoint_();
	//
	getStateManager()->update();
	//
	//
	heroController_.update(delta);
	//
	if (Hero_)
	{
		Hero_->update(delta);
	}
	//
	if (isKeyDown(VK_SPACE) && Previewer_ && !Previewer_->getModelResID().empty())
	{
		getSceneManager()->addEntityInstance(Previewer_->getModelResID());
	}
}

float Global::getBrushStrength()
{
	return brushStrength_;
}

void Global::setBrushStrength( float f )
{
	brushStrength_ = f;
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
struct sPick
{
	bool empty_;
	Vector3 p0_;
	Vector3 p1_;
	Vector3 p2_;
	Vector3 ps_;
};
void calcPick(sPick& pk, Ray r, QuadNode* n)
{
	if (NULL == n)
	{
		return;
	}
	if (!n->boundingBox_.intersectsRay(r))
	{
		return;
	}
	//
	if (n->leaf_ && n->insideChunk_)
	{
		Chunk* c = getSceneManager()->getTerrain()->getChunkFromTopology(n->xNumber_, n->zNumber_);
		Vector3 p0(n->rect_.left_, c->getHeightFromTopology(n->x_, n->z_) ,n->rect_.bottom_);
		Vector3 p1(n->rect_.right_, c->getHeightFromTopology(n->x_ + 1, n->z_) ,n->rect_.bottom_);
		Vector3 p2(n->rect_.right_, c->getHeightFromTopology(n->x_ + 1, n->z_ + 1) ,n->rect_.top_);
		Vector3 p3(n->rect_.left_, c->getHeightFromTopology(n->x_, n->z_ + 1) ,n->rect_.top_);
		Vector3 ps;
		if (r.getIntersetTriangleParameters(p0, p3, p1, ps))
		{
			if (ps.z < pk.ps_.z || pk.empty_)
			{
				pk.empty_ = false;
				pk.p0_ = p0;
				pk.p1_ = p3;
				pk.p2_ = p1;
				pk.ps_ = ps;
			}
		}
		if (r.getIntersetTriangleParameters(p3, p2, p1, ps))
		{
			if (ps.z < pk.ps_.z || pk.empty_)
			{
				pk.empty_ = false;
				pk.p0_ = p3;
				pk.p1_ = p2;
				pk.p2_ = p1;
				pk.ps_ = ps;
			}
		}
		return;
	}
	//
	for (int i = eQuadNode_LeftBottom; i != eQuadNode_Size; ++i)
	{
		QuadNode* c = n->children_[i];
		if (c)
		{
			calcPick(pk, r, c);
		}
	}
}
void Global::updatePickingPoint_()
{
	Ray r = getRenderContex()->getPickingRay();
	float x, z;
	if (0)
	{
		//对于平面地表，可以这样处理
		float t = -r.origion_.y / r.direction_.y;
		x = r.origion_.x + t * r.direction_.x;
		z = r.origion_.z + t * r.direction_.z;
	} 
	else
	{
		sPick pk;
		pk.empty_ = true;
		calcPick(pk, r, getSceneManager()->getTerrainQuadTreeRoot());
		if (pk.empty_)
		{
			return;
		}
		Vector3 p = r.getPoint(pk.ps_.z);
		p = (1 - pk.ps_.x - pk.ps_.y)*pk.p0_ + pk.ps_.x*pk.p1_ + pk.ps_.y*pk.p2_;
		x = p.x;
		z = p.z;
	}
	
	if (getSceneManager()->getTerrain())
	{
		getSceneManager()->getTerrain()->updateVisibleChunks(x, z);
	}
	if (getGlobal() && getGlobal()->getBrushDecal())
	{
		getGlobal()->getBrushDecal()->setCenter(Vector4(x, 0, z, 1));
	}
}

tstring Global::getDataRootDirectory()
{
	return dataRootDirectory_;
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
			skeletonPath = exportPath + "/" + fileFinalName + ".skeleton";
			FileSystem::createFolder(skeletonPath);
			tM.saveSkeleton(skeletonPath);
			//
			std::string materialPath = exportPath + "/";
			FileSystem::createFolder(materialPath);
			materialPath = FileSystem::standardFilePath(materialPath);
			tM.saveMaterial(materialPath);
			//
			{
				std::string animationsPath = exportPath + "/";
				FileSystem::createFolder(animationsPath);
				animationsPath += fileFinalName;
				animationsPath += ".animation";
				animationsPath = FileSystem::standardFilePath(animationsPath);
				tM.saveAnimation(animationsPath);
			}
			//
			{
				std::string animationsPath = exportPath + "/";
				FileSystem::createFolder(animationsPath);
				tM.saveSubEntity(animationsPath);
			}
			//
			{
				skinPath = exportPath + "/";
				FileSystem::createFolder(skinPath);
				tM.saveSkin(skinPath);
			}
		}
	for (int i = 0; i < tM.getSubMeshNumber(); ++i)
	{
		tM.create(i, &m);
		std::string path = exportPath + "/" + tM.getSubMeshName(i) + ".mesh";
		FileSystem::createFolder(path);
		m.save(path);
		std::string bmPath = exportPath + "/" + tM.getSubMeshName(i) + ".boneMapping";
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
		{
			tinyxml2::XMLElement* a = doc.NewElement("skeleton");
			std::string meshPath;
			meshPath += fileFinalName;
			meshPath += ".skeleton";
			a->SetAttribute("file", meshPath.c_str());
			ele->LinkEndChild(a);
		}
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
		std::string path = exportPath + "/" + fileFinalName + ".entity";
		FileSystem::createFolder(path);
		doc.SaveFile(path.c_str());
		return path;
	}
}
void Global::setHero( const char* resID )
{
	if (Hero_)
	{
		EntityInstance* i = Hero_->getEntityInstance();
		if (i)
		{
			i->release();
		}
		Hero_->release();
	}
	Hero_ = NodeManager::getInstancePtr()->createNode("Hero");
	Hero_->attach(getSceneManager()->createEntityInstance(resID));
}
void Global::onSelectFile( const tstring& name )
{
	layer_ = name;
	//
	size_t dotPos = layer_.find('.');
	if (dotPos == std::string::npos)
	{
		return;
	}
	std::string suffix = layer_.substr(dotPos, layer_.size() - dotPos);
	std::transform(suffix.begin(), suffix.end(), suffix.begin(), tolower);
	EntityInstance* i = NULL;
	std::string finalEntityName;
	if(suffix == ".part")
	{
		finalEntityName = selectedFilePart(name);
	}
	else if(suffix == ".particle")
	{
		finalEntityName = selectedFileParticle(name);
	}
	else if(suffix == ".entity")
	{
		finalEntityName = name;
	}
	else if(suffix == ".mz")
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

tstring Global::getCurrentLayer()
{
	return layer_;
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
		path = FileSystem::getParent(name) + "/" + partName + "_t.entity";
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
	std::string path;// = exportPath + "/" + fileFinalName + ".entity";
	{
		path = FileSystem::getParent(name) + "/" + partName + "_t.entity";
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

OrbitCamera* Global::getCamera()
{
	return &camera_;
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

void HeroController::update(float delta)
{
	delta *= 0.001f;
	bool moved = true;
	if ( isKeyDown('W'))
	{
		angleY_ = cameraAngleY + MATH_PI; 
	}
	else if (isKeyDown('S') )
	{
		angleY_ = cameraAngleY;  	
	}
	else if (isKeyDown('A') )
	{
		angleY_ = cameraAngleY + MATH_PI_Half; 
	}
	else if ( isKeyDown('D') )
	{
		angleY_ = cameraAngleY - MATH_PI_Half; 
	}
	else
	{
		moved = false;
	}
	//
	if (moved)
	{
		Vector3 v(sin(angleY_), 0.0f, cos(angleY_));
		float s = speed_ * delta;
		v = v * s;
		position_ += v;
	}
	{
		if (FontManager::getPointer()->getFont())
		{
			std::ostringstream ss;
			ss<<"position = "<<position_.x<<", "<<position_.y<<", "<<position_.z<<std::endl;
			FontManager::getPointer()->getFont()->render(Vector2(10, 30), Vector4(1, 0, 0, 1), ss.str());
		}
	}
}

void HeroController::apply( IMovable* m )
{
	if (NULL == m)
	{
		return;
	}
	m->setPosition(position_);
	m->rotateY(angleY_ + MATH_PI);
}
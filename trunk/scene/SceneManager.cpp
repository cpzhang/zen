#include "SceneManager.h"
#include "Terrain.h"
#include "render/vector3.h"
#include "QuadNode.h"
#include "tinyXML2/tinyxml2.h"
#include "misc/FileSystem.h"
#include "model/EntityInstance.h"
#include "navigation.h"
//#include "font/FontManager.h"
#include "font/FlowText.h"
#include "render/Colour.h"
SceneManager::SceneManager()
{
	clear_();
}

SceneManager::~SceneManager()
{
	clear_();
}

void SceneManager::createTerrain( int xChunks, int zChunks, int n, float unit)
{
	lod_.destroy();
	lod_.setN(n);
	//lod_.setScale(unit);
	lod_.create();
	lod_.setChunkSize(unit);
	destroyTerrain_();
	destroyTerrainQuadTree();
	terrainCurrent_ = new Terrain;
	terrainCurrent_->create(xChunks, zChunks);
//	constructTerrainQuadTree(xChunks, zChunks);
}

void SceneManager::clear_()
{
	Nav_ = NULL;
	terrainQuadTreeRoot_ = NULL;
	allChunksVisible_ = true;
	runType_ = eRunType_Game;
	hero_ = NULL;
	terrainCurrent_ = NULL;
	PickingPoint_ = Vector2::Zero;
	PickingEntity_ = NULL;
}

Terrain* SceneManager::getTerrain()
{
	return terrainCurrent_;
}

void SceneManager::destroy()
{
	if (Nav_)
	{
		NAVIGATION_free(Nav_);
	}
	destroyEntityInstances_();
	//
	lod_.destroy();
	//
	destroyTerrain_();
	//
	destroyTerrainQuadTree();
}

void SceneManager::render()
{
	//1.地表
	if (terrainCurrent_)
	{
		terrainCurrent_->render();
		for (DecalMap::iterator it = decals_.begin(); it != decals_.end(); ++it)
		{
			Decal* d = it->second;
			if (d && d->isVisible())
			{
				terrainCurrent_->renderDecal(d);
			}
		}
	}
	//2.装饰
	for (size_t i = 0; i != entityInstances_.size(); ++i)
	{
		entityInstances_[i]->render();
	}
	//3.nav
	renderNav();
	//4.aabb
	for (size_t i = 0; i != entityInstances_.size(); ++i)
	{
		if (entityInstances_[i] == PickingEntity_)
		{
			entityInstances_[i]->renderAABB(Colour::Red);
		}
		else
		{
			entityInstances_[i]->renderAABB(Colour::Green);
		}
	}
}

LOD* SceneManager::getLOD()
{
	return &lod_;
}

void SceneManager::destroyTerrain_()
{
	if (terrainCurrent_)
	{
		terrainCurrent_->destroy();
		delete terrainCurrent_;
		terrainCurrent_ = NULL;
	}
}

Hero* SceneManager::getHero()
{
	return hero_;
}

void SceneManager::setHero( Hero* h )
{
	hero_ = h;
}

void SceneManager::update(const float delta)
{
	if (hero_ && terrainCurrent_ && !allChunksVisible_)
	{
	//	Vector3 p = hero_->getPosition();
	//	terrainCurrent_->updateVisibleChunks(p.x, p.z);
	}
	//1.地表
	if (terrainCurrent_)
	{
		updatePickingPoint_();
		terrainCurrent_->clearSelected();
	}
	//2.装饰
	for (size_t i = 0; i != entityInstances_.size(); ++i)
	{
		entityInstances_[i]->update(delta);
	}
}

void SceneManager::setRunType( eRunType rt )
{
	runType_ = rt;
}

eRunType SceneManager::getRunType()
{
	return runType_;
}

bool SceneManager::IsAllChunksVisible()
{
	return allChunksVisible_;
}

void SceneManager::setAllChunksVisible( bool b )
{
	allChunksVisible_ = b;
}

Decal* SceneManager::createDecal(const tstring& name)
{
	Decal* d = new Decal;
	d->setName(name);
	decals_[name] = d;
	return d;
}

void SceneManager::destroyDecals()
{
	for (DecalMap::iterator it = decals_.begin(); it != decals_.end(); ++it)
	{
		Decal* d = it->second;
		if (d)
		{
			d->destroy();
			delete d;
		}
	}
	decals_.clear();
}
typedef void createLeafImp_(QuadNode* parent, eQuadNode e, int x, int z);
void constructTerrainQuadTreeImp_(QuadNode* parent, int xChunksBegin, int xChunksEnd, int zChunksBegin, int zChunksEnd, createLeafImp_* cf, bool outSide = true);
//
RectangleT gRC;
int gX;
int gZ;
void constructQuadTreeInsideChunk(QuadNode* parent, eQuadNode e, int x, int z)
{
	QuadNode* n = QuadNode::create();
	parent->children_[e] = n;
	n->leaf_ = true;
	n->insideChunk_ = true;
	n->xNumber_ = gX;
	n->zNumber_ = gZ;
	n->x_ = x;
	n->z_ = z;
	float s = getSceneManager()->getLOD()->getScale();
	n->rect_.left_ = x * s + gRC.left_;
	n->rect_.right_ = n->rect_.left_ + s;
	n->rect_.bottom_ = z * s + gRC.bottom_;
	n->rect_.top_ = n->rect_.bottom_ + s;
	//
	Chunk* ck = getSceneManager()->getTerrain()->getChunkFromTopology(gX, gZ);
	Vector2 p = ck->getMaxMinHeightFromTopology(x, x+1, z, z+1);
	n->boundingBox_.max_ = Vector3(n->rect_.right_, p.x, n->rect_.top_);
	n->boundingBox_.min_ = Vector3(n->rect_.left_, p.y, n->rect_.bottom_);
}
//
void createLeaf(QuadNode* parent, eQuadNode e, int x, int z)
{
	parent->children_[e] = QuadNode::create();
	parent->children_[e]->leaf_ = true;
	parent->children_[e]->xNumber_ = x;
	parent->children_[e]->zNumber_ = z;
	gX = x;
	gZ = z;
	parent->children_[e]->rect_ = getSceneManager()->getTerrain()->getChunkFromTopology(x, z)->getRect();
	gRC = parent->children_[e]->rect_;
	constructTerrainQuadTreeImp_(parent->children_[e], 0, getSceneManager()->getLOD()->getVerticesNumberOneSide() - 2, 0, getSceneManager()->getLOD()->getVerticesNumberOneSide() - 2, &constructQuadTreeInsideChunk, false);
}
//闭区间
void constructTerrainQuadTreeImp_(QuadNode* parent, int xChunksBegin, int xChunksEnd, int zChunksBegin, int zChunksEnd, createLeafImp_* cf, bool outSide /*= true*/)
{
	if (xChunksEnd < xChunksBegin || zChunksEnd < zChunksBegin)
	{
		return;
	}
	eQuadNode e = eQuadNode_Size;
	QuadNode* n = NULL;
	if (xChunksEnd - xChunksBegin == 0)
	{
		if (zChunksEnd - zChunksBegin == 0)
		{
			cf(parent, eQuadNode_LeftBottom, xChunksBegin, zChunksBegin);
			return;
		}
		if (zChunksEnd - zChunksBegin == 1)
		{
			//
			cf(parent, eQuadNode_LeftBottom, xChunksBegin, zChunksBegin);
			//
			cf(parent, eQuadNode_LeftTop, xChunksBegin, zChunksEnd);
			return;
		}
	}
	if (xChunksEnd - xChunksBegin == 1)
	{
		if (zChunksEnd - zChunksBegin == 0)
		{
			cf(parent, eQuadNode_LeftBottom, xChunksBegin, zChunksBegin);
			cf(parent, eQuadNode_RightBottom, xChunksEnd, zChunksBegin);
			return;
		}
		if (zChunksEnd - zChunksBegin == 1)
		{
			//
			cf(parent, eQuadNode_LeftBottom, xChunksBegin, zChunksBegin);
			//
			cf(parent, eQuadNode_LeftTop, xChunksBegin, zChunksEnd);
			//
			cf(parent, eQuadNode_RightBottom, xChunksEnd, zChunksBegin);
			//
			cf(parent, eQuadNode_RightTop, xChunksEnd, zChunksEnd);
			return;
		}
	}
	//
	{
		float s = 0;
		if (outSide)
		{
			s = getSceneManager()->getLOD()->getLengthOneSide();
		}
		else
		{
			s = getSceneManager()->getLOD()->getScale();
		}
		//
		{
			int x0 = xChunksBegin;
			int x1 = xChunksBegin + (xChunksEnd - xChunksBegin)/2;
			int z0 = zChunksBegin;
			int z1 = zChunksBegin + (zChunksEnd - zChunksBegin)/2;
			if (x0 <= x1 && z0 <= z1)
			{
				e = eQuadNode_LeftBottom;
				parent->children_[e] = QuadNode::create();
				parent->children_[e]->leaf_ = false;
				n = parent->children_[e];
				n->rect_.left_ = x0*s;
				n->rect_.right_ = (x1 + 1)*s;
				n->rect_.bottom_ = z0*s;
				n->rect_.top_ = (z1 + 1)*s;
				if (!outSide)
				{
					n->rect_.left_ += gRC.left_;
					n->rect_.right_ += gRC.left_;
					n->rect_.bottom_ += gRC.bottom_;
					n->rect_.top_ += gRC.bottom_;
				}
				if (!outSide)
				{
					Chunk* ck = getSceneManager()->getTerrain()->getChunkFromTopology(gX, gZ);
					Vector2 p = ck->getMaxMinHeightFromTopology(x0, x1, z0, z1);
					n->boundingBox_.max_ = Vector3(n->rect_.right_, p.x, n->rect_.top_);
					n->boundingBox_.min_ = Vector3(n->rect_.left_, p.y, n->rect_.bottom_);
				}
				constructTerrainQuadTreeImp_(n, x0, x1, z0, z1, cf, outSide);
			}
		}
		{
			int x0 = xChunksBegin + (xChunksEnd - xChunksBegin)/2 + 1;
			int x1 = xChunksEnd;
			int z0 = zChunksBegin;
			int z1 = zChunksBegin + (zChunksEnd - zChunksBegin)/2;
			if (x0 <= x1 && z0 <= z1)
			{
				e = eQuadNode_RightBottom;
				parent->children_[e] = QuadNode::create();
				parent->children_[e]->leaf_ = false;
				n = parent->children_[e];
				n->rect_.left_ = x0*s;
				n->rect_.right_ = (x1 + 1)*s;
				n->rect_.bottom_ = z0*s;
				n->rect_.top_ = (z1 + 1)*s;
				if (!outSide)
				{
					n->rect_.left_ += gRC.left_;
					n->rect_.right_ += gRC.left_;
					n->rect_.bottom_ += gRC.bottom_;
					n->rect_.top_ += gRC.bottom_;
				}
				if (!outSide)
				{
					Chunk* ck = getSceneManager()->getTerrain()->getChunkFromTopology(gX, gZ);
					Vector2 p = ck->getMaxMinHeightFromTopology(x0, x1, z0, z1);
					n->boundingBox_.max_ = Vector3(n->rect_.right_, p.x, n->rect_.top_);
					n->boundingBox_.min_ = Vector3(n->rect_.left_, p.y, n->rect_.bottom_);
				}
				constructTerrainQuadTreeImp_(n, x0, x1, z0, z1, cf, outSide);
			}
		}
		{
			int x0 = xChunksBegin;
			int x1 = xChunksBegin + (xChunksEnd - xChunksBegin)/2;
			int z0 = zChunksBegin + (zChunksEnd - zChunksBegin)/2 + 1;
			int z1 = zChunksEnd;
			if (x0 <= x1 && z0 <= z1)
			{
				e = eQuadNode_LeftTop;
				parent->children_[e] = QuadNode::create();
				parent->children_[e]->leaf_ = false;
				n = parent->children_[e];
				n->rect_.left_ = x0*s;
				n->rect_.right_ = (x1 + 1)*s;
				n->rect_.bottom_ = z0*s;
				n->rect_.top_ = (z1 + 1)*s;
				if (!outSide)
				{
					n->rect_.left_ += gRC.left_;
					n->rect_.right_ += gRC.left_;
					n->rect_.bottom_ += gRC.bottom_;
					n->rect_.top_ += gRC.bottom_;
				}
				if (!outSide)
				{
					Chunk* ck = getSceneManager()->getTerrain()->getChunkFromTopology(gX, gZ);
					Vector2 p = ck->getMaxMinHeightFromTopology(x0, x1, z0, z1);
					n->boundingBox_.max_ = Vector3(n->rect_.right_, p.x, n->rect_.top_);
					n->boundingBox_.min_ = Vector3(n->rect_.left_, p.y, n->rect_.bottom_);
				}
				constructTerrainQuadTreeImp_(n, x0, x1, z0, z1, cf, outSide);
			}
		}
		{
			int x0 = xChunksBegin + (xChunksEnd - xChunksBegin)/2 + 1;
			int x1 = xChunksEnd;
			int z0 = zChunksBegin + (zChunksEnd - zChunksBegin)/2 + 1;
			int z1 = zChunksEnd;
			if (x0 <= x1 && z0 <= z1)
			{
				e = eQuadNode_RightTop;
				parent->children_[e] = QuadNode::create();
				parent->children_[e]->leaf_ = false;
				n = parent->children_[e];
				n->rect_.left_ = x0*s;
				n->rect_.right_ = (x1 + 1)*s;
				n->rect_.bottom_ = z0*s;
				n->rect_.top_ = (z1 + 1)*s;
				if (!outSide)
				{
					n->rect_.left_ += gRC.left_;
					n->rect_.right_ += gRC.left_;
					n->rect_.bottom_ += gRC.bottom_;
					n->rect_.top_ += gRC.bottom_;
				}
				if (!outSide)
				{
					Chunk* ck = getSceneManager()->getTerrain()->getChunkFromTopology(gX, gZ);
					Vector2 p = ck->getMaxMinHeightFromTopology(x0, x1, z0, z1);
					n->boundingBox_.max_ = Vector3(n->rect_.right_, p.x, n->rect_.top_);
					n->boundingBox_.min_ = Vector3(n->rect_.left_, p.y, n->rect_.bottom_);
				}
				constructTerrainQuadTreeImp_(n, x0, x1, z0, z1, cf, outSide);
			}
		}
	}
}
void SceneManager::constructTerrainQuadTree(int xChunks, int zChunks )
{
	terrainQuadTreeRoot_ = QuadNode::create(true);
	terrainQuadTreeRoot_->leaf_ = false;
	terrainQuadTreeRoot_->rect_.left_ = 0;
	terrainQuadTreeRoot_->rect_.right_ = lod_.getLengthOneSide() * xChunks;
	terrainQuadTreeRoot_->rect_.bottom_ = 0;
	terrainQuadTreeRoot_->rect_.top_ = lod_.getLengthOneSide() * zChunks;
	constructTerrainQuadTreeImp_(terrainQuadTreeRoot_, 0, xChunks-1, 0, zChunks-1, createLeaf);
	//
	QuadNode::calculateBoundingBox(terrainQuadTreeRoot_);
}

void SceneManager::destroyTerrainQuadTree()
{
	if (terrainQuadTreeRoot_)
	{
		terrainQuadTreeRoot_->release();
	}
}

QuadNode* SceneManager::getTerrainQuadTreeRoot()
{
	return terrainQuadTreeRoot_;
}

void SceneManager::getChunks( ChunkVec& cs, QuadNode* n, RectangleT& rc )
{
	if (NULL == n)
	{
		return;
	}
	if (!rc.isIntersected(&n->rect_))
	{
		return;
	}
	if (n->leaf_)
	{
		cs.push_back(terrainCurrent_->getChunkFromTopology(n->xNumber_, n->zNumber_));
		return;
	}
	for (int i = eQuadNode_LeftBottom; i != eQuadNode_Size; ++i)
	{
		QuadNode* c = n->children_[i];
		if (c)
		{
			getChunks(cs, c, rc);
		}
	}
}

void SceneManager::save(const tstring& path)
{
	if (terrainCurrent_ == NULL)
	{
		return;
	}
	std::string resId(path);
	name_ = FileSystem::removeParent(path);
	//============================================================================
	tinyxml2::XMLDocument doc;
	// 
	tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
	doc.LinkEndChild(dec);
	//
	tinyxml2::XMLElement* ele = doc.NewElement("scene");
	//
	ele->SetAttribute("name", name_.c_str());
	doc.LinkEndChild(ele);
	//
	{
		tinyxml2::XMLElement* e = doc.NewElement("terrain");
		e->SetAttribute("XChunkNumber", terrainCurrent_->getXChunkNumber());
		e->SetAttribute("ZChunkNumber", terrainCurrent_->getZChunkNumber());
		e->SetAttribute("Lod", lod_.getN());
		e->SetAttribute("ChunkSize", lod_.getChunkSize());
		ele->LinkEndChild(e);
	}
	//
	{
		tinyxml2::XMLElement* e = doc.NewElement("material");
		std::string tn(terrainCurrent_->getFXFileName());
		//tn = FileSystem::cutDataPath(tn);
		e->SetAttribute("fx", tn.c_str());
		ele->LinkEndChild(e);
	}
	//
	tstring sf = resId + "\\setting.xml";
	doc.SaveFile(sf.c_str());
	//
	{
		//分配物件
		for (size_t i = 0; i != entityInstances_.size(); ++i)
		{
			Vector3 p = entityInstances_[i]->getPosition();
			Chunk* c = terrainCurrent_->getChunkFromWorldSpacePostion(p.x, p.z);
			if (c)
			{
				c->addEntityInstance(entityInstances_[i]);
			}
		}

		terrainCurrent_->save(resId + "\\");
	}
}

void SceneManager::open( const tstring& dir )
{
	std::string resId(dir);
	if (!FileSystem::isDirExist(resId))
	{
		resId = FileSystem::getDataDirectory() + dir;
		if (!FileSystem::isDirExist(resId))
		{
			return;
		}
	}

	tinyxml2::XMLDocument doc;
	tstring settingFile(resId + "/setting.xml");
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(settingFile.c_str()))
	{
		return;
	}
	tinyxml2::XMLElement* ele = doc.RootElement();
	if (NULL == ele)
	{
		return;
	}
	name_ = ele->Attribute("name");
	//
	int xChunks;
	int zChunks;
	int n;
	float unit;
	{
		tinyxml2::XMLElement* tex= ele->FirstChildElement("terrain");
		if (tex)
		{
			if (tinyxml2::XML_SUCCESS != tex->QueryIntAttribute("XChunkNumber", &xChunks))
			{
				return;
			}
			if (tinyxml2::XML_SUCCESS != tex->QueryIntAttribute("ZChunkNumber", &zChunks))
			{
				return;
			}
			if (tinyxml2::XML_SUCCESS != tex->QueryIntAttribute("Lod", &n))
			{
				return;
			}
			if (tinyxml2::XML_SUCCESS != tex->QueryFloatAttribute("ChunkSize", &unit))
			{
				return;
			}
		}
	}
	//
	createTerrain(xChunks, zChunks, n, unit);
	//
	{
		terrainCurrent_->open(resId);
		constructTerrainQuadTree(xChunks, zChunks);
	}
	//
	{
		tinyxml2::XMLElement* tex= ele->FirstChildElement("material");
		if (tex)
		{
			const char* n = tex->Attribute("fx");
			if (NULL != n)
			{
				terrainCurrent_->setFX(n);
			}
		}
	}
}

EntityInstance* SceneManager::createEntityInstance( const std::string& resID )
{
	EntityInstance* i = new EntityInstance;
	if (!i->create(resID))
	{
		i->release();
		return NULL;
	}
	return i;
}

void SceneManager::removeEntityInstance( EntityInstance* ei )
{
	for (EntityInstanceVec::iterator i = entityInstances_.begin(); i != entityInstances_.end(); ++i)
	{
		if (ei == *i)
		{
			ei->release();
			entityInstances_.erase(i);
			return;
		}
	}
}

EntityInstance* SceneManager::addEntityInstance( const std::string& resID )
{
	EntityInstance* i = createEntityInstance(resID);
	entityInstances_.push_back(i);
	return i;
}

void SceneManager::destroyEntityInstances_()
{
	for (size_t i = 0; i != entityInstances_.size(); ++i)
	{
		entityInstances_[i]->release();
	}
	entityInstances_.clear();
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
Vector2 SceneManager::getPickingPoint()
{
	return PickingPoint_;
}

EntityInstance* SceneManager::getPickingEntityInstance()
{
	PickingEntity_ = NULL;
	Ray r = getRenderContex()->getPickingRay();	
	for (size_t i = 0; i != entityInstances_.size(); ++i)
	{
		if (entityInstances_[i]->isPicking(r))
		{
			PickingEntity_ = entityInstances_[i];
		}
	}
	return PickingEntity_;
}
void SceneManager::updatePickingPoint_()
{
	Ray r = getRenderContex()->getPickingRay();
	if (0)
	{
		float x = 0.0f;
		float z = 0.0f;;
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
		if (!pk.empty_)
		{
			Vector3 p = r.getPoint(pk.ps_.z);
			p = (1 - pk.ps_.x - pk.ps_.y)*pk.p0_ + pk.ps_.x*pk.p1_ + pk.ps_.y*pk.p2_;
			PickingPoint_.x = p.x;
			PickingPoint_.y = p.z;
		}
	}
}

void SceneManager::nmCreateObjFile(const std::string& fn, std::vector<Vector3>& vertices, std::vector<Vector3Int>& indices)
{
	if (NULL == terrainCurrent_)
	{
		return;
	}
	//1.地表
	for (int x = 0; x != terrainCurrent_->getXChunkNumber(); ++x)
	{
		for (int z = 0; z != terrainCurrent_->getZChunkNumber(); ++z)
		{
			Chunk* c = terrainCurrent_->getChunkFromTopology(x, z);
			if (c)
			{
				c->nmAddObj(vertices, indices);
			}
		}
	}
	//2.装饰
	for (size_t i = 0; i != entityInstances_.size(); ++i)
	{
		entityInstances_[i]->nmAddObj(vertices, indices);
		
	}
	return;
	//
	std::ofstream o(fn.c_str());
	for (size_t i = 0; i != vertices.size(); ++i)
	{
		Vector3& p = vertices[i];
		o<<"v "<<p.x<<" "<<p.y<<" "<<p.z<<std::endl;
	}
	for (size_t i = 0; i != indices.size(); ++i)
	{
		Vector3Int& p = indices[i];
		o<<"f "<<p.x + 1<<" "<<p.y + 1<<" "<<p.z + 1<<std::endl;
	}
	o.close();	
}

void SceneManager::getPath( const Vector3& b, const Vector3& e, std::vector<Vector3>& ps )
{
	if (Nav_)
	{
		NAVIGATIONPATH p;
		p.start_location = b;
		p.end_location = e;
		NAVIGATIONPATHDATA d;
		if (NAVIGATION_get_path( Nav_, &p, &d) >= 0)
		{
			for (int i = 0; i != d.path_point_count; ++i)
			{
				ps.push_back(d.path_point_array[i]);
			}
		}
		else
		{
			FlowText::getSingletonP()->add("无效区域，无法到达", Vector4::One);
		}
	}
}

void SceneManager::renderNav() const
{
	if (NULL == Nav_ || NULL == Nav_->dtnavmesh)
	{
		return;
	}
	//光照默认打开
	getRenderContex()->getDxDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	getRenderContex()->SetTransform(D3DTS_WORLD, &Matrix::Identity);
	getRenderContex()->applyViewMatrix();
	getRenderContex()->applyProjectionMatrix();
	getRenderContex()->setVertexDeclaration(sVDT_PositionColor::getType());
	getRenderContex()->getDxDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	unsigned int j = 0;
	while( j != Nav_->dtnavmesh->getMaxTiles() )
	{
		const dtMeshTile *_dtMeshTile = Nav_->dtnavmesh->getTile( j );

		if( !_dtMeshTile->header )
		{
			continue; 
		}

		unsigned int k = 0;

		while( k != _dtMeshTile->header->polyCount )
		{
			dtPoly *_dtPoly = &_dtMeshTile->polys[ k ];

			if( _dtPoly->getType() == DT_POLYTYPE_OFFMESH_CONNECTION )
			{ 
				continue;
			}
			else
			{
				dtPolyDetail* pd = &_dtMeshTile->detailMeshes[ k ];

				unsigned int l = 0;

				while( l != pd->triCount )
				{
					Vector3 v[ 3 ];

					const unsigned char *t = &_dtMeshTile->detailTris[ ( pd->triBase + l ) << 2 ];

					int m = 2;
					while( m != -1 )
					{
						if( t[ m ] < _dtPoly->vertCount )
						{
							memcpy( &v[ m ],
								&_dtMeshTile->verts[ _dtPoly->verts[ t[ m ] ] * 3 ],
								sizeof( Vector3 ) );
						}
						else
						{
							memcpy( &v[ m ],
								&_dtMeshTile->detailVerts[ ( pd->vertBase + t[ m ] - _dtPoly->vertCount ) * 3 ],
								sizeof( Vector3 ) );
						}

						--m;
					}
					//draw
					sVDT_PositionColor ps[3];
					ps[0].position_ = v[0];
					ps[1].position_ = v[1];
					ps[2].position_ = v[2];
					ps[0].color_ = ps[1].color_ = ps[2].color_ = Colour::getUint32(10*j, 10*k, 10*l, 10*(j + k + l));
					getRenderContex()->drawPrimitiveUP(D3DPT_TRIANGLELIST, 1, &ps[0], sVDT_PositionColor::getSize());
					++l;
				}
			}

			++k;
		}

		++j;
	}
	getRenderContex()->getDxDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void SceneManager::setShowNav( bool b )
{
	ShowNav_ = b;
}

NAVIGATIONCONFIGURATION* SceneManager::getNavConfig()
{
	NAVIGATIONCONFIGURATION* c = NULL;
	if (Nav_ == NULL)
	{
		Nav_ = NAVIGATION_init(FileSystem::removeParent(name_).c_str());
	}
	if (NULL != Nav_)
	{
		c = &Nav_->navigationconfiguration;
	}
	return c;
}

bool SceneManager::buildNav()
{
	//
	if (Nav_ == NULL)
	{
		Nav_ = NAVIGATION_init(FileSystem::removeParent(name_).c_str());
	}
	tstring nbfn(name_ + "\\nav.bin");
	if (0 == NAVIGATION_build(Nav_, nbfn.c_str()))
	{
		return false;
	}
	return true;
}

ApiScene_ SceneManager* createSceneManager()
{
	new SceneManager;
	return SceneManager::getInstancePtr();
}

ApiScene_ void destroySceneManager()
{
	if (SceneManager::getInstancePtr())
	{
		SceneManager::getInstancePtr()->destroy();
		delete SceneManager::getInstancePtr();
	}
}

ApiScene_ SceneManager* getSceneManager()
{
	return SceneManager::getInstancePtr();
}
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
Global::Global()
{
	clear_();
}

Global::~Global()
{

}

bool Global::create()
{
	createStateManager();
	createFxManager();
	createTextureManager();
	createSceneManager();
	if (!createBrushDecal())
	{
		return false;
	}
	return true;
}

void Global::destroy()
{
	//
	destroySceneManager();
	//
	destroyTextureManager();
	//
	destroyFxManager();
	//
	destroyStateManager();
}

void Global::clear_()
{
	isSmoothAverage_ = false;
	absoluteHeight_ = 0;
	isAbsoluteHeight_ = false;
	brushDecal_ = 0;
	brushStrength_ = 10;
}

bool Global::createBrushDecal()
{
	brushDecal_ = getSceneManager()->createDecal(TEXT("HeightBrush"));
	brushDecal_->setFxFile(TEXT("e:/ZenBin/data/shader/PositionDecal.fx"));
	brushDecal_->setTexture(TEXT("e:/ZenBin/data/image/heighttool.dds"));
	brushDecal_->setRadius(100.0f);
	return true;
}

Decal* Global::getBrushDecal()
{
	return brushDecal_;
}

void Global::update()
{
	//
	updatePickingPoint_();
	//
	getStateManager()->update();
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
void createGlobal()
{
	new Global;
	Global::getInstancePtr()->create();
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

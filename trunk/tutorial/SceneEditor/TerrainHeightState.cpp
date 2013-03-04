#include "TerrainHeightState.h"
#include "misc/helper.h"
#include "misc/windowsHead.h"
#include "scene/Terrain.h"
#include "scene/SceneManager.h"
#include "scene/Chunk.h"
#include "render/math.h"
#include "scene/QuadNode.h"
#include "Global.h"
#include "font/FlowText.h"
void TerrainHeightState::update()
{
	if (getSceneManager() && getSceneManager()->getTerrain() && SculptorDecal_)
	{
		Vector2 pp = getSceneManager()->getPickingPoint();
		SculptorDecal_->setCenter(Vector4(pp.x, 0, pp.y, 1));
	}

	if (isKeyDown(VK_LBUTTON))
	{
		if (getSceneManager() && getSceneManager()->getTerrain())
		{
			Terrain* t = getSceneManager()->getTerrain();
			Vector4 center = SculptorDecal_->getCenter();
			LOD* d = getSceneManager()->getLOD();
			int n = d->getVerticesNumberOneSide();
			float radius = SculptorDecal_->getRadius();
			RectangleT rc;
			rc.left_ = center.x - radius;
			rc.right_ = center.x + radius;
			rc.bottom_ = center.z - radius;
			rc.top_ = center.z + radius;
			ChunkVec cs;
			getSceneManager()->getChunks(cs, getSceneManager()->getTerrainQuadTreeRoot(), rc);
			float radius2 = radius * radius;
			bool dirty = false;
			for (size_t i = 0; i != cs.size(); ++i)
			{
				Chunk* c0 = cs[i]; 
				c0->setSelected(true);
				for (int x = 0; x != n; ++x)
				for (int z = 0; z != n; ++z)
				{
					Vector2 p = c0->getWorldCoordination(x, z);
					float distance2 = (p.x - center.x)*(p.x - center.x) + (p.y - center.z)*(p.y - center.z);
					if (distance2 <= radius2)
					{
						dirty = true;
						float h = 0;
						if (getGlobal()->isSmoothAverage())
						{
							float left = c0->getHeightFromTopology(x - 1, z);
							float right = c0->getHeightFromTopology(x + 1, z);
							float bottom = c0->getHeightFromTopology(x, z - 1);
							float top = c0->getHeightFromTopology(x, z + 1);
							h = (c0->getHeightFromTopology(x, z) + left + right + bottom + top) / 5; 
							//
						}
						else
						{
							if (getGlobal()->isAbosoluteHeight())
							{
								h = getGlobal()->getAbsoluteHeight();
							}
							else
							{
								float r = cos(MATH_PI_Half * distance2/radius2);
								r = 1 - distance2/radius2;
								h = c0->getHeightFromTopology(x, z) + r * getSculptorStrength();
							}
							//
							if(0){
								float left = c0->getHeightFromTopology(x - 1, z);
								float right = c0->getHeightFromTopology(x + 1, z);
								float bottom = c0->getHeightFromTopology(x, z - 1);
								float top = c0->getHeightFromTopology(x, z + 1);
								h = (h + left + right + bottom + top) / 5; 
							}
						}
						//
						if (h > 255)
						{
							h = 255;
						}
						//
						c0->setHeightFromTopology(x, z, h);
					}
				}
				//
				c0->refreshHeight();
			}
			if (dirty)
			{
				QuadNode::calculateBoundingBox(getSceneManager()->getTerrainQuadTreeRoot());
			}
		}
	}
}

TerrainHeightState::~TerrainHeightState()
{

}

TerrainHeightState::TerrainHeightState()
{
	type_ = eState_TerrainHeight;
	SculptorDecal_ = NULL;
	SculptorRadius_ = 1.0f;
	SculptorStrength_ = 1.0f;
}

void TerrainHeightState::enter()
{
	if (SculptorDecal_ == NULL)
	{
		createBrushDecal();
	}
	SculptorDecal_->setVisible(true);
}

void TerrainHeightState::leave()
{
	SculptorDecal_->setVisible(false);
}

void TerrainHeightState::destroy()
{
}

bool TerrainHeightState::createBrushDecal()
{
	SculptorDecal_ = getSceneManager()->createDecal(TEXT("HeightBrush"));
	SculptorDecal_->setFxFile(TEXT("\\shader\\PositionDecal.fx"));
	SculptorDecal_->setTexture(TEXT("\\brush\\heighttool.dds"));
	SculptorDecal_->setRadius(3.0f);
	return true;
}

void TerrainHeightState::setSculptorStrength( float f )
{
	std::ostringstream ss;
	ss<<"高度刷强度："<<f;
	FlowText::getSingletonP()->add(ss.str(), Vector4(1, 1, 1, 1));
	SculptorStrength_ = f;
}

float TerrainHeightState::getSculptorStrength() const
{
	return SculptorStrength_;
}

void TerrainHeightState::setSculptorRadius( float f )
{
	std::ostringstream ss;
	ss<<"高度刷半径："<<f;
	FlowText::getSingletonP()->add(ss.str(), Vector4(1, 1, 1, 1));
	SculptorRadius_ = f;
	if (SculptorDecal_)
	{
		SculptorDecal_->setRadius(getSculptorRadius());
	}
}

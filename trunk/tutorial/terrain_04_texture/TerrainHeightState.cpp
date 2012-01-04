#include "TerrainHeightState.h"
#include "misc/helper.h"
#include "misc/windowsHead.h"
#include "scene/Terrain.h"
#include "scene/SceneManager.h"
#include "Global.h"
#include "scene/Chunk.h"
#include "render/math.h"
#include "scene/QuadNode.h"
void TerrainHeightState::update()
{
	if (isKeyDown(VK_LBUTTON))
	{
		if (getSceneManager() && getSceneManager()->getTerrain())
		{
			Terrain* t = getSceneManager()->getTerrain();
			Vector4 center = getGlobal()->getBrushDecal()->getCenter();
			LOD* d = getSceneManager()->getLOD();
			int n = d->getVerticesNumberOneSide();
			float radius = getGlobal()->getBrushDecal()->getRadius() * 0.5f;
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
								h = c0->getHeightFromTopology(x, z) + cos(MATH_PI_Half * distance2/radius2) * getGlobal()->getBrushStrength() * 0.1f;
							}
							//
							{
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
}

void TerrainHeightState::enter()
{
	float h = getGlobal()->getBrushDecal()->getRadius();
	getGlobal()->getBrushDecal()->setRadius(h);
}

void TerrainHeightState::leave()
{
	//getGlobal()->getBrushDecal()->setRadius(50);
}

void TerrainHeightState::destroy()
{
}
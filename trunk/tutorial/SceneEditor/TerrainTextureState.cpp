#include "TerrainTextureState.h"
#include "misc/helper.h"
#include "misc/windowsHead.h"
#include "scene/Terrain.h"
#include "scene/SceneManager.h"
#include "Global.h"
#include "scene/Chunk.h"
#include "render/math.h"
#include "scene/QuadNode.h"
void TerrainTextureState::update()
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
			for (size_t i = 0; i != cs.size(); ++i)
			{
				Chunk* c0 = cs[i]; 
				c0->setSelected(true);
				//
				eTerrainLayer ln = c0->getLayer(getGlobal()->getCurrentLayer());
				if (ln == eTerrainLayer_Size)//没找到该层
				{
					ln = c0->getNextLayer();//新加一层
					if (ln == eTerrainLayer_Size)
					{
						continue;//已满
					}
					c0->setLayer(ln, getGlobal()->getCurrentLayer());
				}
				for (int x = 0; x != n; ++x)
				for (int z = 0; z != n; ++z)
				{
					Vector2 p = c0->getWorldCoordination(x, z);
					float distance2 = (p.x - center.x)*(p.x - center.x) + (p.y - center.z)*(p.y - center.z);
					if (distance2 <= radius2)
					{
						Vector4 b = c0->getBlendFromTopology(x, z);
						b.w = 0.0f;
						b[ln] += cos(MATH_PI_Half * distance2/radius2) * getGlobal()->getBrushStrength() * 0.1f;
						b.w += cos(MATH_PI_Half * distance2/radius2) * getGlobal()->getBrushStrength() * 0.1f;
						c0->setBlendFromTopology(x, z, b);
					}
				}
				
				c0->refreshBlend();
			}
		}
	}
}

TerrainTextureState::~TerrainTextureState()
{

}

TerrainTextureState::TerrainTextureState()
{
	type_ = eState_TerrainTexture;
}

void TerrainTextureState::enter()
{
}

void TerrainTextureState::leave()
{
}

void TerrainTextureState::destroy()
{
}
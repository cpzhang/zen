#include "TerrainTextureState.h"
#include "misc/helper.h"
#include "misc/windowsHead.h"
#include "scene/Terrain.h"
#include "scene/SceneManager.h"
#include "Global.h"
#include "scene/Chunk.h"
#include "render/math.h"
#include "scene/QuadNode.h"
#include "FlowText.h"
//HBITMAP loadBrush(int nInnerRadius,int nOuterRadius,int nStrength, u8 *data)
//{
//	int innerRadius2 = nInnerRadius * nInnerRadius;
//	int outerRadius2 = nOuterRadius * nOuterRadius;
//	float invStrength = nStrength / 255.0f;
//	for(int i = 0;i < 256;i++)
//	for(int j = 0;j < 256;j++)
//	{
//		int index = (j * 256 + i) * 3;
//		int length = (i - 128) * (i - 128) + (j - 128) * (j - 128);
//		uchar c;
//		if(length > outerRadius2)
//		{
//			c = 0;
//		}
//		else if(length > innerRadius2)
//		{
//			c = 255 * (outerRadius2 - length) / (outerRadius2 - innerRadius2);
//		}
//		else
//		{
//			c = 255;
//		}
//		c *= invStrength;
//		data[index + 0] = c;
//		data[index + 1] = c;
//		data[index + 2] = c;
//	}
//}

void TerrainTextureState::update()
{
	if (isKeyDown(VK_LBUTTON))
	{
		if (getSceneManager() && getSceneManager()->getTerrain() && !getBrushTextureFile().empty())
		{
			Terrain* t = getSceneManager()->getTerrain();
			Vector4 center = getGlobal()->getBrushDecal()->getCenter();
			LOD* d = getSceneManager()->getLOD();
			int n = d->getVerticesNumberOneSide();
			float alphaMapInterval = d->getChunkSize() / (Chunk::tAlphaMapUnCompressedSize - 1);
			float radius = getGlobal()->getBrushDecal()->getRadius();
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
				eTerrainLayer ln = c0->getLayer(getBrushTextureFile());
				if (ln == eTerrainLayer_Size)//没找到该层
				{
					ln = c0->getNextLayer();//新加一层
					if (ln == eTerrainLayer_Size)
					{
						FlowText::getSingletonP()->add("纹理已满，无法增加Layer或修改AlphaMap", Vector4(1, 1, 1, 1));
						continue;//已满，需要删除某层才能继续添加
					}
					c0->setLayer(ln, getBrushTextureFile());
				}
				Vector2 baseP = c0->getWorldCoordination(0, n - 1);
				bool dirty = false;
				for (int x = 0; x != Chunk::tAlphaMapUnCompressedSize; ++x)
				for (int y = 0; y != Chunk::tAlphaMapUnCompressedSize; ++y)
				{
					Vector2 p;
					p.x = baseP.x + x * alphaMapInterval;
					p.y = baseP.y - y * alphaMapInterval;
					float distance2 = (p.x - center.x)*(p.x - center.x) + (p.y - center.z)*(p.y - center.z);
					{
						int c = 0;
						float outRadius2 = getBrushRadiusOuter();
						outRadius2 *= outRadius2;
						if (distance2 > outRadius2)
						{
							c = 0;
						}
						else 
						{
							float inRadius2 = getBrushRadiusInner();
							inRadius2 *= inRadius2;
							if (distance2 > inRadius2)
							{
								float r = (outRadius2 - distance2) / (outRadius2 - inRadius2);
								c = r * 255;
							}
							else
							{
								c = 255;
							}
						}
						if (c > 0)
						{
							dirty = true;
							c *= (getBrushStrength() / 255.0f);
							Vector4 a = c0->getAlphaMapUncompressed(x, y);//argb
							a[ln] += c;
							if (1)
							{
								if (a[ln] > 255)
								{
									a[ln] = 255;
								}
							} 
							else
							{
								if (a[0] > 255 || ln > eTerrainLayer_0)
								{
									a.normalize();
									a *= 255;
								}
							}
							
							c0->setAlphaMapUncompressed(x, y, Vector4(a.y, a.z, a.w, a.x));//rgba
						}
					}
				}
				if (dirty)
				{
					c0->refreshBlend();
				}
				//c0->saveAlphaMap("c:\\am.raw");
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
	BrushStrength_ = 1.0f;
	BrushRadiusInner_ = 3;
	BrushRadiusOuter_ = 6;
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

void TerrainTextureState::setBrushTextureFile( const tstring& n )
{
	std::ostringstream ss;
	ss<<"地表纹理刷选择文件："<<n;
	FlowText::getSingletonP()->add(ss.str(), Vector4(1, 1, 1, 1));
	BrushTextureFileSelected_ = n;
}

void TerrainTextureState::setBrushStrength( float f )
{
	IState::setBrushStrength(f);
	std::ostringstream ss;
	ss<<"画刷强度："<<f;
	FlowText::getSingletonP()->add(ss.str(), Vector4(1, 1, 1, 1));
}

void TerrainTextureState::setBrushRadiusInner( float f )
{
	std::ostringstream ss;
	ss<<"画刷内径："<<f;
	FlowText::getSingletonP()->add(ss.str(), Vector4(1, 1, 1, 1));
	BrushRadiusInner_ = f;
}

void TerrainTextureState::setBrushRadiusOuter( float f )
{
	std::ostringstream ss;
	ss<<"画刷外径："<<f;
	FlowText::getSingletonP()->add(ss.str(), Vector4(1, 1, 1, 1));
	BrushRadiusOuter_ = f;
}

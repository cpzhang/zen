#include "TerrainTextureState.h"
#include "misc/helper.h"
#include "misc/windowsHead.h"
#include "scene/Terrain.h"
#include "scene/SceneManager.h"
#include "scene/Chunk.h"
#include "render/math.h"
#include "render/Decal.h"
#include "scene/QuadNode.h"
#include "font/FlowText.h"
#include "Global.h"

void TerrainTextureState::update()
{
	if (getSceneManager() && getSceneManager()->getTerrain() && BrushDecalInner_ && BrushDecalOuter_)
	{
		Vector2 pp = getSceneManager()->getPickingPoint();
		BrushDecalInner_->setCenter(Vector4(pp.x, 0, pp.y, 1));
		BrushDecalOuter_->setCenter(Vector4(pp.x, 0, pp.y, 1));
	}
	if (isKeyDown(VK_LBUTTON))
	{
		if (getBrushTextureFile().empty())
		{
				FlowText::getSingletonP()->add("无法刷地表，未选中地表纹理文件", Vector4(1, 1, 1, 1));
			return;
		}
		if (getSceneManager() && getSceneManager()->getTerrain())
		{
			Terrain* t = getSceneManager()->getTerrain();
			Vector4 center = BrushDecalOuter_->getCenter();
			LOD* d = getSceneManager()->getLOD();
			int n = d->getVerticesNumberOneSide();
			float alphaMapInterval = d->getChunkSize() / (Chunk::tAlphaMapUnCompressedSize - 1);
			float radius = BrushDecalOuter_->getRadius();
			RectangleT rc;
			rc.left_ = center.x - radius;
			rc.right_ = center.x + radius;
			rc.bottom_ = center.z - radius;
			rc.top_ = center.z + radius;
			ChunkVec cs;
			getSceneManager()->getChunks(cs, getSceneManager()->getTerrainQuadTreeRoot(), rc);
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
							//如果是最外面一层，alpha累加
							if (ln == eTerrainLayer_3 || c0->getLayer((eTerrainLayer)(ln + 1))->getFileName().empty())
							{
								a[ln] += c;
								if (a[ln] > 255)
								{
									a[ln] = 255;
								}
							}
							//否则，其上面的层递减，递减为零时，自己那层再累加
							else
							{
								bool aboveHasAlpha = false;
								for (int lay = ln + 1; lay != eTerrainLayer_Size; ++lay)
								{
									if (!c0->getLayer((eTerrainLayer)lay)->getFileName().empty())
									{
										a[lay] -= c;
										if (a[lay] < 0)
										{
											a[lay] = 0;
										}
										if (a[lay] > 0)
										{
											aboveHasAlpha = true;
										}
									}
								}
								//
								if (!aboveHasAlpha)
								{
									a[ln] += c;
									if (a[ln] > 255)
									{
										a[ln] = 255;
									}
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
	BrushRadiusInner_ = 3;
	BrushRadiusOuter_ = 6;
	BrushDecalOuter_ = NULL;
	BrushDecalInner_ = NULL;
	BrushStrength_ = 10.0f;
}

void TerrainTextureState::enter()
{
	if (BrushDecalInner_ == NULL && BrushDecalOuter_ == NULL)
	{
		createBrushDecal();
	}
	BrushDecalInner_->setVisible(true);
	BrushDecalOuter_->setVisible(true);
}

void TerrainTextureState::leave()
{
	BrushDecalInner_->setVisible(false);
	BrushDecalOuter_->setVisible(false);
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

void TerrainTextureState::setBrushRadiusInner( float f )
{
	std::ostringstream ss;
	ss<<"画刷内径："<<f;
	FlowText::getSingletonP()->add(ss.str(), Vector4(1, 1, 1, 1));
	BrushRadiusInner_ = f;
	if (BrushDecalInner_)
	{
		BrushDecalInner_->setRadius(BrushRadiusInner_);
	}
}

void TerrainTextureState::setBrushRadiusOuter( float f )
{
	std::ostringstream ss;
	ss<<"画刷外径："<<f;
	FlowText::getSingletonP()->add(ss.str(), Vector4(1, 1, 1, 1));
	BrushRadiusOuter_ = f;
	if (BrushDecalOuter_)
	{
		BrushDecalOuter_->setRadius(BrushRadiusOuter_);
	}
}

bool TerrainTextureState::createBrushDecal()
{
	BrushDecalInner_ = getSceneManager()->createDecal(TEXT("BrushDecalInner"));
	BrushDecalInner_->setFxFile(TEXT("\\shader\\PositionDecal.fx"));
	BrushDecalInner_->setTexture(TEXT("\\brush\\heighttool.dds"));
	BrushDecalInner_->setRadius(getBrushRadiusInner());
	//
	BrushDecalOuter_ = getSceneManager()->createDecal(TEXT("BrushDecalOuter"));
	BrushDecalOuter_->setFxFile(TEXT("\\shader\\PositionDecal.fx"));
	BrushDecalOuter_->setTexture(TEXT("\\brush\\heighttool.dds"));
	BrushDecalOuter_->setRadius(getBrushRadiusOuter());
	return true;
}

void TerrainTextureState::setBrushStrength( float f )
{
	BrushStrength_ = f;
}

void TerrainTextureState::setModelFile( const tstring& mf )
{
	BrushTextureFileSelected_ = "brush\\";
	BrushTextureFileSelected_ += mf;
	std::ostringstream ss;
	ss<<"地表纹理刷选择文件："<<BrushTextureFileSelected_;
	FlowText::getSingletonP()->add(ss.str(), Vector4(1, 1, 1, 1));
}

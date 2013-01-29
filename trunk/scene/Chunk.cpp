#include "Chunk.h"
#include "scene/SceneManager.h"
#include "render/rendercontext.h"
#include "render/BufferLocker.h"
#include "render/TextureManager.h"
#include "Terrain.h"
#include "render/Colour.h"
#include "tinyXML2/tinyxml2.h"
Chunk::Chunk( int x, int z )
{
	clear_();
	x_ = x;
	z_ = z;
	//
	worldMatrix_.setIdentity();
	float s = getSceneManager()->getLOD()->getScale();
	float len = getSceneManager()->getLOD()->getLengthOneSide();
	worldMatrix_.setTranslate(x_*len, 0.0f, z_*len);
	//
	center_.x = x_*len + 0.5f*len;
	center_.y = 0.0f;
	center_.z = z_*len + 0.5f*len;
	//
	int vn = getSceneManager()->getLOD()->getVerticesNumberOneChunk();
	vertexBuffer_.create(vn * sizeof(sVDT_PositionColorTexture), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED);
	refreshHeight();
	//heights_.resize(vn, 0);
	//
	rect_.left_ = x_*len;
	rect_.right_ = x_*len + len;
	rect_.bottom_ = z*len;
	rect_.top_ = z*len + len;
}

Chunk::~Chunk()
{
	clear_();
}

void Chunk::clear_()
{
	isSelected_ = false;
	x_ = 0;
	z_ = 0;
	worldMatrix_.setIdentity();
	nIndices_ = 0;
	maxHeight_ = 0.0f;
	minHeight_ = 0.0f;
	//heights_.clear();
	layers_.resize(eTerrainLayer_Size, Texture::getNullObject());
}

void Chunk::render()
{
	vertexBuffer_.apply(0, 0, sizeof(sVDT_PositionColorTexture));
	getSceneManager()->getLOD()->getIndexBuffer()->apply();
	getRenderContex()->drawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, getSceneManager()->getLOD()->getVerticesNumberOneChunk(), 0, 
		getSceneManager()->getLOD()->getPrimitiveNumber());
}

Matrix& Chunk::getWorldMatrix()
{
	return worldMatrix_;
}

void Chunk::destroy()
{
	vertexBuffer_.destroy();
}

float Chunk::getHeightFromTopology( int x, int z )
{
	int n = getSceneManager()->getLOD()->getVerticesNumberOneSide();
	return getSceneManager()->getTerrain()->getHeightFromImage(x_ * (n - 1) + x, z_ * (n - 1) +z);
}

void Chunk::setHeightFromTopology( int x, int z, float h )
{
	//
	int n = getSceneManager()->getLOD()->getVerticesNumberOneSide();
	getSceneManager()->getTerrain()->setHeightFromImage(x_ * (n - 1) + x, z_ * (n - 1) +z, h);
}

void Chunk::refreshHeight()
{
	refreshVB_();
	//
	calcMaxMinHeight_();
}

Vector2 Chunk::getWorldCoordination( int x, int z )
{
	Vector2 p;
	float len = getSceneManager()->getLOD()->getLengthOneSide();
	float s = getSceneManager()->getLOD()->getScale();
	p.x = x_ * len + x*s;
	p.y = z_ * len + z*s;
	return p;
}

RectangleT Chunk::getRect()
{
	return rect_;
}

void Chunk::setSelected( bool b )
{
	isSelected_ = b;
}

bool Chunk::isSelected()
{
	return isSelected_;
}

int Chunk::getNumberX()
{
	return x_;
}

int Chunk::getNumberZ()
{
	return z_;
}

float Chunk::getMaxHeight()
{
	return maxHeight_;
}

float Chunk::getMinHeight()
{
	return minHeight_;
}

void Chunk::calcMaxMinHeight_()
{
	int vn = getSceneManager()->getLOD()->getVerticesNumberOneSide();
	float s = getSceneManager()->getLOD()->getScale();
	for (size_t z = 0; z != vn; z++)
	for (size_t x = 0; x != vn; x++)
	{
		maxHeight_ = max(getHeightFromTopology(x, z), maxHeight_);
		minHeight_ = min(getHeightFromTopology(x, z), minHeight_);
	}
}

Vector2 Chunk::getMaxMinHeightFromTopology( int x0, int x1, int z0, int z1 )
{
	Vector2 p = Vector2::Zero;
	int vn = getSceneManager()->getLOD()->getVerticesNumberOneSide();
	float s = getSceneManager()->getLOD()->getScale();
	for (size_t z = z0; z <= z1; z++)
	for (size_t x = x0; x <= x1; x++)
	{
		p.x = max(getHeightFromTopology(x, z), p.x);
		p.y = min(getHeightFromTopology(x, z), p.y);
	}
	return p;
}

Texture* Chunk::getLayer( eTerrainLayer e )
{
	return layers_[e];
}

eTerrainLayer Chunk::getLayer( const tstring & resourceID )
{
	for (int i = 0; i != eTerrainLayer_Size; ++i)
	{
		if (layers_[i]->getFileName() == resourceID)
		{
			return (eTerrainLayer)i;
		}
	}
	return eTerrainLayer_Size;
}

void Chunk::setLayer( eTerrainLayer e, const tstring & resourceID)
{
	layers_[e] = getTextureManager()->getTexture(resourceID);
}

Vector4 Chunk::getBlendFromTopology( int x, int z )
{
	int n = getSceneManager()->getLOD()->getVerticesNumberOneSide();
	return getSceneManager()->getTerrain()->getBlendFromImage(x_ * (n - 1) + x, z_ * (n - 1) +z);
}

void Chunk::setBlendFromTopology( int x, int z, Vector4 h )
{
	int n = getSceneManager()->getLOD()->getVerticesNumberOneSide();
	getSceneManager()->getTerrain()->setBlendFromImage(x_ * (n - 1) + x, z_ * (n - 1) +z, h);
}

void Chunk::refreshBlend()
{
	refreshVB_();
}

void Chunk::refreshVB_()
{
	std::vector<sVDT_PositionColorTexture> vertices;
	int vn = getSceneManager()->getLOD()->getVerticesNumberOneSide();
	float s = getSceneManager()->getLOD()->getScale();
	float fv = 1.0f/(float)(vn-1);
	for (size_t z = 0; z != vn; z++)
	for (size_t x = 0; x != vn; x++)
	{
		sVDT_PositionColorTexture v;
		v.position_.x = x*s;
		v.position_.z = z*s;
		v.position_.y = int(getHeightFromTopology(x, z));
		//
		v.color_ = Colour::getUint32FromNormalised(getBlendFromTopology(x,z));
		//
		v.texture_.x = x == vn-1 ? 1.0f : (float)x * fv;
		v.texture_.y = z == vn-1 ? 1.0f : (float)z * fv;
		vertices.push_back(v);
	}
	BufferLocker<VertexBuffer, sVDT_PositionColorTexture> vl(vertexBuffer_);
	vl.fill(&vertices[0], vertices.size()*sizeof(sVDT_PositionColorTexture));
}

eTerrainLayer Chunk::getNextLayer()
{
	for (int i = 0; i != eTerrainLayer_Size; ++i)
	{
		if (layers_[i]->getFileName().empty())
		{
			return (eTerrainLayer)i;
		}
	}
	return eTerrainLayer_Size;
}

void Chunk::save( const tstring& path )
{
	//============================================================================
	tinyxml2::XMLDocument doc;
	// 
	tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
	doc.LinkEndChild(dec);
	//
	tinyxml2::XMLElement* ele = doc.NewElement("chunk");
	//
	doc.LinkEndChild(ele);
	//
	for (int i = 0; i != eTerrainLayer_Size; ++i)
	{
		if (layers_[i] && !layers_[i]->getFileName().empty())
		{
			tinyxml2::XMLElement* e = doc.NewElement("layer");
			e->SetAttribute("texture", layers_[i]->getFileName().c_str());
			ele->LinkEndChild(e);		
		}
	}	
	doc.SaveFile(path.c_str());
}

void Chunk::open( const tstring& path )
{
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(path.c_str()))
	{
		return;
	}
	tinyxml2::XMLElement* ele = doc.RootElement();
	if (NULL == ele)
	{
		return;
	}
	tinyxml2::XMLElement* tex= ele->FirstChildElement("layer");
	eTerrainLayer l = eTerrainLayer_0;
	while (tex)
	{
		//
		{
			const char* n = tex->Attribute("texture");
			if (NULL != n)
			{
				setLayer(l, n);
			}
		}
		tex = tex->NextSiblingElement("layer");
		l = (eTerrainLayer)(l + 1);
	}
}

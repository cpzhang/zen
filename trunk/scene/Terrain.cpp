#include "Terrain.h"
#include "Chunk.h"
#include "render/FxManager.h"
#include "render/Fx.h"
#include "render/Texture.h"
#include "render/TextureManager.h"
#include "render/rendercontext.h"
#include "LOD.h"
#include "scene/SceneManager.h"
#include "misc/DataChunk.h"
void Terrain::save(const tstring& path)
{
	//
	{
		//保存高度及混合系数，一次读出
		tstring heightmapfile(path + "heightmap.raw");
		// 开始写入数据 
		ChunkSet cs;
		//============================================================================
		// 版本号
		cs.beginChunk("MVER");	
		cs.write(&mVersion, sizeof(mVersion));
		cs.endChunk();
		//============================================================================
		// 高度
		cs.beginChunk("HEIG");
		writeSequenceEx(cs, heights_);
		cs.endChunk();
		// 混合因子
		cs.beginChunk("BLEN");
		writeSequenceEx(cs, blends_);
		cs.endChunk();
		//============================================================================
		// 保存文件，结束
		cs.save(heightmapfile);
	}
	//保存chunk
	{
		for (size_t i = 0; i != chunks_.size(); ++i)
		{
			std::stringstream ss;
			ss<<path<<chunks_[i]->getNumberX()<<"_"<<chunks_[i]->getNumberZ()<<".xml";
			chunks_[i]->save(ss.str());
		}
	}
}

void Terrain::setFileName( const tstring& fn )
{
	fileName_ = fn;
}

Terrain::Terrain()
{
	clear_();
}

Terrain::~Terrain()
{
	clear_();
}

void Terrain::create( int xChunks, int zChunks )
{
	xChunkNumber_ = xChunks;
	zChunkNumber_ = zChunks;
	destroyChunks_();
	//
	for (int z = 0; z < zChunks; ++z)
	for (int x = 0; x < xChunks; ++x)
	{
		Chunk* ck = new Chunk(x, z);
		chunks_.push_back(ck);
	}
	//
	int n = getSceneManager()->getLOD()->getVerticesNumberOneSide();
	totalNumberX_ = xChunks * n - (xChunks - 1);
	totalNumberZ_ = zChunks * n - (zChunks - 1);
	heights_.resize(totalNumberX_ * totalNumberZ_, 0.0f);
	blends_.resize(totalNumberX_ * totalNumberZ_, Vector4::Zero);
}

void Terrain::destroyChunks_()
{
	heights_.clear();
	blends_.clear();
	if (!chunks_.empty())
	{
		for (size_t i = 0; i != chunks_.size(); ++i)
		{
			Chunk* ck = chunks_[i];
			if (ck)
			{
				ck->destroy();
				delete ck;
			}
		}
		chunks_.clear();
	}
}

void Terrain::destroy()
{
	destroyChunks_();
}

void Terrain::render()
{
	ChunkVec::iterator itBegin, itEnd;
	if (getSceneManager()->IsAllChunksVisible())
	{
		itBegin = chunks_.begin();
		itEnd = chunks_.end();
	}
	else
	{
		itBegin = chunksVisible_.begin();
		itEnd = chunksVisible_.end();
	}
	if (itBegin == itEnd)
	{
		return;
	}
	if (NULL == fx_)
	{
		return;
	}
	ID3DXEffect* ef = fx_->getDxEffect();
	if (NULL == ef)
	{
		return;
	}
	//Handles provide an efficient means for referencing effect parameters, techniques, passes, and annotations with an effect. 
	D3DXHANDLE p = ef->GetParameterByName(NULL,Fx::getProjectionMatrixName());
	D3DXHANDLE v = ef->GetParameterByName(NULL,Fx::getViewMatrixName());
	D3DXHANDLE w = ef->GetParameterByName(NULL,Fx::getWorldMatrixName());	
	//
	UINT passes = 0;
	ef->SetTechnique("T2");
	ef->Begin(&passes, D3DXFX_DONOTSAVESTATE);
	if (passes > 0)
	{
		if (v)
		{
			ef->SetMatrix(v, &getRenderContex()->getViewMatrix());
		}
		if (p)
		{
			ef->SetMatrix(p, &getRenderContex()->getProjectionMatrix());
		}
		getRenderContex()->setVertexDeclaration(sVDT_PositionColorTexture::getType());
	}
	Vector4 nc(1.0, 0.0, 0.0, 1.0);
	Vector4 sc(0.0, 1.0, 0.0, 1.0);
	for (ChunkVec::iterator it = itBegin; it != itEnd; ++it)
	{
		Chunk* ck = *it;
		if (NULL == ck)
		{
			continue;
		}
		if (w)
		{
			ef->SetMatrix(w, &ck->getWorldMatrix());
		}
		if (ck->isSelected())
		{
			ef->SetVector("gAmbient", &sc);
		}
		else
		{
			ef->SetVector("gAmbient", &nc);
		}
		textureSetter_.apply(ck);
		ef->CommitChanges();
		for (int p = 0; p != passes; ++p)
		{
			ef->BeginPass(p);
			ck->render();
			ef->EndPass();
		}
	}
	ef->End();
}

void Terrain::setFX( const tstring& fn )
{
	fx_ = getFxManager()->getFx(fn);
	textureSetter_.setDxEffect(fx_);
}

Chunk* Terrain::getChunkFromWorldSpacePostion( float x, float z )
{
	if (!isXInside_(x) || !isZInside_(z))
	{
		return NULL;
	}
	float lenghthOneSide = getSceneManager()->getLOD()->getLengthOneSide();
	int xn = x / lenghthOneSide;
	int zn = z / lenghthOneSide;
	return chunks_[zn * xChunkNumber_ + xn];
}
//以主角为中心的九个chunks可见
void Terrain::updateVisibleChunks( float x, float z )
{
	chunksVisible_.clear();
	float lenghthOneSide = getSceneManager()->getLOD()->getLengthOneSide();
	int xn = x / lenghthOneSide;
	int zn = z / lenghthOneSide;
	for (int xi = -1; xi <= 1; ++xi)
	for (int zi = -1; zi <= 1; ++zi)
	{
		int xk = xi + xn;
		if (xk < 0 || xk >= xChunkNumber_)
		{
			continue;
		}
		int zk = zi + zn;
		if (zk < 0 || zk >= zChunkNumber_)
		{
			continue;
		}
		chunksVisible_.push_back(chunks_[zk * xChunkNumber_ + xk]);
	}
}

bool Terrain::isXInside_( float f )
{
	float lenghthOneSide = getSceneManager()->getLOD()->getLengthOneSide();
	float lengthAll = lenghthOneSide * xChunkNumber_;
	if (f < 0.0f || f > lengthAll)
	{
		return false;
	}
	return true;
}

bool Terrain::isZInside_( float f )
{
	float lenghthOneSide = getSceneManager()->getLOD()->getLengthOneSide();
	float lengthAll = lenghthOneSide * zChunkNumber_;
	if (f < 0.0f || f > lengthAll)
	{
		return false;
	}
	return true;
}

void Terrain::renderDecal( Decal* dc )
{
	if (NULL == dc)
	{
		return;
	}
	ChunkVec::iterator itBegin, itEnd;
	if (getSceneManager()->IsAllChunksVisible())
	{
		itBegin = chunks_.begin();
		itEnd = chunks_.end();
	}
	else
	{
		itBegin = chunksVisible_.begin();
		itEnd = chunksVisible_.end();
	}
	if (itBegin == itEnd)
	{
		return;
	}
	Fx* fx = getFxManager()->getFx(dc->getFxFile());
	if (NULL == fx)
	{
		return;
	}
	ID3DXEffect* ef = fx->getDxEffect();
	if (NULL == ef)
	{
		return;
	}
	//Handles provide an efficient means for referencing effect parameters, techniques, passes, and annotations with an effect. 
	D3DXHANDLE p = ef->GetParameterByName(NULL,Fx::getProjectionMatrixName());
	D3DXHANDLE v = ef->GetParameterByName(NULL,Fx::getViewMatrixName());
	D3DXHANDLE w = ef->GetParameterByName(NULL,Fx::getWorldMatrixName());
	//
	UINT passes = 0;
	ef->SetTechnique("T2");
	ef->Begin(&passes, D3DXFX_DONOTSAVESTATE);
	if (passes > 0)
	{
		if (v)
		{
			ef->SetMatrix(v, &getRenderContex()->getViewMatrix());
		}
		if (p)
		{
			ef->SetMatrix(p, &getRenderContex()->getProjectionMatrix());
		}
		getRenderContex()->setVertexDeclaration(sVDT_Position::getType());
		//
		Texture* tex = getTextureManager()->getTexture(dc->getTextureFile());
		if (tex)
		{
			ef->SetTexture("gTexture", tex->getDxTexture());
		}
		ef->SetVector("gCenter", &dc->getCenter());
		ef->SetFloat("gRadius", dc->getRadius());
	}
	for (ChunkVec::iterator it = itBegin; it != itEnd; ++it)
	{
		Chunk* ck = *it;
		if (NULL == ck)
		{
			continue;
		}
		if (w)
		{
			ef->SetMatrix(w, &ck->getWorldMatrix());
		}

		ef->CommitChanges();
		for (int p = 0; p != passes; ++p)
		{
			ef->BeginPass(p);
			if (p != passes - 1)
			{
				ck->render();
			}
			ef->EndPass();
		}
	}
	ef->End();
}

Chunk* Terrain::getChunkFromTopology( int x, int z )
{
	if (x < 0 || x >= xChunkNumber_)
	{
		return NULL;
	}
	if (z < 0 || z >= zChunkNumber_)
	{
		return NULL;
	}
	return chunks_[z * xChunkNumber_ + x];
}

void Terrain::clearSelected()
{
	for (size_t i = 0; i != chunks_.size(); ++i)
	{
		Chunk* ck = chunks_[i];
		if (ck)
		{
			ck->setSelected(false);
		}
	}
}

float Terrain::getHeightFromImage( int x, int z )
{
	int index = z * totalNumberX_ + x;
	if (index < 0 || index >= heights_.size())
	{
		return 0.0f;
	}
	return heights_[index];
}

void Terrain::setHeightFromImage( int x, int z, float h )
{
	int index = z * totalNumberX_ + x;
	if (index < 0 || index >= heights_.size())
	{
		return;
	}
	heights_[index] = h;
}

void Terrain::clear_()
{
	totalNumberX_ = 0;
	totalNumberZ_ = 0;
	fileName_.clear();
	xChunkNumber_ = 0;
	zChunkNumber_ = 0;
	fx_ = NULL;
	heights_.clear();
	blends_.clear();
	mVersion = 1;
}

Vector4 Terrain::getBlendFromImage( int x, int z )
{
	int index = z * totalNumberX_ + x;
	if (index < 0 || index >= blends_.size())
	{
		return Vector4::Zero;
	}
	return blends_[index];
}

void Terrain::setBlendFromImage( int x, int z, Vector4 b )
{
	int index = z * totalNumberX_ + x;
	if (index < 0 || index >= blends_.size())
	{
		return;
	}
	Vector3 p(b.x, b.y, b.z);
	p.normalise();
	b.x = p.x;
	b.y = p.y;
	b.z = p.z;
	blends_[index] = b;
}

void Terrain::open( const tstring& path )
{
	//保存高度及混合系数，一次读出
	tstring heightmapfile(path + "/heightmap.raw");
	//
	{
		//
		std::ifstream f(heightmapfile.c_str(), std::ios::binary);
		if (!f.good())
		{
			return;
		}

		u32 version = 0;
		//	[Tag Size Data]
		int t;
		int s;
		while(f.good())
		{
			t = 0;
			s = 0;
			f.read((char*)&t, sizeof(int));
			f.read((char*)&s, sizeof(int));

			if (s <= 0)
			{
				continue;
			}

			char c[5];
			c[0] = *((char*)&t + 3);
			c[1] = *((char*)&t + 2);
			c[2] = *((char*)&t + 1);
			c[3] = *((char*)&t + 0);
			c[4] = 0;

			switch (t)
			{
			case 'MVER':
				{
					f.read((char*)&mVersion, sizeof(mVersion));
				}break;
			case 'HEIG':
				{
					readSequenceEx<std::vector<float>, float>(f, heights_);
				}break;
			case 'BLEN':
				{
					readSequenceEx<std::vector<Vector4>, Vector4>(f, blends_);
				}break;
			default:
				{
					f.ignore(s);
				}break;
			}
		}
	}
	//读取chunk
	{
		for (size_t i = 0; i != chunks_.size(); ++i)
		{
			std::stringstream ss;
			ss<<path<<"/"<<chunks_[i]->getNumberX()<<"_"<<chunks_[i]->getNumberZ()<<".xml";
			chunks_[i]->open(ss.str());
			chunks_[i]->refreshHeight();
		}
	}
}

tstring Terrain::getFXFileName()
{
	return fx_->getFilePath();
}
float getHeight(const Vector3& a, const Vector3& b, const Vector3& c, float x, float z)
{
	// x = a.x * s + b.x * t + c.x * (1 - s - t)
	// z = a.z * s + b.z * t + c.z * (1 - s - t)
	float m = a.x- c.x;
	float n = b.x - c.x;
	float p = x - c.x;
	float h = a.z - c.z;
	float i = b.z - c.z;
	float j = z - c.z;
	float s = (n*j - p*i) / (h*n - m*i);
	float t = (h*p - m*j) / (h*n - m*i);

	return (a.y * s + b.y * t + c.y * (1 - s - t));
}
float Terrain::getHeightFromeWorldSpacePosition( float x, float z )
{
	if (!isXInside_(x) || !isZInside_(z))
	{
		return 0.0f;
	}
	float s = getSceneManager()->getLOD()->getScale();
	int xb = x / s;
	int zb = z / s;
	int xn = xb;
	int zn = zb;
	Vector3 lb(xn * s, heights_[zn * totalNumberX_ + xn], zn *s);
	xn = xb; zn = zb + 1;
	Vector3 lt(xn * s, heights_[zn * totalNumberX_ + xn], zn *s);
	xn = xb + 1; zn = zb;
	Vector3 rb(xn * s, heights_[zn * totalNumberX_ + xn], zn *s);
	xn = xb + 1; zn = zb + 1;
	Vector3 rt(xn * s, heights_[zn * totalNumberX_ + xn], zn *s);
	//以\为对角线，判定属于上三角形还是下三角形，判定准则：点p到lb的距离小于或等于到rt的距离，则位于下三角形；反之，上三角形
	float dlb = (x - lb.x) * (x - lb.x) + (z - lb.z) * (z - lb.z);
	float drt = (x - rt.x) * (x - rt.x) + (z - rt.z) * (z - rt.z);
	if (dlb <= drt)
	{
		return getHeight(lb, lt, rb, x, z);
	}
	return getHeight(lt, rb, rt, x, z);
}
